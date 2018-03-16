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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QVector>

#include <Qsci/qscilexerjson.h>
#include "qafbwebsocketclient.h"

namespace Ui {
class MainWindow;
}

/*!
 * \brief The main app's window.
 */
class MainWindow
    : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onBtnConnectionClicked(bool /*checked*/);
    void onBtnCustomTextClicked(bool /*checked*/);
    void onLstDebugItemSelectionChanged();
    void onClientConnected();
    void onClientDisconnected();
    void onClientError(QAbstractSocket::SocketError /*e*/);
    void onClientTextReceived(QString msg);
    void onClientTextSent(QString msg);
    void onClientEvent(QString eventName, const QJsonValue& data);
    void onBtnCallClicked(bool /*checked*/);
    void onMonitorGet(bool success, const QJsonValue& response);
    void onCbApiCurrentIndexChanged(QString item);

private:
    Ui::MainWindow* ui;
    QsciLexerJSON m_jsonLexer;
    QAfbWebsocketClient m_client;
    QMap<QString, QVector<QString>> m_apis;
};

#endif // MAINWINDOW_H
