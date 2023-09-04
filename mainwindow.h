#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tray.h"

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
    void changeEvent(QEvent *event) override;
    void RadioButtonToggled();

private slots:
    void initLanguage(QString language);
    void onRbEnglishClicked();
    void onRbChineseClicked();
    void onBtnReloadClicked();
    void showSetting();
    void quit();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
