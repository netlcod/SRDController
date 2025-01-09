#include "controlgroupbox.h"

ControlGroupBox::ControlGroupBox(QWidget* parent) : QGroupBox(parent) {
    createGridUI();
    createStatusUI();
    createControlUI();

    makeConnections();

    QPointer<QGridLayout> mainLayout = new QGridLayout;
    mainLayout->addWidget(statusWidget, 0, 0, 2, 1);
    mainLayout->addWidget(gridWidget, 0, 1);
    mainLayout->addWidget(controlWidget, 1, 1);

    setTitle("Rotator");
    setLayout(mainLayout);
}

ControlGroupBox::~ControlGroupBox() {}

void ControlGroupBox::onUpdateUI(QString cmd, QByteArray payload) {
    QDataStream ds(payload);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

    if (cmd == QString("ptuYawCurrentStatus")) {
        uchar value;
        ds >> value;
        yawActualTargetReachedComboBox->setCurrentIndex(bitExtractor(value, 0, 2));
        yawActualActuatorModeComboBox->setCurrentIndex(bitExtractor(value, 2, 2));
        yawActualActuatorStateComboBox->setCurrentIndex(bitExtractor(value, 7, 1));
    } else if (cmd == QString("ptuPitchCurrentStatus")) {
        uchar value;
        ds >> value;
        pitchActualTargetReachedComboBox->setCurrentIndex(bitExtractor(value, 0, 2));
        pitchActualActuatorModeComboBox->setCurrentIndex(bitExtractor(value, 2, 2));
        pitchActualActuatorStateComboBox->setCurrentIndex(bitExtractor(value, 7, 1));
    } else if (cmd == QString("ptuYawPos")) {
        float value;
        ds >> value;
        yawActualPositionDoubleSpinBox->setValue(value);

        QPointF pos = gridWidget->getItemPosition(QString("CurrentPositionItem"));
        pos.rx() = value;
        gridWidget->setItemPosition(QString("CurrentPositionItem"), pos);
    } else if (cmd == QString("ptuYawSpeed")) {
        float value;
        ds >> value;
        yawActualSpeedDoubleSpinBox->setValue(value);
    } else if (cmd == QString("ptuPitchPos")) {
        float value;
        ds >> value;
        pitchActualPositionDoubleSpinBox->setValue(value);

        QPointF pos = gridWidget->getItemPosition(QString("CurrentPositionItem"));
        pos.ry() = value;
        gridWidget->setItemPosition(QString("CurrentPositionItem"), pos);
    } else if (cmd == QString("ptuPitchSpeed")) {
        float value;
        ds >> value;
        pitchActualSpeedDoubleSpinBox->setValue(value);
    }
}

void ControlGroupBox::createGridUI() {
    gridWidget = new GridWidget(this);
    gridWidget->setRowCount(SharedData::yawRange);
    gridWidget->setColumnCount(SharedData::pitchRange);
    gridWidget->setGridStep(10);
    gridWidget->setGridBoldStep(100);
    gridWidget->addItem(GridWidget::ItemType::Rect, QString("CurrentPositionItem"), QBrush(Qt::green), QRectF(-1 * 10, -1 * 10, 20, 20));
    gridWidget->addItem(
        GridWidget::ItemType::MovableRect,
        QString("RequiredPositionItem"),
        QBrush(Qt::gray),
        QRectF(-0.5 * 10, -0.5 * 10, 10, 10));
}

