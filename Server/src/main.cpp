#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "server/server.h"
#include "storage/storage.h"
#include "src/storage/tablemodelclients.h"

#include <QThread>

int main(int argc, char *argv[])
{
    qDebug() << "=== start app ===";
    QGuiApplication app(argc, argv);

    quint16 port = 12345; // Порт по умолчанию

    // Парсинг аргументов командной строки для порта
    if (argc == 3 && QString(argv[1]) == "--port") {
        bool ok;
        quint16 parsedPort = QString(argv[2]).toUShort(&ok);
        if (ok && parsedPort > 0) {
            port = parsedPort;
        } 
    }

    Storage storage;

    Server *server = new Server;
    server->setPort(port);

    // соединяем server signals к storage slots
    QObject::connect(server, &Server::sendServerLog,        &storage, &Storage::onLog);
    QObject::connect(server, &Server::updateLog,            &storage, &Storage::onUpdateLog);
    QObject::connect(server, &Server::newConnection,        &storage, &Storage::onNewConnection);
    QObject::connect(server, &Server::clientDisconected,    &storage, &Storage::onClientDisconected);
    QObject::connect(server, &Server::updateDeviceStatus,   &storage, &Storage::onUpdateDeviceStatus);
    QObject::connect(server, &Server::updateNetworkMetrics, &storage, &Storage::onUpdateNetworkMetrics);

    {
        // Запускаем сервер в отдельном потоке, чтобы не блокировать UI
        auto thread = new QThread;
        server->moveToThread(thread);
        QObject::connect(thread, &QThread::started, server, &Server::onListen);
        QObject::connect(server, &Server::finished, server, &QObject::deleteLater);
        QObject::connect(server, &Server::errorListen, &app, &QGuiApplication::deleteLater);
        // Убедимся, что сервер будет корректно удалён при завершении приложения
        QObject::connect(&app, &QGuiApplication::aboutToQuit, server, &QObject::deleteLater);

        thread->start();
    }


    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Передаем модели в QML
    engine.rootContext()->setContextProperty("clientsModel", storage.getModelClients());
    engine.rootContext()->setContextProperty("logsModel", storage.getModelLogs());
    engine.loadFromModule("Server", "Main");

    return QCoreApplication::exec();
}
