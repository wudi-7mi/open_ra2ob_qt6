#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icon/icon_highres.png"));
    this->setWindowTitle(tr("open_ra2ob - An Opensource Ra2 observer"));

    QSystemTrayIcon *m_trayicon = new QSystemTrayIcon(this);

    m_trayicon->setToolTip(QString("Open_Ra2ob"));

    m_trayicon->setIcon(QIcon(":/icon/icon_highres.png"));

    connect(m_trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActived(QSystemTrayIcon::ActivationReason)));

    m_trayicon->show();

    QMenu *m_traymenu = new QMenu();

    QAction *action_show_setting = new QAction(tr("Setting"), this);
    QAction *action_quit = new QAction(tr("Quit"), this);

    action_show_setting->setIcon(QIcon(":/icon/icon_highres.png"));

    m_traymenu->setAttribute(Qt::WA_TranslucentBackground);
    m_traymenu->setStyleSheet("QMenu{ border-radius: 3px; }");

    m_traymenu->addAction(action_show_setting);
    m_traymenu->addAction(action_quit);

    m_trayicon->setContextMenu(m_traymenu);

    connect(action_show_setting, SIGNAL(triggered()), this, SLOT(showSetting()));
    connect(action_quit, SIGNAL(triggered()), this, SLOT(quit()));

}

void MainWindow::iconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        this->activateWindow();
        break;
    default:
        break;
    }
    return;
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