void ControlGroupBox::createStatusUI() {
    statusWidget = new QWidget(this);

    yawCurrentStatusTISSpinBox = new QSpinBox(this);
    yawCurrentStatusTISSpinBox->setKeyboardTracking(false);
    yawCurrentStatusTISSpinBox->setRange(0, 1000);
    yawCurrentStatusTISSpinBox->clear();
    yawPosTISSpinBox = new QSpinBox(this);
    yawPosTISSpinBox->setKeyboardTracking(false);
    yawPosTISSpinBox->setRange(0, 1000);
    yawPosTISSpinBox->clear();
    yawSpeedTISSpinBox = new QSpinBox(this);
    yawSpeedTISSpinBox->setKeyboardTracking(false);
    yawSpeedTISSpinBox->setRange(0, 1000);
    yawSpeedTISSpinBox->clear();
    yawActuatorModeComboBox = new QComboBox(this);
    yawActuatorModeComboBox->addItem(QString("Free Run"), 0);
    yawActuatorModeComboBox->addItem(QString("Without Brake"), 1);
    yawActuatorModeComboBox->addItem(QString("Auto Brake"), 2);
    yawActualTargetReachedComboBox = new QComboBox(this);
    yawActualTargetReachedComboBox->addItem(QString("Not reached"), 0);
    yawActualTargetReachedComboBox->addItem(QString("Reached"), 1);
    yawActualTargetReachedComboBox->addItem(QString("Jog"), 2);
    yawActualActuatorModeComboBox = new QComboBox(this);
    yawActualActuatorModeComboBox->addItem(QString("Free Run"), 0);
    yawActualActuatorModeComboBox->addItem(QString("Without Brake"), 1);
    yawActualActuatorModeComboBox->addItem(QString("Auto Brake"), 2);
    yawActualActuatorStateComboBox = new QComboBox(this);
    yawActualActuatorStateComboBox->addItem(QString("Disabled"), 0);
    yawActualActuatorStateComboBox->addItem(QString("Enabled"), 1);

    pitchCurrentStatusTISSpinBox = new QSpinBox(this);
    pitchCurrentStatusTISSpinBox->setKeyboardTracking(false);
    pitchCurrentStatusTISSpinBox->setRange(0, 1000);
    pitchCurrentStatusTISSpinBox->clear();
    pitchPosTISSpinBox = new QSpinBox(this);
    pitchPosTISSpinBox->setKeyboardTracking(false);
    pitchPosTISSpinBox->setRange(0, 1000);
    pitchPosTISSpinBox->clear();
    pitchSpeedTISSpinBox = new QSpinBox(this);
    pitchSpeedTISSpinBox->setKeyboardTracking(false);
    pitchSpeedTISSpinBox->setRange(0, 1000);
    pitchSpeedTISSpinBox->clear();
    pitchActuatorModeComboBox = new QComboBox(this);
    pitchActuatorModeComboBox->addItem(QString("Free Run"), 0);
    pitchActuatorModeComboBox->addItem(QString("Without Brake"), 1);
    pitchActuatorModeComboBox->addItem(QString("Auto Brake"), 2);
    pitchActualTargetReachedComboBox = new QComboBox(this);
    pitchActualTargetReachedComboBox->addItem(QString("Not reached"), 0);
    pitchActualTargetReachedComboBox->addItem(QString("Reached"), 1);
    pitchActualTargetReachedComboBox->addItem(QString("Jog"), 2);
    pitchActualActuatorModeComboBox = new QComboBox(this);
    pitchActualActuatorModeComboBox->addItem(QString("Free Run"), 0);
    pitchActualActuatorModeComboBox->addItem(QString("Without Brake"), 1);
    pitchActualActuatorModeComboBox->addItem(QString("Auto Brake"), 2);
    pitchActualActuatorStateComboBox = new QComboBox(this);
    pitchActualActuatorStateComboBox->addItem(QString("Disabled"), 0);
    pitchActualActuatorStateComboBox->addItem(QString("Enabled"), 1);

    rememberPositionPushButton = new QPushButton("Remember Position", this);
    restorePositionPushButton = new QPushButton("Restore Position", this);

    QGroupBox* yawStatusGroupBox = new QGroupBox(QString("Yaw"), this);
    QPointer<QFormLayout> yawActualLayout = new QFormLayout;
    yawActualLayout->addRow(new QLabel("Yaw Current Status TIS"), yawCurrentStatusTISSpinBox);
    yawActualLayout->addRow(new QLabel("Yaw Positions TIS"), yawPosTISSpinBox);
    yawActualLayout->addRow(new QLabel("Yaw Speed TIS"), yawSpeedTISSpinBox);
    yawActualLayout->addRow(new QLabel("Set Yaw Actuator Mode"), yawActuatorModeComboBox);
    yawActualLayout->addRow(new QLabel("Yaw Target Reached"), yawActualTargetReachedComboBox);
    yawActualLayout->addRow(new QLabel("Yaw Actuator Mode"), yawActualActuatorModeComboBox);
    yawActualLayout->addRow(new QLabel("Yaw Actuator State"), yawActualActuatorStateComboBox);
    yawStatusGroupBox->setLayout(yawActualLayout);

    QGroupBox* pitchStatusGroupBox = new QGroupBox(QString("Pitch"), this);
    QPointer<QFormLayout> pitchActualLayout = new QFormLayout;
    pitchActualLayout->addRow(new QLabel("Pitch Current Status TIS"), pitchCurrentStatusTISSpinBox);
    pitchActualLayout->addRow(new QLabel("Pitch Positions TIS"), pitchPosTISSpinBox);
    pitchActualLayout->addRow(new QLabel("Pitch Speed TIS"), pitchSpeedTISSpinBox);
    pitchActualLayout->addRow(new QLabel("Set Pitch Actuator Mode"), pitchActuatorModeComboBox);
    pitchActualLayout->addRow(new QLabel("Pitch Target Reached"), pitchActualTargetReachedComboBox);
    pitchActualLayout->addRow(new QLabel("Pitch Actuator Mode"), pitchActualActuatorModeComboBox);
    pitchActualLayout->addRow(new QLabel("Pitch Actuator State"), pitchActualActuatorStateComboBox);
    pitchStatusGroupBox->setLayout(pitchActualLayout);

    QPointer<QGridLayout> statusLayout = new QGridLayout;
    statusLayout->addWidget(yawStatusGroupBox, 0, 0, 1, 2);
    statusLayout->addWidget(pitchStatusGroupBox, 1, 0, 1, 2);
    statusLayout->addWidget(rememberPositionPushButton, 2, 0);
    statusLayout->addWidget(restorePositionPushButton, 2, 1);

    statusWidget->setLayout(statusLayout);
}

