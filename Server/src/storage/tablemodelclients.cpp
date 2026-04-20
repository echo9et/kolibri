#include "tablemodelclients.h"

using namespace ENTITIES;

TableModelClients::TableModelClients(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TableModelClients::rowCount(const QModelIndex &) const
{
    return m_data.size();
}

int TableModelClients::columnCount(const QModelIndex &) const
{
    return Data::countFiled();
}

QVariant TableModelClients::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case TableDataRole: {
            auto row = m_data.at(index.row());
            switch (index.column()) {
                case 0:
                    return row.uuid;
                case 1:
                    return row.client.infoConnection.status;
                case 2:
                    return row.client.infoConnection.getHost();
                case 3:
                    return row.client.networkMetrics.getBandwidth();
                case 4:
                    return row.client.networkMetrics.getLatency();
                case 5:
                    return row.client.networkMetrics.getPacketLoss();
                case 6:
                    return row.client.deviceStatus.getUptime();
                case 7:
                    return row.client.deviceStatus.getCpuUsage();
                case 8:
                    return row.client.deviceStatus.getMemoryUsage();
            }
        }
        case UUIDRole:
            return m_data.at(index.row()).uuid;
        case IsOnlineRole:
            return m_data.at(index.row()).client.infoConnection.status;
    }
    return QVariant();
}

QVariant TableModelClients::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if      (orientation == Qt::Horizontal && section == 0) {return "UUID";}
    else if (orientation == Qt::Horizontal && section == 1) {return "Online";}
    else if (orientation == Qt::Horizontal && section == 2) {return "Host";}
    else if (orientation == Qt::Horizontal && section == 3) {return "Bandwidth";}
    else if (orientation == Qt::Horizontal && section == 4) {return "Latency"; }
    else if (orientation == Qt::Horizontal && section == 5) {return "Packet loss";}
    else if (orientation == Qt::Horizontal && section == 6) {return "Uptime";}
    else if (orientation == Qt::Horizontal && section == 7) {return "Cpu usage";}
    else if (orientation == Qt::Horizontal && section == 8) {return "Memory usage";}
    else                                                    {return "unknow";}

    if(orientation == Qt::Vertical){
        return QVariant::fromValue(section + 1) ;
    }
    return QVariant();
}

QHash<int, QByteArray> TableModelClients::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[TableDataRole]   = "tabledata";
    roles[UUIDRole]        = "uuid";
    roles[IsOnlineRole]      = "isOnline";
    return roles;
}

void TableModelClients::addClient(UUID uuid, const InfoConnection &client) {
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.push_back({uuid});
    m_data[uuid].client.infoConnection = qMove(client);
    endInsertRows();
}

void TableModelClients::updateNetworkMetrics(UUID uuid, NetworkMetrics &nm)
{
    if (m_data.size() > uuid) {
        m_data[uuid].client.networkMetrics = qMove(nm);
        emit dataChanged(createIndex(uuid, 3), createIndex(uuid, 5));
    }
}

void TableModelClients::updateDeviceStatus(UUID uuid, DeviceStatus &ds)
{
    if (m_data.size() > uuid) {
        m_data[uuid].client.deviceStatus = qMove(ds);
        emit dataChanged(createIndex(uuid, 6), createIndex(uuid, 8));
    }
}

void TableModelClients::updateLogs(UUID uuid, Log &log)
{
    if (m_data.size() > uuid) {
        m_data[uuid].client.logs.append(qMove(log));
    }
}

void TableModelClients::clientDisconected(UUID uuid)
{
    if (m_data.size() > uuid) {
        m_data[uuid].client.infoConnection.status = false;
        emit dataChanged(createIndex(uuid, 1), createIndex(uuid, 1));
    }
}

QList<ENTITIES::Log> *TableModelClients::getLog(ENTITIES::UUID uuid)
{
    if (uuid >= m_data.size()) {
        return Q_NULLPTR;
    }
    return &m_data[uuid].client.logs;
}

void TableModelClients::dicsonectedClient(ENTITIES::UUID uuid)
{
    emit dicsonectedTo(uuid);
}
