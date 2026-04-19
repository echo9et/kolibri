#include <QCoreApplication>
#include <QString>
#include <QTimer>

#include "client.h"
#include "randommessage.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString host = "127.0.0.1";
    quint16 port = 12345;

    if (argc > 1) {
        if (QString(argv[1]) == "--host" || argc > 3 && QString(argv[3]) == "--host") {
            auto numArgIp = QString(argv[1]) == "--host" ? 2 : 4;
            QString ip = QString(argv[numArgIp]);
            if (!QHostAddress (ip).isNull()) {
                host = ip;
            }
        }
        if (QString(argv[1]) == "--port" || argc > 3 && QString(argv[3]) == "--port") {
            auto numArgPort = QString(argv[1]) == "--port" ? 2 : 4;
            bool ok = false;
            auto parsedPort = QString::fromUtf8(argv[numArgPort]).toUShort(&ok);
            if (ok && parsedPort > 0) {
                port = parsedPort;
            }
        }
    }

    RandomMessage randomMessage;
    TcpClient client(host, port);
    QObject::connect(&randomMessage, &RandomMessage::sendMessage, &client, &TcpClient::onNeedSendMessage);

    QTimer *timer = new QTimer;
    timer->setInterval(5000);
    timer->start();
    QObject::connect(timer, &QTimer::timeout, &client, &TcpClient::onCheckConnected);
    randomMessage.startGenerate();
    return app.exec();
}

