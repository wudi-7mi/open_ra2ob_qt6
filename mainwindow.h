#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./tray.h"
#include "./ob.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "configmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, ConfigManager *cfgm = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ConfigManager *_cfgm;
    Tray *tray;
    Ob *ob;

    bool forceHideOb = false;

private slots:
    void initLanguage(QString language);
    void onRbEnglishClicked();
    void onRbChineseClicked();
    void onBtnReloadClicked();
    void showSetting();
    void quit();

protected:
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
