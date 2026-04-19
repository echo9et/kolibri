#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "src/entities.h"

// @brief Класс для работы с сервером
// Слушает входящие подключения, обрабатывает полученные данные от клиентов в Storage 
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = Q_NULLPTR);
    ~Server();

    void setPort(quint16 port);

signals:
    void updateLog(ENTITIES::UUID, ENTITIES::Log);
    void newConnection(ENTITIES::UUID, ENTITIES::InfoConnection);
    void updateDeviceStatus(ENTITIES::UUID, ENTITIES::DeviceStatus);
    void updateNetworkMetrics(ENTITIES::UUID, ENTITIES::NetworkMetrics);
    void clientDisconected(ENTITIES::UUID);
    void sendServerLog(ENTITIES::Log);

    void finished();
    void errorListen();

public slots:
    void onListen();

private slots:
    void onNewConnection();

private:
    void incomingMesg(ENTITIES::UUID);
    void clientDisconect(ENTITIES::UUID);
    void clientErrors(ENTITIES::UUID);

    void handlerIncomingJson(ENTITIES::UUID, QJsonDocument &);

    QTcpServer *m_tcpServer = Q_NULLPTR;

    quint16 m_port = 0;
    QMap<ENTITIES::UUID, QTcpSocket*> m_clients;
};
