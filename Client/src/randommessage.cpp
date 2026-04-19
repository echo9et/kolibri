#include "randommessage.h"
#include <QDateTime>
#include <QRandomGenerator>

using namespace ENTITIES;

inline quint64 getRandomInt(quint64 l = 10, quint64 r = 100){
    return l + QRandomGenerator::global()->bounded(r);
};

inline qreal getRandomFloat(qreal l = 0, qreal r = 1){
    return l + QRandomGenerator::global()->generateDouble() * 1 * r;
};

RandomMessage::RandomMessage(QObject *parent)
    : QObject{parent}
    , m_startTime(QDateTime::currentSecsSinceEpoch())
    , m_timerGenerateMessage(new QTimer)
    , m_timerSendStatistic(new QTimer)
{
    m_timerSendStatistic->setInterval(5000);
    connect(m_timerSendStatistic, &QTimer::timeout, this, [this](){
        m_stat.counterLogs++;
        emit sendMessage(Log("INFO", m_stat.getString()).serializeToJson());
    });


    m_timerGenerateMessage->setInterval(getRandomInt());
    connect(m_timerGenerateMessage, &QTimer::timeout, this, &RandomMessage::onTimeout);
}

void RandomMessage::startGenerate()
{
    m_timerSendStatistic->start();
    m_timerGenerateMessage->start();
}

void RandomMessage::onTimeout()
{
    auto msg = getRandomInt(0,2);

    if (msg == 0) {
        emit sendMessage(generateMsgNetworkMetrics());
    } else {
        emit sendMessage(generateMsgDeviceStatus());
    }
}

QByteArray RandomMessage::generateMsgDeviceStatus()
{
    m_stat.counterDeviceStatus++;
    m_timerGenerateMessage->setInterval(getRandomInt());
    m_deviceStatus.uptime = QDateTime::currentSecsSinceEpoch() - m_startTime;
    m_deviceStatus.cpu_usage = getRandomFloat(0.01, 100);
    m_deviceStatus.memory_usage = getRandomFloat(0.01, 100);

    return m_deviceStatus.serializeToJson();
}

QByteArray RandomMessage::generateMsgNetworkMetrics()
{
    m_stat.counterNetworkMetrics++;
    m_networkMetrics.bandwidth = getRandomFloat(0001, 100);
    m_networkMetrics.latency = getRandomFloat(0.001, 100);
    m_networkMetrics.packet_loss = getRandomFloat(0.01, 0.05);

    return m_networkMetrics.serializeToJson();
}


