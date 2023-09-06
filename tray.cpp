#include "tray.h"

#include <QAction>
#include <QMenu>

Tray::Tray(QObject *parent)
    : QObject{parent}
{
    m_trayicon = new QSystemTrayIcon();

    connect(
        m_trayicon,
        SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this,
        SLOT(iconActivated(QSystemTrayIcon::ActivationReason))
    );
}

Tray::~Tray()
{
    delete m_trayicon;
}

void Tray::setupTray()
{
    // Setup trayicon.
    m_trayicon->setToolTip(tr("Open_Ra2ob"));
    m_trayicon->setIcon(QIcon(":/icon/icon_32.png"));
    m_trayicon->show();

    // Create and set traymenu.
    QMenu *m_traymenu = new QMenu();
    m_traymenu->setAttribute(Qt::WA_TranslucentBackground);
    m_traymenu->setStyleSheet("QMenu{ border-radius: 3px; }");

    // Define actions.
    QAction *action_show_setting = new QAction(tr("Setting"));
    QAction *action_quit = new QAction(tr("Quit"));

    action_show_setting->setIcon(QIcon(":/icon/icon_highres.png"));

    // Add actions to traymenu.
    m_traymenu->addAction(action_show_setting);
    m_traymenu->addAction(action_quit);

    // Bind traymenu to trayicon.
    m_trayicon->setContextMenu(m_traymenu);

    connect(action_show_setting, SIGNAL(triggered()), this, SIGNAL(showSetting()));
    connect(action_quit, SIGNAL(triggered()), this, SIGNAL(quit()));
}

void Tray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        emit showSetting();
        break;

    default:
        break;
    }
}
