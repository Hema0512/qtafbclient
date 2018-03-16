/*
 * Copyright (C) 2018 Iot.Bzh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <Qsci/qscilexerjson.h>

/*!
 * \brief Convert a QJsonValue to a string representation.
 * \param v Value to convert.
 * \param fmt Format to use (indented or compact)
 * \return A string representation of a QJsonValue.
 */
QString toString(const QJsonValue& v, QJsonDocument::JsonFormat fmt = QJsonDocument::Indented)
{
    switch(v.type())
    {
    case QJsonValue::Bool:      return v.toBool() ? "true" : "false";
    case QJsonValue::Double:    return QString::number(v.toDouble());
    case QJsonValue::String:    return v.toString();
    case QJsonValue::Null:      return "null";
    case QJsonValue::Undefined: return "undefined";
    }

    QJsonDocument doc;
    switch(v.type())
    {
    case QJsonValue::Array:
        doc.setArray(v.toArray());
        break;
    case QJsonValue::Object:
        doc.setObject(v.toObject());
        break;
    }

    return doc.toJson(fmt);
}

/*!
 * \brief Default contructor for MainWindow.
 * \param parent Parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
{
    ui->setupUi(this);
    ui->tbFeatures->setCurrentIndex(0);
    ui->tbFeatures->setEnabled(false);

    ui->txtCallArg->setMarginLineNumbers(1, true);
    ui->txtCallArg->setMarginWidth(1, 30);
    ui->txtCallArg->setLexer(&m_jsonLexer);

    ui->txtCallResult->setMarginLineNumbers(1, true);
    ui->txtCallResult->setMarginWidth(1, 30);
    ui->txtCallResult->setLexer(&m_jsonLexer);
    ui->txtCallResult->setReadOnly(true);

    ui->txtDebug->setMarginLineNumbers(1, true);
    ui->txtDebug->setMarginWidth(1, 30);
    ui->txtDebug->setLexer(&m_jsonLexer);
    ui->txtDebug->setReadOnly(true);

    QFont fnt = ui->txtDebug->font();
    fnt.setPointSize(fnt.pointSize() + 2);
    m_jsonLexer.setFont(fnt);

    connect(ui->btnConnection, SIGNAL(clicked(bool)), this, SLOT(onBtnConnectionClicked(bool)));
    connect(ui->btnCustomText, SIGNAL(clicked(bool)), this, SLOT(onBtnCustomTextClicked(bool)));
    connect(ui->lstDebug, SIGNAL(itemSelectionChanged()), this, SLOT(onLstDebugItemSelectionChanged()));
    connect(ui->cbApi, SIGNAL(currentIndexChanged(QString)), this, SLOT(onCbApiCurrentIndexChanged(QString)));
    connect(ui->btnCall, SIGNAL(clicked(bool)), this, SLOT(onBtnCallClicked(bool)));

    connect(&m_client, SIGNAL(connected()), this, SLOT(onClientConnected()));
    connect(&m_client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    connect(&m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onClientError(QAbstractSocket::SocketError)));
    connect(&m_client, SIGNAL(textReceived(QString)), this, SLOT(onClientTextReceived(QString)));
    connect(&m_client, SIGNAL(textSent(QString)), this, SLOT(onClientTextSent(QString)));
    connect(&m_client, SIGNAL(event(QString,QJsonValue)), this, SLOT(onClientEvent(QString, QJsonValue)));
}

/*!
 * \brief Destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Slot for when the btnConnection is clicked.
 */
void MainWindow::onBtnConnectionClicked(bool /*checked*/)
{
    if (m_client.isValid()) m_client.close();
    else
    {
        QUrl url{ui->txtConnection->text()};
        if (url.isValid()) m_client.open(url);
        else QMessageBox::critical(this, tr("Error"), tr("Invalid URL!"));
    }
}

/*!
 * \brief Slot for when the btnCustomText is clicked.
 */
void MainWindow::onBtnCustomTextClicked(bool /*checked*/)
{
    if (ui->txtCustomText->text().size())
        m_client.sendTextMessage(ui->txtCustomText->text());
}

/*!
 * \brief Slot for when selection changed in lstDebugItem.
 */
void MainWindow::onLstDebugItemSelectionChanged()
{
    QString result = "";
    for(QListWidgetItem* item : ui->lstDebug->selectedItems())
    {
        QJsonDocument doc = QJsonDocument::fromJson(item->text().toUtf8());
        result += doc.toJson(QJsonDocument::Indented) + "\n";
    }
    ui->txtDebug->setText(result);
}

