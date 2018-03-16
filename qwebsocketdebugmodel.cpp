#include "qwebsocketdebugmodel.h"

QWebsocketDebugModel::QWebsocketDebugModel(QObject* parent)
    : QAbstractTableModel{parent}
{
}

int QWebsocketDebugModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_messages.count();
}

int QWebsocketDebugModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant QWebsocketDebugModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.row() < m_messages.count() && index.column() < 3)
        {
            const QWebsocketDebugMessage& m = m_messages[index.row()];
            switch(index.column())
            {
            case 0: return m.timestamp();
            case 1: return m.direction();
            case 2: return m.text();
            }
        }
    }
    return QVariant();
}

void QWebsocketDebugModel::append(const QString &direction, const QString &text)
{
    m_messages.append(QWebsocketDebugMessage(direction, text));
    emit dataChanged(createIndex(m_messages.count() - 1, 0), createIndex(m_messages.count() - 1, 2));
}

void QWebsocketDebugModel::clear()
{
    int lastRow = m_messages.count() - 1;
    m_messages.clear();
    emit dataChanged(createIndex(0, 0), createIndex(lastRow, 2));
}
