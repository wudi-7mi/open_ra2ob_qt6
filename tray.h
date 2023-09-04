#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QSystemTrayIcon>

class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent = nullptr);
    ~Tray();

    void setupTray();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

signals:
    void showSetting();
    void quit();

private:
    QSystemTrayIcon *m_trayicon;
};

#endif // TRAY_H
