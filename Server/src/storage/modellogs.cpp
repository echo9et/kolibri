#include "modellogs.h"

ModelLogs::ModelLogs(QObject *parent)
    : QAbstractListModel{parent}
{}

void ModelLogs::setServerLog(QList<ENTITIES::Log> *logs)
{
    p_serverLogs = logs;
}

int ModelLogs::rowCount(const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return p_data ? p_data->count() : 0;
}

QVariant ModelLogs::data(const QModelIndex &index, int role) const
{
    if (!p_data || index.row() < 0 || index.row() >= p_data->size())
    {
        return QVariant();
    }

    if      (role == MessageRole) return p_data->at(index.row()).message;
    else if (role == SeverityRole) return p_data->at(index.row()).severity;

    return QVariant();
}

QHash<int, QByteArray> ModelLogs::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[MessageRole]     = "message";
    roles[SeverityRole]    = "severity";
    roles[DataRole]        = "data";
    return roles;
}

void ModelLogs::setPLog(QList<ENTITIES::Log> *pLog)
{
    p_data = pLog;

    beginResetModel();
    endResetModel();
}

void ModelLogs::setLog(QVariant v)
{
    if (v.userType() == qMetaTypeId<QList<ENTITIES::Log>*>()) {
        setPLog(v.value<QList<ENTITIES::Log>*>());
    }
}

void ModelLogs::showServerLogs()
{
    setPLog(p_serverLogs);
}
