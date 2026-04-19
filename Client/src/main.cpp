#include <QCoreApplication>
#include <QString>
#include <QTimer>

#include "client.h"
#include "randommessage.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString host = "127.0.0.1";
    quint16 port = 12345;

    if (argc == 3) {
        host = QString::fromUtf8(argv[1]);

        bool ok = false;
        port = QString::fromUtf8(argv[2]).toUShort(&ok);
        if (!ok) {
            qWarning() << "Неверный номер порта";
            return 1;
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

