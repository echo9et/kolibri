#pragma once

#include <QObject>
#include <QTimer>
#include "../../Server/src/entities.h"

class RandomMessage : public QObject
{
    Q_OBJECT
public:
    explicit RandomMessage(QObject *parent = nullptr);

    void startGenerate();

signals:
    void sendMessage(QByteArray);
private:
    struct Statistic {
        QString getString(){
            qDebug() << Q_FUNC_INFO << counterLogs;
            return "Счетчик отправленных сообщений журнала: " + QString::number(counterLogs)
                    +  ", метрик сети: " + QString::number(counterNetworkMetrics)
                    +  ", данных устройства: " + QString::number(counterDeviceStatus);
        }

        quint64 counterNetworkMetrics = 0;
        quint64 counterDeviceStatus = 0;
        quint64 counterLogs = 0;
    };

    void onTimeout();
    QByteArray generateMsgDeviceStatus();
    QByteArray generateMsgNetworkMetrics();


    quint64 m_startTime;
    QTimer *m_timerGenerateMessage;
    QTimer *m_timerSendStatistic;
    ENTITIES::NetworkMetrics m_networkMetrics;
    ENTITIES::DeviceStatus m_deviceStatus;
    Statistic m_stat;
};
