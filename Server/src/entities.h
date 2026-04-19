#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtGlobal>
#include <QMetaType>

namespace ENTITIES
{

using UUID = quint64;

template <typename T>
inline QString getValue(std::optional<T> o){
    return o.has_value() ? QString::number(o.value())
                         : "None";
}

struct NetworkMetrics {

    NetworkMetrics (){}
    NetworkMetrics (const QJsonObject &obj) { parseJson(obj); }
    static QString name() { return "NetworkMetrics"; }

    void parseJson(const QJsonObject &obj) {
        if (obj.contains("bandwidth")) {
            bandwidth = obj["bandwidth"].toDouble();
        }
        if (obj.contains("latency")) {
            latency = obj["latency"].toDouble();
        }
        if (obj.contains("packet_loss")) {
            packet_loss = obj["packet_loss"].toDouble();
        }
    }

    QByteArray serializeToJson() {
        QJsonObject obj;
        obj["type"] = name();
        if (latency.has_value())     {obj["latency"] = latency.value();}
        if (bandwidth.has_value())   {obj["bandwidth"] = bandwidth.value();}
        if (packet_loss.has_value()) {obj["packet_loss"] = packet_loss.value();}

        return QJsonDocument (obj).toJson(QJsonDocument::Compact);
    }

    std::optional<qreal> bandwidth;
    std::optional<qreal> latency;
    std::optional<qreal> packet_loss;

    QString getBandwidth() const {return getValue(bandwidth);}
    QString getLatency() const {return getValue(latency);}
    QString getPacketLoss() const {return getValue(packet_loss);}
};

struct DeviceStatus {
    DeviceStatus () {}
    DeviceStatus (const QJsonObject &obj){ parseJson(obj); }
    static QString name() { return "DeviceStatus"; }

    void parseJson(const QJsonObject &obj) {
        if (obj.contains("uptime")) {
            uptime = obj["uptime"].toDouble();
        }
        if (obj.contains("cpu_usage")) {
            cpu_usage = obj["cpu_usage"].toDouble();
        }
        if (obj.contains("memory_usage")) {
            memory_usage = obj["memory_usage"].toDouble();
        }
    }

    QByteArray serializeToJson() {
        QJsonObject obj;
        obj["type"] = name();
        if (uptime.has_value())       {obj["uptime"] = uptime.value();}
        if (cpu_usage.has_value())    {obj["cpu_usage"] = cpu_usage.value();}
        if (memory_usage.has_value()) {obj["memory_usage"] = memory_usage.value();}

        return QJsonDocument (obj).toJson(QJsonDocument::Compact);
    }

    std::optional<qreal> uptime;
    std::optional<qreal> cpu_usage;
    std::optional<qreal> memory_usage;

    QString getUptime() const {return getValue(uptime);}
    QString getCpuUsage() const {return getValue(cpu_usage);}
    QString getMemoryUsage() const {return getValue(memory_usage);}
};

struct Log {

public:
    Log (){}
    Log (QString severity, QString message)
        : severity(severity)
        , message(message)
    {
    }
    Log (const QJsonObject &obj) { parseJson(obj); }
    static QString name() { return "Log"; }

    void parseJson(const QJsonObject &obj) {
        if (obj.contains("message")) {
            message = obj["message"].toString();
        }
        if (obj.contains("severity")) {
            severity = obj["severity"].toString();
        }
    }

    QByteArray serializeToJson() {
        QJsonObject obj;
        obj["type"] = name();
        obj["message"] = message;
        obj["severity"] = severity;

        return QJsonDocument (obj).toJson(QJsonDocument::Compact);
    }

    QString severity;
    QString message;
};

struct InfoConnection {
    QString ip;
    quint16 port;
    bool status;

    QString getHost() { return ip + ':' + QString::number(port); }
};

struct Client {
    InfoConnection infoConnection;
    NetworkMetrics networkMetrics;
    DeviceStatus   deviceStatus;
    QList<Log>     logs;
};

}

Q_DECLARE_METATYPE(ENTITIES::InfoConnection)
Q_DECLARE_METATYPE(ENTITIES::Log)
Q_DECLARE_METATYPE(ENTITIES::Client)
Q_DECLARE_METATYPE(ENTITIES::DeviceStatus)
Q_DECLARE_METATYPE(ENTITIES::NetworkMetrics)
