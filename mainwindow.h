#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void changeEvent(QEvent *event);

private slots:
    void iconActived(QSystemTrayIcon::ActivationReason reason);
    void initLanguage();
    void onReloadButtonClicked();
    void onRadioButtonToggled();
    void showSetting();
    void quit();
};
#endif // MAINWINDOW_H
