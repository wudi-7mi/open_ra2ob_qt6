#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "configmanager.h"
#include "tray.h"

#include "ra2ob.hpp"

#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent, ConfigManager *cfgm)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icon/icon_highres.png"));
    this->setWindowTitle(tr("open_ra2ob - An Opensource Ra2 observer"));
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);

    _cfgm = cfgm;
    if (cfgm == nullptr)
    {
        _cfgm = new ConfigManager();
        _cfgm->checkConfig();
        qDebug() << "Abnormal cfgm rebuild.";
    }

    initLanguage(_cfgm->getLanguage());
    ui->btn_reload->hide();
    connect(ui->rb_English, &QRadioButton::toggled, this, &MainWindow::onRbEnglishClicked);
    connect(ui->rb_Chinese, &QRadioButton::toggled, this, &MainWindow::onRbChineseClicked);
    connect(ui->btn_reload, &QPushButton::clicked, this, &MainWindow::onBtnReloadClicked);

    tray = new Tray(this);
    connect(tray, SIGNAL(showSetting()), this, SLOT(showSetting()));
    connect(tray, SIGNAL(quit()), this, SLOT(quit()));

    tray->setupTray();

    ob = new Ob();

    connect(ui->btn_emit, &QPushButton::clicked, this, &MainWindow::onBtnEmitClicked);
    connect(ui->btn_show_ob, &QPushButton::clicked, this, &MainWindow::showOb);

    QTimer* detectGameTimer = new QTimer();
    detectGameTimer->setInterval(1000);
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(obToggle()));
    detectGameTimer->start();
}


void MainWindow::initLanguage(QString language)
{
    if (language == "zh_CN")
    {
        ui->rb_Chinese->setChecked(true);
        return;
    }
    ui->rb_English->setChecked(true);
}

void MainWindow::onRbEnglishClicked()
{
    ui->btn_reload->show();
    _cfgm->setLanguage("en_US");
}

void MainWindow::onRbChineseClicked()
{
    ui->btn_reload->show();
    _cfgm->setLanguage("zh_CN");
}

void MainWindow::onBtnReloadClicked()
{
    qApp->exit(773);
}

void MainWindow::onBtnEmitClicked()
{
    Ra2ob& g = Ra2ob::getInstance();
    std::cout << g._view.viewToJson().dump() << std::endl;
}

void MainWindow::obToggle()
{
    Ra2ob& g = Ra2ob::getInstance();

    if (g._view.m_gameValid) {
        ob->show();
        return;
    }

    ob->hide();
}

void MainWindow::showSetting()
{
    this->showNormal();
    this->activateWindow();
}

void MainWindow::showOb()
{
    ob->show();
}


void MainWindow::quit()
{
    qApp->quit();
}


void MainWindow::hideEvent(QHideEvent *event)
{
    this->hide();
    event->ignore();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}


MainWindow::~MainWindow()
{
    delete ui;
}

