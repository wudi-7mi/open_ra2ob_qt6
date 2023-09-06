#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "configmanager.h"
#include "tray.h"

#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>

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


void MainWindow::RadioButtonToggled()
{
    ui->btn_reload->show();
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
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}


void MainWindow::showSetting()
{
    this->showNormal();
    this->activateWindow();
}


void MainWindow::quit()
{
    if (QMessageBox::question(this, tr("Notice"), tr("Do you want to Exit?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QApplication::exit(0);
    }
}


void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() != QEvent::WindowStateChange)
        return;
    if (this->windowState() == Qt::WindowMinimized)
    {
        this->hide();
    }
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

