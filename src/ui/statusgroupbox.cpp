#include "statusgroupbox.h"
#include "src/can.h"

StatusGroupBox::StatusGroupBox(QWidget* parent) : QGroupBox(parent) {
    createUI();

    QFile mapperFile(QString("can.json"));
    if (mapperFile.open(QFile::ReadOnly)) {
        canDoc = QJsonDocument::fromJson(mapperFile.readAll());
        mapperFile.close();
    } else {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Cannot find can.json!");
    }
}

StatusGroupBox::~StatusGroupBox() {}

void StatusGroupBox::onStatusCommandProcess(QString cmd, QByteArray payload) {
    QLineEdit* widget = this->findChild<QLineEdit*>(cmd);
    if (widget) {
        QJsonObject canObject = canDoc[cmd].toObject();
        QDataStream ds(payload);
#ifdef CAN_BIG_ENDIAN
        ds.setByteOrder(QDataStream::BigEndian);
#else
        ds.setByteOrder(QDataStream::LittleEndian);
#endif
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        switch (canObject["TYPE"].toInt()) {
            case can::PayloadType::ptFloat: {
                float value;
                ds >> value;
                widget->setText(QString::number(value));
            } break;
            case can::PayloadType::ptUchar: {
                unsigned char value;
                ds >> value;
                widget->setText(QString::number(value));
            } break;
        }
    }
}

void StatusGroupBox::createUI() {
    yawActualPositionLineEdit = new QLineEdit(this);
    yawActualPositionLineEdit->setObjectName("ptuYawPos");
    yawActualPositionLineEdit->setReadOnly(true);

    yawActualSpeedLineEdit = new QLineEdit(this);
    yawActualSpeedLineEdit->setObjectName("ptuYawSpeed");
    yawActualSpeedLineEdit->setReadOnly(true);

    yawEngineTempLineEdit = new QLineEdit(this);
    yawEngineTempLineEdit->setObjectName("ptuYawEngineTemp");
    yawEngineTempLineEdit->setReadOnly(true);

    yawModeLineEdit = new QLineEdit(this);
    yawModeLineEdit->setObjectName("ptuYawMode");
    yawModeLineEdit->setReadOnly(true);

    yawCurrentBusVoltageLineEdit = new QLineEdit(this);
    yawCurrentBusVoltageLineEdit->setObjectName("ptuYawCurrentBusVoltage");
    yawCurrentBusVoltageLineEdit->setReadOnly(true);

    yawActualCurrentLineEdit = new QLineEdit(this);
    yawActualCurrentLineEdit->setObjectName("ptuYawActualCurrent");
    yawActualCurrentLineEdit->setReadOnly(true);

    yawCurrentStatusLineEdit = new QLineEdit(this);
    yawCurrentStatusLineEdit->setObjectName("ptuYawCurrentStatus");
    yawCurrentStatusLineEdit->setReadOnly(true);

    QFormLayout* yawStateLayout = new QFormLayout;
    yawStateLayout->addRow(new QLabel("Actual position"), yawActualPositionLineEdit);
    yawStateLayout->addRow(new QLabel("Actual velocity"), yawActualSpeedLineEdit);
    yawStateLayout->addRow(new QLabel("Motor temperature"), yawEngineTempLineEdit);
    yawStateLayout->addRow(new QLabel("Current actuator mode"), yawModeLineEdit);
    yawStateLayout->addRow(new QLabel("Current bus voltage"), yawCurrentBusVoltageLineEdit);
    yawStateLayout->addRow(new QLabel("Actual current"), yawActualCurrentLineEdit);
    yawStateLayout->addRow(new QLabel("Current status"), yawCurrentStatusLineEdit);

    QGroupBox* yawStateGroupBox = new QGroupBox("Yaw", this);
    yawStateGroupBox->setLayout(yawStateLayout);

    pitchActualPositionLineEdit = new QLineEdit(this);
    pitchActualPositionLineEdit->setObjectName("ptuPitchPos");
    pitchActualPositionLineEdit->setReadOnly(true);

    pitchActualSpeedLineEdit = new QLineEdit(this);
    pitchActualSpeedLineEdit->setObjectName("ptuPitchSpeed");
    pitchActualSpeedLineEdit->setReadOnly(true);

    pitchEngineTempLineEdit = new QLineEdit(this);
    pitchEngineTempLineEdit->setObjectName("ptuPitchEngineTemp");
    pitchEngineTempLineEdit->setReadOnly(true);

    pitchModeLineEdit = new QLineEdit(this);
    pitchModeLineEdit->setObjectName("ptuPitchMode");
    pitchModeLineEdit->setReadOnly(true);

    pitchCurrentBusVoltageLineEdit = new QLineEdit(this);
    pitchCurrentBusVoltageLineEdit->setObjectName("ptuPitchCurrentBusVoltage");
    pitchCurrentBusVoltageLineEdit->setReadOnly(true);

    pitchActualCurrentLineEdit = new QLineEdit(this);
    pitchActualCurrentLineEdit->setObjectName("ptuPitchActualCurrent");
    pitchActualCurrentLineEdit->setReadOnly(true);

    pitchCurrentStatusLineEdit = new QLineEdit(this);
    pitchCurrentStatusLineEdit->setObjectName("ptuPitchCurrentStatus");
    pitchCurrentStatusLineEdit->setReadOnly(true);

    QFormLayout* pitchStateLayout = new QFormLayout;
    pitchStateLayout->addRow(new QLabel("Actual position"), pitchActualPositionLineEdit);
    pitchStateLayout->addRow(new QLabel("Actual velocity"), pitchActualSpeedLineEdit);
    pitchStateLayout->addRow(new QLabel("Motor temperature"), pitchEngineTempLineEdit);
    pitchStateLayout->addRow(new QLabel("Current actuator mode"), pitchModeLineEdit);
    pitchStateLayout->addRow(new QLabel("Current bus voltage"), pitchCurrentBusVoltageLineEdit);
    pitchStateLayout->addRow(new QLabel("Actual current"), pitchActualCurrentLineEdit);
    pitchStateLayout->addRow(new QLabel("Current status"), pitchCurrentStatusLineEdit);

    QGroupBox* pitchStateGroupBox = new QGroupBox("Pitch", this);
    pitchStateGroupBox->setLayout(pitchStateLayout);

    QHBoxLayout* statusLayout = new QHBoxLayout;
    statusLayout->addWidget(yawStateGroupBox);
    statusLayout->addWidget(pitchStateGroupBox);

    setTitle("Status");
    setLayout(statusLayout);
}
