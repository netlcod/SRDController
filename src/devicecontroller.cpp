#include <QCoreApplication>
#include <QErrorMessage>

#include "devicecontroller.h"
#include "src/can.h"

DeviceController::DeviceController(QObject* parent) : QObject {parent} {
    initialize();
    makeConnections();
}

QHash<QString, QByteArray> DeviceController::getData() const {
    return rxData;
}

void DeviceController::onConnect() {
    foreach (auto& dev, devices) {
        if (!threads.contains(dev->uiid())) {
            threads[dev->uiid()] = new QThread;
            dev.get()->moveToThread(threads[dev->uiid()]);

            connect(dev.get(), &QUDevice::destroyed, threads[dev->uiid()], &QThread::quit);
            connect(threads[dev->uiid()], &QThread::finished, threads[dev->uiid()], &QThread::deleteLater);
        }
    }

    foreach (auto& dev, devices) {
        if (!threads[dev->uiid()]->isRunning()) {
            threads[dev->uiid()]->start();
        }
    }

    QMetaObject::invokeMethod(rotatorDevice.get(), &QUAsyncSerialPort::acquire, Qt::QueuedConnection);
}

void DeviceController::onDisconnect() {
    QMetaObject::invokeMethod(rotatorDevice.get(), &QUAsyncSerialPort::release, Qt::QueuedConnection);
}

void DeviceController::onSaveProject(QString projectfile) {
    QJsonObject json;
    foreach (QSharedPointer<QUDevice> dev, devices) {
        QJsonObject nodeObject;
        nodeObject.insert("name", dev->name());
        nodeObject.insert("uiid", dev->uiid());
        nodeObject.insert("cfg", dev->configuration()->json());
        json.insert(dev->name(), nodeObject);
    }

    QJsonDocument jsonDoc(json);
    QFile jsonFile(projectfile);
    if (jsonFile.open(QFile::WriteOnly)) {
        jsonFile.write(jsonDoc.toJson());
        jsonFile.close();
    }
}

