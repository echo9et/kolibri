#include "server.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>


Server::Server(QObject *parent)
    : QObject{parent}
    , m_tcpServer(new QTcpServer(this))
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{
    qDebug() << Q_FUNC_INFO;

    if (m_tcpServer->isListening()) {
        m_tcpServer->close();
    }

    emit finished();
}

void Server::setPort(quint16 port)
{
    m_port = port;
}

void Server::onListen()
{
    if (!m_tcpServer->listen(QHostAddress::Any, m_port)) {
        qWarning() << "Не удалось подключиться к порту:" << m_port;
        emit errorListen();
    }

    emit sendServerLog({"DEBUG", "server слушает 0.0.0.0:" + QString::number(m_port)});
}

void Server::onNewConnection()
{
    static ENTITIES::UUID counterUUID = 0;

    while (m_tcpServer->hasPendingConnections()) {
        auto id = counterUUID++;
        m_clients[id] = m_tcpServer->nextPendingConnection();
        auto socket = m_clients[id];

        connect(socket, &QTcpSocket::readyRead,
                this, [this, id](){incomingMesg(id);
        });

        connect(socket, &QTcpSocket::disconnected,
                this, [this, id](){ clientDisconected(id);
        });

        connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
                this, [this, id](){ clientErrors(id);
        });

        auto ip = socket->peerAddress().toString().split(':').last();
        emit newConnection(id, {ip, socket->peerPort(), true});
        emit sendServerLog({"INFO", "Подключился новый клиент uuid:" + QString::number(id) +
                                        " host:" + ip + ":" + QString::number(socket->peerPort())});

        socket->write(ENTITIES::Log("INFO", "назначен uuid:" + QString::number(id)).serializeToJson());
    }
}

// Обработка входящих сообщений от клиентов
// ToDo - могут приходить несколько json сообщений в одном пакете и нет этой обработки
void Server::incomingMesg(ENTITIES::UUID uuid)
{
    QJsonParseError error;
    QByteArray data = m_clients[uuid]->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        emit sendServerLog({"ERROR",
                            "uuid:" + QString::number(uuid) + ", ошибка парсинга json:" + error.errorString()});
        qDebug() << Q_FUNC_INFO << "ошибка парсинга json:" << data;
        return;
    }
    handlerIncomingJson(uuid, doc);

}

void Server::clientDisconect(ENTITIES::UUID uuid)
{
    emit sendServerLog({"INFO", "Client " + QString::number(uuid) + " отключился"});

    if (m_clients.contains(uuid)) {
        auto socket = m_clients[uuid];
        m_clients.remove(uuid);
        socket->deleteLater();
    }

    emit clientDisconected(uuid);
}

void Server::clientErrors(ENTITIES::UUID uuid)
{
    emit sendServerLog({"ERROR", "Ошибка сети у клиента " + QString::number(uuid)});
}

void Server::handlerIncomingJson(ENTITIES::UUID uuid, QJsonDocument &doc)
{
    auto obj = doc.object();
    if (!obj.contains("type")) {
        emit sendServerLog({"WARRING", "отсуствует поле <type>"});
        return;
    }

    auto type = obj["type"].toString();

    if (type == ENTITIES::NetworkMetrics::name()) {
        emit updateNetworkMetrics(uuid, ENTITIES::NetworkMetrics (obj));
    } else if (type == ENTITIES::DeviceStatus::name()) {
        emit updateDeviceStatus(uuid, ENTITIES::DeviceStatus(obj));
    } else if (type == ENTITIES::Log::name()) {
        emit updateLog(uuid, ENTITIES::Log (obj));
    } else {
        emit sendServerLog({"WARRING", "пришел неизвестный тип сообщения:" + type});
    }
}


