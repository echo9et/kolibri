#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "server/server.h"
#include "storage/storage.h"
#include "src/storage/tablemodelclients.h"

#include <QThread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Storage storage;

    qRegisterMetaType<ENTITIES::InfoConnection>();
    qRegisterMetaType<ENTITIES::NetworkMetrics>();
    qRegisterMetaType<ENTITIES::DeviceStatus>();
    qRegisterMetaType<ENTITIES::Log>();
    qRegisterMetaType<QList<ENTITIES::Log>>();


    Server *server = new Server;
    server->setPort(12345);

    QObject::connect(server, &Server::sendServerLog,        &storage, &Storage::onLog);
    QObject::connect(server, &Server::updateLog,            &storage, &Storage::onUpdateLog);
    QObject::connect(server, &Server::newConnection,        &storage, &Storage::onNewConnection);
    QObject::connect(server, &Server::clientDisconected,    &storage, &Storage::onClientDisconected);
    QObject::connect(server, &Server::updateDeviceStatus,   &storage, &Storage::onUpdateDeviceStatus);
    QObject::connect(server, &Server::updateNetworkMetrics, &storage, &Storage::onUpdateNetworkMetrics);

    {
        auto thread = new QThread;
        server->moveToThread(thread);
        QObject::connect(thread, &QThread::started, server, &Server::onListen);
        QObject::connect(server, &Server::finished, server, &QObject::deleteLater);
        QObject::connect(server, &Server::errorListen, &app, &QGuiApplication::deleteLater);
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

    engine.rootContext()->setContextProperty("clientsModel", storage.getModelClients());
    engine.rootContext()->setContextProperty("logsModel", storage.getModelLogs());
    engine.loadFromModule("Server", "Main");

    return QCoreApplication::exec();
}
