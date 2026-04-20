#pragma once

#include <QObject>
#include <QAbstractListModel>
#include "src/entities.h"

// @brief Модель данных для отображения логов сервера в QML
// Хранит список логов сервера и обновляет его при получении новых данных от сервера
class ModelLogs : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModelLogs(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setPLog(QList<ENTITIES::Log> *);

    Q_INVOKABLE void setServerLog(QList<ENTITIES::Log> *);
    Q_INVOKABLE void setLog(QVariant);
    Q_INVOKABLE void showServerLogs();

private:
    enum LogsRoles{
        DataRole = Qt::UserRole + 1,
        MessageRole,
        SeverityRole
    };

    QList<ENTITIES::Log> *p_data = nullptr;
    QList<ENTITIES::Log> *p_serverLogs = nullptr;
};
