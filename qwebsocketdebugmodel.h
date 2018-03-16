#ifndef QWEBSOCKETDEBUGMODEL_H
#define QWEBSOCKETDEBUGMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "qwebsocketdebugmessage.h"

class QWebsocketDebugModel
    : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit QWebsocketDebugModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void append(const QString& direction, const QString& text);
    void clear();

private:
    QList<QWebsocketDebugMessage> m_messages;
};

#endif // QWEBSOCKETDEBUGMODEL_H
