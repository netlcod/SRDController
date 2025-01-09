#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    controller = QSharedPointer<DeviceController>::create(this);

    createToolBar();
    mainWidget = new QTabWidget(this);
    mainWidget->addTab(createSettingsTab(), tr("Settings"));
    mainWidget->addTab(createManualControlTab(), tr("Control"));

    makeConnections();

    setCentralWidget(mainWidget);
    setMinimumSize(1280, 720);

    uiTimer = QSharedPointer<QTimer>::create(this);
    connect(uiTimer.get(), &QTimer::timeout, this, &MainWindow::onTimerUI);
    uiTimer->setInterval(25);
    uiTimer->start();
}

MainWindow::~MainWindow() {}

void MainWindow::onTimerUI() {
    QHash<QString, QByteArray> data = controller->getData();

    emit updateUI(QString("ptuYawPos"), data["ptuYawPos"]);
    emit updateUI(QString("ptuYawSpeed"), data["ptuYawSpeed"]);
    emit updateUI(QString("ptuPitchPos"), data["ptuPitchPos"]);
    emit updateUI(QString("ptuPitchSpeed"), data["ptuPitchSpeed"]);
    emit updateUI(QString("ptuYawCurrentStatus"), data["ptuYawCurrentStatus"]);
    emit updateUI(QString("ptuPitchCurrentStatus"), data["ptuPitchCurrentStatus"]);
}

void MainWindow::saveProject() {
    QString projectfile = QFileDialog::getSaveFileName(this, tr("Сохранить файл проекта"), ".", tr("Проект (*.json)"));

    if (!projectfile.isEmpty()) {
        controller->onSaveProject(projectfile);
    }
}

void MainWindow::loadProject() {
    QString projectfile = QFileDialog::getOpenFileName(this, tr("Открыть файл проекта"), ".", tr("Проект (*.json)"));

    if (!projectfile.isEmpty()) {
        controller->onLoadProject(projectfile);
    }
}

void MainWindow::createToolBar() {
    QPointer<QActionGroup> agConfiguration = new QActionGroup(this);
    aSave = new QAction(QIcon(":/icons/save-96px.png"), tr("Сохранить"), agConfiguration);
    connect(aSave, &QAction::triggered, this, &MainWindow::saveProject);
    aLoad = new QAction(QIcon(":/icons/load-96px.png"), tr("Загрузить"), agConfiguration);
    connect(aLoad, &QAction::triggered, this, &MainWindow::loadProject);

    QPointer<QActionGroup> agConnection = new QActionGroup(this);
    aConnect = new QAction(QIcon(":/icons/connect-96px.png"), tr("Подключить устройства"), agConnection);
    connect(aConnect, &QAction::triggered, controller.get(), &DeviceController::onConnect);
    aDisconnect = new QAction(QIcon(":/icons/disconnect-96px.png"), tr("Отключить устройства"), agConnection);
    connect(aDisconnect, &QAction::triggered, controller.get(), &DeviceController::onDisconnect);

    QPointer<QToolBar> toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setEnabled(true);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    toolBar->addAction(aSave);
    toolBar->addAction(aLoad);
    toolBar->addAction(aConnect);
    toolBar->addAction(aDisconnect);
    toolBar->addSeparator();

    addToolBar(toolBar);
}

QPointer<QWidget> MainWindow::createSettingsTab() {
    QPointer<QTabWidget> targetConnectionTableWidget = new QTabWidget(this);
    targetConnectionTableWidget->addTab(controller->rotatorWidget.get(), tr("ОПУ"));

    return targetConnectionTableWidget;
}

QPointer<QWidget> MainWindow::createManualControlTab() {
    QPointer<QWidget> manualControlWidget = new QWidget(this);

    controlWidget = new ControlGroupBox(this);
    controlWidget->setEnabled(false);

    QPointer<QVBoxLayout> manualControlLayout = new QVBoxLayout;
    manualControlLayout->addWidget(controlWidget);

    manualControlWidget->setLayout(manualControlLayout);

    return manualControlWidget;
}

void MainWindow::makeConnections() {
    // UI
    connect(this, &MainWindow::updateUI, controlWidget, &ControlGroupBox::onUpdateUI);

    connect(controller.get(), &DeviceController::connectionEstablished, controlWidget, &QWidget::setEnabled);

    // DATA
    connect(controlWidget, &ControlGroupBox::sendCommand, controller.get(), &DeviceController::onInputCommand);
}
