#pragma once

#include "src/shareddata.h"
#include <QtWidgets>

class StatusGroupBox : public QGroupBox {
    Q_OBJECT

public:
    explicit StatusGroupBox(QWidget* parent = nullptr);
    ~StatusGroupBox();

public slots:
    void onStatusCommandProcess(QString cmd, QByteArray payload);

private:
    void createUI();

private:
    // QPointer<QLineEdit> yawActualPositionLineEdit;
    // QPointer<QLineEdit> yawActualSpeedLineEdit;
    // QPointer<QLineEdit> yawModeLineEdit;
    // QPointer<QLineEdit> yawCurrentStatusLineEdit;

    // QPointer<QLineEdit> pitchActualPositionLineEdit;
    // QPointer<QLineEdit> pitchActualSpeedLineEdit;
    // QPointer<QLineEdit> pitchModeLineEdit;
    // QPointer<QLineEdit> pitchCurrentStatusLineEdit;

private:
    QJsonDocument canDoc;
};
