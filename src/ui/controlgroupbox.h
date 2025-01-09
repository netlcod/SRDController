#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QtWidgets>

#include "src/shareddata.h"
#include "src/ui/widgets/gridwidget.h"

class ControlGroupBox : public QGroupBox {
    Q_OBJECT

public:
    explicit ControlGroupBox(QWidget* parent = nullptr);
    ~ControlGroupBox();

public slots:
    void onUpdateUI(QString cmd, QByteArray payload);

signals:
    void sendCommand(QString type, QString cmd, QByteArray payload);

private:
    void createGridUI();
    void createStatusUI();
    void createControlUI();
    void makeConnections();

    int bitExtractor(int number, int p, int k);

private:
    QPointer<GridWidget> gridWidget;

    QPointer<QWidget> controlWidget;
    QPointer<QPushButton> moveLeftPushButton;
    QPointer<QPushButton> moveRightPushButton;
    QPointer<QPushButton> moveUpPushButton;
    QPointer<QPushButton> moveDownPushButton;
    QPointer<QDoubleSpinBox> yawVelocityDoubleSpinBox;
    QPointer<QDoubleSpinBox> pitchVelocityDoubleSpinBox;
    QPointer<QDoubleSpinBox> yawActualSpeedDoubleSpinBox;
    QPointer<QDoubleSpinBox> pitchActualSpeedDoubleSpinBox;
    QPointer<QDoubleSpinBox> yawSetPointDoubleSpinBox;
    QPointer<QDoubleSpinBox> pitchSetPointDoubleSpinBox;
    QPointer<QDoubleSpinBox> yawActualPositionDoubleSpinBox;
    QPointer<QDoubleSpinBox> pitchActualPositionDoubleSpinBox;
    QPointer<QDoubleSpinBox> yawStepDoubleSpinBox;
    QPointer<QDoubleSpinBox> pitchStepDoubleSpinBox;
    QPointer<QPushButton> stopPushButton;

    QPointer<QWidget> statusWidget;
    QPointer<QSpinBox> yawCurrentStatusTISSpinBox;
    QPointer<QSpinBox> yawPosTISSpinBox;
    QPointer<QSpinBox> yawSpeedTISSpinBox;
    QPointer<QComboBox> yawActuatorModeComboBox;
    QPointer<QComboBox> yawActualTargetReachedComboBox;
    QPointer<QComboBox> yawActualActuatorModeComboBox;
    QPointer<QComboBox> yawActualActuatorStateComboBox;
    QPointer<QSpinBox> pitchCurrentStatusTISSpinBox;
    QPointer<QSpinBox> pitchPosTISSpinBox;
    QPointer<QSpinBox> pitchSpeedTISSpinBox;
    QPointer<QComboBox> pitchActuatorModeComboBox;
    QPointer<QComboBox> pitchActualTargetReachedComboBox;
    QPointer<QComboBox> pitchActualActuatorModeComboBox;
    QPointer<QComboBox> pitchActualActuatorStateComboBox;
    QPointer<QPushButton> rememberPositionPushButton;
    QPointer<QPushButton> restorePositionPushButton;

    QPointF lastPos;
};
