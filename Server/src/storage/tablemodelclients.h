#pragma once

#include <QObject>
#include <QAbstractTableModel>

#include "src/entities.h"

// @brief Модель данных для отображения информации о клиентах в QML
// Хранит список клиентов и их данные, обновляет их при получении новых данных от сервера
class TableModelClients  : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
    };
public:
    explicit TableModelClients(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;


    void addClient(ENTITIES::UUID, const ENTITIES::InfoConnection &);
    void updateNetworkMetrics(ENTITIES::UUID, ENTITIES::NetworkMetrics &);
    void updateDeviceStatus(ENTITIES::UUID, ENTITIES::DeviceStatus &);
    void updateLogs(ENTITIES::UUID, ENTITIES::Log &);
    void clientDisconected(ENTITIES::UUID);    

    Q_INVOKABLE QList<ENTITIES::Log> *getLog(const QModelIndex&);

private:
    struct Data {
        static int countFiled() {return  9;};
        ENTITIES::UUID uuid;
        ENTITIES::Client client;
    };
    QVector<Data> m_data;
};