void ControlGroupBox::createControlUI() {
    controlWidget = new QWidget(this);

    moveLeftPushButton = new QPushButton("←", this);
    moveRightPushButton = new QPushButton("→", this);
    moveUpPushButton = new QPushButton("↑", this);
    moveDownPushButton = new QPushButton("↓", this);

    yawVelocityDoubleSpinBox = new QDoubleSpinBox(this);
    yawVelocityDoubleSpinBox->setKeyboardTracking(false);
    yawVelocityDoubleSpinBox->setRange(-120, 120);
    yawVelocityDoubleSpinBox->setDecimals(2);
    yawVelocityDoubleSpinBox->setSingleStep(0.1);
    yawVelocityDoubleSpinBox->setPrefix("Yaw Speed: ");
    yawVelocityDoubleSpinBox->setSuffix(" deg/s");
    pitchVelocityDoubleSpinBox = new QDoubleSpinBox(this);
    pitchVelocityDoubleSpinBox->setKeyboardTracking(false);
    pitchVelocityDoubleSpinBox->setRange(-120, 120);
    pitchVelocityDoubleSpinBox->setDecimals(2);
    pitchVelocityDoubleSpinBox->setSingleStep(0.1);
    pitchVelocityDoubleSpinBox->setPrefix("Pitch Speed: ");
    pitchVelocityDoubleSpinBox->setSuffix(" deg/s");

    yawActualSpeedDoubleSpinBox = new QDoubleSpinBox(this);
    yawActualSpeedDoubleSpinBox->setReadOnly(true);
    yawActualSpeedDoubleSpinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    yawActualSpeedDoubleSpinBox->setRange(-120, 120);
    yawActualSpeedDoubleSpinBox->setDecimals(2);
    yawActualSpeedDoubleSpinBox->setSingleStep(0.01);
    yawActualSpeedDoubleSpinBox->setPrefix("Actual Yaw Speed: ");
    yawActualSpeedDoubleSpinBox->setSuffix(" deg/s");
    pitchActualSpeedDoubleSpinBox = new QDoubleSpinBox(this);
    pitchActualSpeedDoubleSpinBox->setReadOnly(true);
    pitchActualSpeedDoubleSpinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    pitchActualSpeedDoubleSpinBox->setRange(-120, 120);
    pitchActualSpeedDoubleSpinBox->setDecimals(2);
    pitchActualSpeedDoubleSpinBox->setSingleStep(0.01);
    pitchActualSpeedDoubleSpinBox->setPrefix("Actual Pitch Speed: ");
    pitchActualSpeedDoubleSpinBox->setSuffix(" deg/s");

    yawSetPointDoubleSpinBox = new QDoubleSpinBox(this);
    yawSetPointDoubleSpinBox->setKeyboardTracking(false);
    yawSetPointDoubleSpinBox->setRange(-SharedData::yawRange / 2, SharedData::yawRange / 2);
    yawSetPointDoubleSpinBox->setDecimals(2);
    yawSetPointDoubleSpinBox->setSingleStep(1);
    yawSetPointDoubleSpinBox->setPrefix("Yaw Set-Point: ");
    yawSetPointDoubleSpinBox->setSuffix(" deg");
    yawStepDoubleSpinBox = new QDoubleSpinBox(this);
    yawStepDoubleSpinBox->setKeyboardTracking(false);
    yawStepDoubleSpinBox->setDecimals(2);
    yawStepDoubleSpinBox->setSingleStep(1);
    yawStepDoubleSpinBox->setPrefix("Yaw Step: ");
    yawStepDoubleSpinBox->setSuffix(" deg");
    pitchSetPointDoubleSpinBox = new QDoubleSpinBox(this);
    pitchSetPointDoubleSpinBox->setKeyboardTracking(false);
    pitchSetPointDoubleSpinBox->setRange(-SharedData::pitchRange / 2, SharedData::pitchRange / 2);
    pitchSetPointDoubleSpinBox->setDecimals(2);
    pitchSetPointDoubleSpinBox->setSingleStep(1);
    pitchSetPointDoubleSpinBox->setPrefix("Pitch Set-Point: ");
    pitchSetPointDoubleSpinBox->setSuffix(" deg");
    pitchStepDoubleSpinBox = new QDoubleSpinBox(this);
    pitchStepDoubleSpinBox->setKeyboardTracking(false);
    pitchStepDoubleSpinBox->setDecimals(2);
    pitchStepDoubleSpinBox->setSingleStep(1);
    pitchStepDoubleSpinBox->setPrefix("Pitch Step: ");
    pitchStepDoubleSpinBox->setSuffix(" deg");

    yawActualPositionDoubleSpinBox = new QDoubleSpinBox(this);
    yawActualPositionDoubleSpinBox->setReadOnly(true);
    yawActualPositionDoubleSpinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    yawActualPositionDoubleSpinBox->setRange(-SharedData::yawRange / 2, SharedData::yawRange / 2);
    yawActualPositionDoubleSpinBox->setDecimals(2);
    yawActualPositionDoubleSpinBox->setSingleStep(1);
    yawActualPositionDoubleSpinBox->setPrefix("Actual Yaw Position: ");
    yawActualPositionDoubleSpinBox->setSuffix(" deg");
    pitchActualPositionDoubleSpinBox = new QDoubleSpinBox(this);
    pitchActualPositionDoubleSpinBox->setReadOnly(true);
    pitchActualPositionDoubleSpinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    pitchActualPositionDoubleSpinBox->setRange(-SharedData::pitchRange / 2, SharedData::pitchRange / 2);
    pitchActualPositionDoubleSpinBox->setDecimals(2);
    pitchActualPositionDoubleSpinBox->setSingleStep(1);
    pitchActualPositionDoubleSpinBox->setPrefix("Actual Pitch Position: ");
    pitchActualPositionDoubleSpinBox->setSuffix(" deg");

    stopPushButton = new QPushButton("Stop", this);

    QPointer<QGridLayout> controlLayout = new QGridLayout;
    controlLayout->addWidget(yawVelocityDoubleSpinBox, 0, 0);
    controlLayout->addWidget(yawActualSpeedDoubleSpinBox, 0, 1);
    controlLayout->addWidget(stopPushButton, 0, 2);
    controlLayout->addWidget(pitchActualSpeedDoubleSpinBox, 0, 3);
    controlLayout->addWidget(pitchVelocityDoubleSpinBox, 0, 4);

    controlLayout->addWidget(yawSetPointDoubleSpinBox, 1, 0);
    controlLayout->addWidget(yawActualPositionDoubleSpinBox, 1, 1);
    controlLayout->addWidget(moveUpPushButton, 1, 2);
    controlLayout->addWidget(pitchActualPositionDoubleSpinBox, 1, 3);
    controlLayout->addWidget(pitchSetPointDoubleSpinBox, 1, 4);

    controlLayout->addWidget(yawStepDoubleSpinBox, 2, 0);
    controlLayout->addWidget(moveLeftPushButton, 2, 1);
    controlLayout->addWidget(moveDownPushButton, 2, 2);
    controlLayout->addWidget(moveRightPushButton, 2, 3);
    controlLayout->addWidget(pitchStepDoubleSpinBox, 2, 4);

    controlWidget->setLayout(controlLayout);
}

