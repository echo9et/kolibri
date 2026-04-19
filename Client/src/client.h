#pragma once

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

#include <QTimer>


class TcpClient : public QObject {
    Q_OBJECT
public:
    TcpClient(const QString &host, quint16 port, QObject *parent = nullptr);
public slots:
    void onCheckConnected();
    void onNeedSendMessage(QByteArray);

private slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void onDisconnected();

private:
    QTcpSocket *m_socket;
    QString m_host;
    quint16 m_port;
    QTimer *timer;
    bool isConnected = false;
};
