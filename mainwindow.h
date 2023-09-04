#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tray.h"

#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Tray *tray;
    void changeEvent(QEvent *event);

private slots:
    void showSetting();
    void quit();
};

#endif // MAINWINDOW_H