void DeviceController::onLoadProject(QString projectfile) {
    QFile jsonFile(projectfile);
    if (jsonFile.open(QFile::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
        jsonFile.close();

        QJsonObject json = jsonDoc.object();
        foreach (QSharedPointer<QUDevice> dev, devices) {
            QJsonObject cfg = json[dev->name()].toObject();
            dev->configuration()->readFrom(cfg["cfg"].toObject());
        }
    }
}

void DeviceController::onInputCommand(QString type, QString cmd, QByteArray payload) {
    if (type == "NOD") {
        makeNODCommand(cmd, payload);
    } else if (type == "TIS") {
        makeTISCommand(cmd, payload);
    }
}

void DeviceController::makeNODCommand(QString cmd, QByteArray payload) {
    QJsonObject canObject = canDoc[cmd].toObject();
    cmdCounterTx[cmd]++;

    QByteArray can_frame;
    QDataStream ds(&can_frame, QDataStream::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

    switch (canObject["TYPE"].toInt()) {
        case can::PayloadType::ptFloat: {
            uint32_t id = canObject["CMD"].toInt();
            uint8_t dlc = 8;
            uint8_t node_id = can::MASTER_NODE_ID;
            uint8_t data_type = can::PayloadType::ptFloat;
            uint8_t service_code = 0;
            float value = payload.toDouble();

            ds << id;
            ds << dlc;
#ifdef CAN_BIG_ENDIAN
            ds.setByteOrder(QDataStream::BigEndian);
#else
            ds.setByteOrder(QDataStream::LittleEndian);
#endif
            ds << node_id;
            ds << data_type;
            ds << service_code;
            ds << cmdCounterTx[cmd];
            ds << value;
        } break;

        case can::PayloadType::ptUchar: {
            uint32_t id = canObject["CMD"].toInt();
            uint8_t dlc = 5;
            uint8_t node_id = can::MASTER_NODE_ID;
            uint8_t data_type = can::PayloadType::ptUchar;
            uint8_t service_code = 0;
            unsigned char value = payload.toUInt();

            ds << id;
            ds << dlc;
#ifdef CAN_BIG_ENDIAN
            ds.setByteOrder(QDataStream::BigEndian);
#else
            ds.setByteOrder(QDataStream::LittleEndian);
#endif
            ds << node_id;
            ds << data_type;
            ds << service_code;
            ds << cmdCounterTx[cmd];
            ds << value;
            ds << uint8_t(0);
            ds << uint8_t(0);
            ds << uint8_t(0);
        } break;

        default: {
            qDebug() << "Invalid TX CMD";
        } break;
    }

    emit txCommand(can_frame);
#ifdef QT_DEBUG
    qDebug() << "NOD: " << can_frame.toHex();
#endif
}

void DeviceController::makeTISCommand(QString cmd, QByteArray payload) {
    QByteArray can_frame;
    QDataStream ds(&can_frame, QDataStream::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

    uint32_t id = can::CAN_TIS_ID;
    uint8_t dlc = 8;
    uint8_t node_id = can::TURRET_NODE_ID;
    uint8_t data_type = can::PayloadType::ptShort2;
    uint8_t service_code = 5;
    uint8_t message_code = 0;
    uint16_t canid = idMapper.key(cmd);
    uint16_t rate = payload.toUShort();

    ds << id;
    ds << dlc;
#ifdef CAN_BIG_ENDIAN
    ds.setByteOrder(QDataStream::BigEndian);
#else
    ds.setByteOrder(QDataStream::LittleEndian);
#endif
    ds << node_id;
    ds << data_type;
    ds << service_code;
    ds << message_code;
    ds << canid;
    ds << rate;

    emit txCommand(can_frame);
#ifdef QT_DEBUG
    qDebug() << "TIS: " << can_frame.toHex();
#endif
}

void DeviceController::onRxPacket(const QByteArray data) {
    static int cnt = 0;

    rxBuffer.append(data);
    QByteArray cursor = rxBuffer;
    auto bytes = 0;

    uint8_t stx = 0xAA;
    uint8_t etx = 0xFF;
    uint8_t packet_size = 13; // или length
    while (cursor.contains(stx) && cursor.contains(etx)) {
        auto sIndex = cursor.indexOf(stx);
        if (sIndex != (cursor.size() - 1)) {
            uint8_t length = cursor[sIndex + 1];
            if (cursor.size() > sIndex + packet_size + 2) {
                qsizetype eIndex = sIndex + packet_size + 2;
                if (static_cast<uint8_t>(cursor[eIndex]) == etx) {
                    // CAN Processing
                    QByteArray can_frame = cursor.sliced(sIndex + 2,
                                                         length); // TODO packet_size вместо length
                    QDataStream ds(&can_frame, QDataStream::ReadOnly);
                    ds.setByteOrder(QDataStream::LittleEndian);
                    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

                    uint32_t id;
                    uint8_t dlc;
                    uint8_t node_id;
                    uint8_t data_type;
                    uint8_t service_code;
                    uint8_t message_code;

                    ds >> id;
                    ds >> dlc;
#ifdef CAN_BIG_ENDIAN
                    ds.setByteOrder(QDataStream::BigEndian);
#else
                    ds.setByteOrder(QDataStream::LittleEndian);
#endif
                    ds >> node_id;
                    ds >> data_type;
                    ds >> service_code;
                    ds >> message_code;

                    QString cmd = idMapper[id];
                    cmdCounterRx[cmd]++;
                    bool status = cmdCounterRx[cmd] == message_code ? true : false;
                    cmdCounterRx[cmd] = message_code;
                    QJsonObject canObject = canDoc[cmd].toObject();
                    switch (canObject["TYPE"].toInt()) {
                        case can::PayloadType::ptFloat: {
                            QByteArray payload(4, 0);
                            ds.readRawData(payload.data(), payload.size());
                            rxData[cmd] = payload;
                        } break;
                        case can::PayloadType::ptUchar: {
                            QByteArray payload(1, 0);
                            ds.readRawData(payload.data(), payload.size());
                            rxData[cmd] = payload;
                        } break;
                    }
                }
                cursor = cursor.mid(eIndex + 1);
                bytes += eIndex + 1;
            } else {
                break;
            }
        } else {
            cursor = cursor.mid(sIndex + 1);
            bytes += sIndex + 1;
        }
    }
    rxBuffer.remove(0, bytes);
}

void DeviceController::initialize() {
    QFile mapperFile(QString("can.json"));
    if (mapperFile.open(QFile::ReadOnly)) {
        canDoc = QJsonDocument::fromJson(mapperFile.readAll());
        mapperFile.close();

        QJsonObject canObj = canDoc.object();
        for (const auto& key : canObj.keys()) {
            QJsonObject cmd = canDoc[key].toObject();
            idMapper[cmd["CMD"].toInt()] = key;
        }
    } else {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Cannot find can.json!");
        errorMessage.exec();
    }

    // Rotator
    rotatorDevice = QSharedPointer<QUAsyncSerialPort>::create();
    rotatorConfiguration = QSharedPointer<QUAsyncSerialPortConfiguration>::create();
    rotatorWidget = QSharedPointer<QUAsyncSerialPortWidget>::create();
    rotatorDevice->setConfiguration(rotatorConfiguration);
    rotatorWidget->setConfiguration(rotatorConfiguration);

    connect(rotatorDevice.get(), &QUAsyncSerialPort::opened, this, &DeviceController::connectionEstablished, Qt::QueuedConnection);
    connect(
        rotatorDevice.get(),
        &QUAsyncSerialPort::closed,
        this,
        [this](bool status) { emit connectionEstablished(!status); },
        Qt::QueuedConnection);
    connect(
        this,
        &DeviceController::txCommand,
        qSharedPointerCast<QUAsyncSerialPort>(rotatorDevice).get(),
        &QUAsyncSerialPort::write,
        Qt::QueuedConnection);

    devices.append(rotatorDevice);
}

void DeviceController::makeConnections() {
    connect(
        qSharedPointerCast<QUAsyncSerialPort>(rotatorDevice).get(),
        &QUAsyncSerialPort::outputBytes,
        this,
        &DeviceController::onRxPacket,
        Qt::QueuedConnection);
}
