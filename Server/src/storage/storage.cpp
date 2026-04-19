#include "storage.h"
#include <QDebug>

Storage::Storage(QObject *parent)
    : QObject{parent}
{
    m_logs.setServerLog(&m_serverLogs);
}

void Storage::onUpdateLog(ENTITIES::UUID uuid, ENTITIES::Log log)
{
    m_modelClients.updateLogs(uuid, log);
}

void Storage::onNewConnection(ENTITIES::UUID uuid, ENTITIES::InfoConnection info)
{
    m_modelClients.addClient(uuid, info);
}

void Storage::onUpdateDeviceStatus(ENTITIES::UUID uuid, ENTITIES::DeviceStatus ds)
{
    m_modelClients.updateDeviceStatus(uuid, ds);
}

void Storage::onUpdateNetworkMetrics(ENTITIES::UUID uuid, ENTITIES::NetworkMetrics nm)
{
    m_modelClients.updateNetworkMetrics(uuid, nm);
}

void Storage::onClientDisconected(ENTITIES::UUID uuid)
{
    m_modelClients.clientDisconected(uuid);
}

void Storage::onLog(ENTITIES::Log log)
{
    m_serverLogs.append(qMove(log));
}






