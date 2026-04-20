#include "core.h"

using namespace APP;

Core::Core(QObject *parent)
    : QObject{parent}
    , p_storage(new Storage(this))
{}

const Storage *Core::getStorage() {return p_storage;}

TableModelClients *Core::getModelClients() {
    return p_storage->getModelClients();
}

ModelLogs *Core::getModelLogs() {
    return p_storage->getModelLogs();
}

void Core::stopClient(ENTITIES::UUID uuid)
{
    emit stopToClient(uuid);
}