/*!
 * \brief Slot for when client is connected.
 */
void MainWindow::onClientConnected()
{
    ui->statusBar->showMessage(tr("Connected!"));
    ui->btnConnection->setText(tr("Disconnect"));
    ui->tbFeatures->setEnabled(true);
    ui->txtConnection->setEnabled(false);

    m_client.call("monitor", "get", QJsonObject{{"apis", true}}, std::bind(&MainWindow::onMonitorGet, this, std::placeholders::_1, std::placeholders::_2));
}

/*!
 * \brief Slot for when client is disconnected.
 */
void MainWindow::onClientDisconnected()
{
    if (isVisible()) // Disconnection can happen when closing the window.
    {
        ui->statusBar->showMessage(tr("Disconnected!"));
        ui->btnConnection->setText(tr("Connect"));
        ui->tbFeatures->setEnabled(false);
        ui->txtConnection->setEnabled(true);
    }
}

/*!
 * \brief Slot for when client emit an error.
 */
void MainWindow::onClientError(QAbstractSocket::SocketError /*e*/)
{
    if (isVisible()) ui->statusBar->showMessage(m_client.errorString());
}

/*!
 * \brief Slot for when client received a text message.
 * \param msg Message received.
 */
void MainWindow::onClientTextReceived(QString msg)
{
    QListWidgetItem* item = new QListWidgetItem(QIcon(":/receive_msg.png"), msg);
    item->setData(Qt::ToolTipRole, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    if(msg.startsWith("[5"))
        item->setBackgroundColor(QColor::fromRgb(255, 128, 0));

    ui->lstDebug->addItem(item);
}

/*!
 * \brief Slot for when client sent a text message.
 * \param msg Message sent.
 */
void MainWindow::onClientTextSent(QString msg)
{
    QListWidgetItem* item = new QListWidgetItem(QIcon(":/emit_msg.png"), msg);
    item->setData(Qt::ToolTipRole, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    ui->lstDebug->addItem(item);
}

/*!
 * \brief Slot for when client received an event.
 * \param eventName Name of the event (api/event)
 * \param data Data associated to the event.
 */
void MainWindow::onClientEvent(QString eventName, const QJsonValue& data)
{
    QMessageBox::information(this, "Event Received", eventName + ":\n\n" + toString(data));
}

/*!
 * \brief Slot for when btnCall is clicked.
 */
void MainWindow::onBtnCallClicked(bool /*checked*/)
{
    QJsonDocument doc = QJsonDocument::fromJson(ui->txtCallArg->text().toUtf8());
    m_client.call(ui->cbApi->currentText(), ui->cbVerb->currentText(), doc.object(), [this](bool r, const QJsonValue& a){
        ui->txtCallResult->setText(toString(a));
    });
}

/*!
 * \brief Callback used when the "monitor/get" call's response is available.
 * \param success Indicate if the call is a success or not.
 * \param response Response associated.
 */
void MainWindow::onMonitorGet(bool success, const QJsonValue& response)
{
    if (!success) return;
    QJsonObject apis = response.toObject()["response"].toObject()["apis"].toObject();

    m_apis.clear();
    ui->cbApi->blockSignals(true);
    ui->cbVerb->blockSignals(true);
    ui->cbApi->clear();
    ui->cbVerb->clear();

    for(QJsonObject::Iterator itA = apis.begin(); itA != apis.end(); ++itA)
    {
        QVector<QString> verbs;
        /// add verbs
        QJsonObject paths = itA.value().toObject()["paths"].toObject();
        for(QJsonObject::Iterator itV = paths.begin(); itV != paths.end(); ++itV)
        {
            verbs.append(itV.key().remove(0, 1));
        }
        m_apis[itA.key()] = verbs;
        ui->cbApi->addItem(itA.key());
    }

    ui->cbApi->blockSignals(false);
    ui->cbVerb->blockSignals(false);
    onCbApiCurrentIndexChanged(ui->cbApi->itemText(0));
}

/*!
 * \brief Slot for when current index changed in cbApi.
 * \param item Current text in cbApi.
 */
void MainWindow::onCbApiCurrentIndexChanged(QString item)
{
    ui->cbVerb->blockSignals(true);
    ui->cbVerb->clear();
    for(const auto& v : m_apis[item])
    {
        ui->cbVerb->addItem(v);
    }
    ui->cbVerb->blockSignals(false);
}
