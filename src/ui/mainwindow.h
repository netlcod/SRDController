#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>
#include <QSharedPointer>
#include <QThread>
#include <QtWidgets>

#include "src/devicecontroller.h"
#include "src/ui/controlgroupbox.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void updateUI(QString cmd, QByteArray payload);

private slots:
    void onTimerUI();
    void saveProject();
    void loadProject();

private:
    void createToolBar();
    QPointer<QWidget> createSettingsTab();
    QPointer<QWidget> createManualControlTab();
    void makeConnections();

private:
    QPointer<QMenu> fileMenu;
    QPointer<QAction> aSave;
    QPointer<QAction> aLoad;
    QPointer<QAction> aConnect;
    QPointer<QAction> aDisconnect;

    QPointer<QTabWidget> mainWidget;
    QPointer<ControlGroupBox> controlWidget;

private:
    QSharedPointer<QTimer> uiTimer;
    QSharedPointer<DeviceController> controller;
};

#endif // MAINWINDOW_H