void ControlGroupBox::makeConnections() {
    // UI
    connect(yawStepDoubleSpinBox, &QDoubleSpinBox::valueChanged, yawSetPointDoubleSpinBox, &QDoubleSpinBox::setSingleStep);
    connect(pitchStepDoubleSpinBox, &QDoubleSpinBox::valueChanged, pitchSetPointDoubleSpinBox, &QDoubleSpinBox::setSingleStep);

    // YAW
    connect(yawVelocityDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        emit sendCommand(QString("NOD"), QString("ptuYawCmdSpeed"), QByteArray::number(value));
    });
    connect(yawSetPointDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        QPointF pos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        pos.setX(value);
        QSignalBlocker blocker(gridWidget);
        gridWidget->setItemPosition(QString("RequiredPositionItem"), pos);
        emit sendCommand(QString("NOD"), QString("ptuYawCmdPos"), QByteArray::number(value));
    });
    connect(moveLeftPushButton, &QPushButton::clicked, this, [this]() {
        double offset = -yawStepDoubleSpinBox->value();
        QPointF oldPos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        QPointF newPos = gridWidget->moveItem(QString("RequiredPositionItem"), QPointF(offset, 0));
        if (oldPos != newPos) {
            QSignalBlocker blocker(yawSetPointDoubleSpinBox);
            yawSetPointDoubleSpinBox->setValue(newPos.x());
        }
    });
    connect(moveRightPushButton, &QPushButton::clicked, this, [this]() {
        double offset = yawStepDoubleSpinBox->value();
        QPointF oldPos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        QPointF newPos = gridWidget->moveItem(QString("RequiredPositionItem"), QPointF(offset, 0));
        if (oldPos != newPos) {
            QSignalBlocker blocker(yawSetPointDoubleSpinBox);
            yawSetPointDoubleSpinBox->setValue(newPos.x());
        }
    });

    // PITCH
    connect(pitchVelocityDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        emit sendCommand(QString("NOD"), QString("ptuPitchCmdSpeed"), QByteArray::number(value));
    });
    connect(pitchSetPointDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        QPointF pos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        pos.setY(value);
        QSignalBlocker blocker(gridWidget);
        gridWidget->setItemPosition(QString("RequiredPositionItem"), pos);
        emit sendCommand(QString("NOD"), QString("ptuPitchCmdPos"), QByteArray::number(value));
    });
    connect(moveUpPushButton, &QPushButton::clicked, this, [this]() {
        double offset = -pitchStepDoubleSpinBox->value();
        QPointF oldPos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        QPointF newPos = gridWidget->moveItem(QString("RequiredPositionItem"), QPointF(0, offset));
        if (oldPos != newPos) {
            QSignalBlocker blocker(pitchSetPointDoubleSpinBox);
            pitchSetPointDoubleSpinBox->setValue(newPos.y());
        }
    });
    connect(moveDownPushButton, &QPushButton::clicked, this, [this]() {
        double offset = pitchStepDoubleSpinBox->value();
        QPointF oldPos = gridWidget->getItemPosition(QString("RequiredPositionItem"));
        QPointF newPos = gridWidget->moveItem(QString("RequiredPositionItem"), QPointF(0, offset));
        if (oldPos != newPos) {
            QSignalBlocker blocker(pitchSetPointDoubleSpinBox);
            pitchSetPointDoubleSpinBox->setValue(newPos.y());
        }
    });

    // POSITION
    connect(gridWidget, &GridWidget::itemPositionChanged, this, [this](QPointF pos) {
        if (pos.x() != lastPos.x()) {
            emit sendCommand(QString("NOD"), QString("ptuYawCmdPos"), QByteArray::number(pos.x()));
            lastPos.setX(pos.x());
        }
        if (pos.y() != lastPos.y()) {
            emit sendCommand(QString("NOD"), QString("ptuPitchCmdPos"), QByteArray::number(pos.y()));
            lastPos.setY(pos.y());
        }
    });

    // COMMAND
    connect(yawActuatorModeComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        emit sendCommand(QString("NOD"), QString("ptuYawCmdMode"), QByteArray::number(index));
    });
    connect(pitchActuatorModeComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        emit sendCommand(QString("NOD"), QString("ptuPitchCmdMode"), QByteArray::number(index));
    });
    connect(rememberPositionPushButton, &QPushButton::clicked, this, [this]() {
        uint8_t value = 0;
        value |= 0b00000001;
        emit sendCommand(QString("NOD"), QString("ptuRememberCurrentPos"), QByteArray::number(value));
    });
    connect(stopPushButton, &QPushButton::clicked, this, [this]() {
        uint8_t value = 0;
        value |= 0b00000001;
        value |= 0b00000010;
        emit sendCommand(QString("NOD"), QString("ptuStopCmd"), QByteArray::number(value));
    });
    connect(restorePositionPushButton, &QPushButton::clicked, this, [this]() {
        uint8_t value = 0;
        value |= 0b00000001;
        emit sendCommand(QString("NOD"), QString("ptuRestoreCurrentPos"), QByteArray::number(value));
    });

    // TIS
    connect(yawCurrentStatusTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuYawCurrentStatus"), QByteArray::number(value));
    });
    connect(yawPosTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuYawPos"), QByteArray::number(value));
    });
    connect(yawSpeedTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuYawSpeed"), QByteArray::number(value));
    });
    connect(pitchCurrentStatusTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuPitchCurrentStatus"), QByteArray::number(value));
    });
    connect(pitchPosTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuPitchPos"), QByteArray::number(value));
    });
    connect(pitchSpeedTISSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        emit sendCommand(QString("TIS"), QString("ptuPitchSpeed"), QByteArray::number(value));
    });
}

int ControlGroupBox::bitExtractor(int value, int p, int k) {
    return (((1 << k) - 1) & (value >> p));
}
