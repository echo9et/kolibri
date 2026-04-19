#include "client.h"

TcpClient::TcpClient(const QString &host, quint16 port, QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_host(host)
    , m_port(port)
{
    connect(m_socket, &QTcpSocket::connected,    this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead,    this, &TcpClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &TcpClient::onError);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
}

void TcpClient::onCheckConnected() {
    if (isConnected) {
        return;
    }

    qDebug() << "Попытка подключения к" << m_host << ":" << m_port;
    m_socket->connectToHost(m_host, m_port);
}

void TcpClient::onNeedSendMessage(QByteArray data)
{
    if (m_socket->state() != QTcpSocket::SocketState::ConnectedState) {
        return;
    }
    m_socket->write(data);
    m_socket->flush();
}

void TcpClient::onConnected() {
    isConnected = true;
}

void TcpClient::onReadyRead() {
    QByteArray data = m_socket->readAll();
    qDebug() << "Получено:" << data;
}

void TcpClient::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qWarning() << "Ошибка сокета:" << m_socket->errorString();
    isConnected = false;
}

void TcpClient::onDisconnected() {
    qDebug() << "Соединение закрыто.";
    isConnected = false;
}
