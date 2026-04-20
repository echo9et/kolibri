#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "server/server.h"
#include "storage/storage.h"
#include "core.h"
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
    APP::Core appCore;

    Server *server = new Server;
    server->setPort(port);

    QObject::connect(&appCore, &APP::Core::stopToClient, server, &Server::onStopToClient);
    // соединяем server signals к storage slots
    QObject::connect(server, &Server::sendServerLog,        appCore.getStorage(), &Storage::onLog);
    QObject::connect(server, &Server::updateLog,            appCore.getStorage(), &Storage::onUpdateLog);
    QObject::connect(server, &Server::newConnection,        appCore.getStorage(), &Storage::onNewConnection);
    QObject::connect(server, &Server::clientDisconected,    appCore.getStorage(), &Storage::onClientDisconected);
    QObject::connect(server, &Server::updateDeviceStatus,   appCore.getStorage(), &Storage::onUpdateDeviceStatus);
    QObject::connect(server, &Server::updateNetworkMetrics, appCore.getStorage(), &Storage::onUpdateNetworkMetrics);

    {
        // Запускаем сервер в отдельном потоке, чтобы не блокировать UI
        auto thread = new QThread;
        server->moveToThread(thread);
        QObject::connect(thread, &QThread::started, server, &Server::onListen);
        QObject::connect(server, &Server::finished, server, &QObject::deleteLater);
        // При неудачной попытке подключиться к порту - закроем приложение
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
    engine.rootContext()->setContextProperty("clientsModel", appCore.getModelClients());
    engine.rootContext()->setContextProperty("logsModel", appCore.getModelLogs());
    engine.rootContext()->setContextProperty("appCore", &appCore);
    engine.loadFromModule("Server", "Main");

    return QCoreApplication::exec();
}
