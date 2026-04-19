#pragma once

#include <QObject>

#include "src/entities.h"
#include "./tablemodelclients.h"
#include "./modellogs.h"

class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(QObject *parent = nullptr);
    TableModelClients *getModelClients() {return &m_modelClients;}
    ModelLogs         *getModelLogs()    {return &m_logs;}
public slots:
    void onUpdateLog(ENTITIES::UUID, ENTITIES::Log);
    void onNewConnection(ENTITIES::UUID, ENTITIES::InfoConnection);
    void onUpdateDeviceStatus(ENTITIES::UUID, ENTITIES::DeviceStatus);
    void onUpdateNetworkMetrics(ENTITIES::UUID, ENTITIES::NetworkMetrics);
    void onClientDisconected(ENTITIES::UUID);

    void onLog(ENTITIES::Log);

private:
    TableModelClients m_modelClients;
    ModelLogs m_logs;

    QList<ENTITIES::Log> m_serverLogs;
};
