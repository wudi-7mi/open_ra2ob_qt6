#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "./configmanager.h"
#include "./globalsetting.h"
#include "./ob.h"
#include "./tray.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ConfigManager *cfgm = nullptr);
    void detectShortcutStatus();
    void drawPreview(QWidget *widget);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *watched, QEvent *event) override;
    ConfigManager *_cfgm;
    Tray *tray;
    Ob *ob;
    Globalsetting *gls;
    QColorDialog *cd;

    bool forceHideOb = false;

private slots:
    void initLanguage(QString language);
    void onRbEnglishClicked();
    void onRbChineseClicked();
    void onBtnReloadClicked();
    void onOpacityChanged(int opacity);
    void onSidebarButtonClicked();
    void onSidebarResetButtonClicked();
    void onColorChanged(const QColor &color);
    void onBtnUpdatePlayerClicked();
    void updatePlayername();
    void showSetting();
    void quit();

protected:
    void hideEvent(QHideEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif  // MAINWINDOW_H
