#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "tray.h"

#include <QMessageBox>
#include <QTranslator>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icon/icon_highres.png"));
    this->setWindowTitle(tr("open_ra2ob - An Opensource Ra2 observer"));
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    initLanguage();
    ui->btn_reload->hide();
    connect(ui->rb_English, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->btn_reload, &QPushButton::clicked, this, &MainWindow::onReloadButtonClicked);

    tray = new Tray(this);
    connect(tray, SIGNAL(showSetting()), this, SLOT(showSetting()));
    connect(tray, SIGNAL(quit()), this, SLOT(quit()));

    tray->setupTray();
}

void MainWindow::initLanguage()
{
    ui->rb_Chinese->setChecked(true);
}

void MainWindow::onRadioButtonToggled()
{
    ui->btn_reload->show();
}

void MainWindow::onReloadButtonClicked()
{
    QTranslator translator;

    qApp->removeTranslator(&translator);

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

MainWindow::~MainWindow()
{
    delete ui;
}

