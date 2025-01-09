#pragma once

#include <QObject>
#include <QThread>

#include "quasyncserialport.h"
#include "quasyncserialportconfiguration.h"
#include "quasyncserialportwidget.h"

class DeviceController : public QObject {
    Q_OBJECT

    friend class MainWindow;

public:
    explicit DeviceController(QObject* parent = nullptr);

    QHash<QString, QByteArray> getData() const;

public slots:
    void onConnect();
    void onDisconnect();
    void onSaveProject(QString projectfile);
    void onLoadProject(QString projectfile);

public slots:
    void onInputCommand(QString type, QString cmd, QByteArray payload);
    void onRxPacket(const QByteArray data);

signals:
    void connectionEstablished(bool status);
    void txCommand(QByteArray payload);

private:
    void initialize();
    void makeConnections();

    void makeNODCommand(QString cmd, QByteArray payload);
    void makeTISCommand(QString cmd, QByteArray payload);

private:
    QHash<QString, QThread*> threads;

    QJsonDocument canDoc;
    QHash<int, QString> idMapper;
    QHash<QString, uint8_t> cmdCounterTx;
    QHash<QString, uint8_t> cmdCounterRx;

    QList<QSharedPointer<QUDevice>> devices;
    QSharedPointer<QUAsyncSerialPort> rotatorDevice;
    QSharedPointer<QUConfiguration> rotatorConfiguration;
    QSharedPointer<QUWidget> rotatorWidget;

private:
    QByteArray rxBuffer;
    QHash<QString, QByteArray> rxData;
};
