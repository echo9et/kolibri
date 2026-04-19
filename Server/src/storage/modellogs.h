#pragma once

#include <QObject>
#include <QAbstractListModel>
#include "src/entities.h"

class ModelLogs : public QAbstractListModel
{
    Q_OBJECT
    enum LogsRoles{
        DataRole = Qt::UserRole + 1,
        MessageRole,
        SeverityRole

    };
public:
    explicit ModelLogs(QObject *parent = nullptr);

    Q_INVOKABLE void setServerLog(QList<ENTITIES::Log> *);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setPLog(QList<ENTITIES::Log> *);
    Q_INVOKABLE void setLog(QVariant);
    Q_INVOKABLE void showServerLogs();

private:
    QList<ENTITIES::Log> *p_data = nullptr;
    QList<ENTITIES::Log> *p_serverLogs = nullptr;
};
