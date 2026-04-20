#pragma once

#include <QObject>

#include "src/entities.h"
#include "src/storage/storage.h"

namespace APP {

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    const Storage *getStorage();
    TableModelClients *getModelClients();
    ModelLogs         *getModelLogs();

    Q_INVOKABLE void stopClient(ENTITIES::UUID);
signals:
    void stopToClient(ENTITIES::UUID);

private:
    Storage *p_storage = Q_NULLPTR;
};

}