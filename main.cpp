#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QObject>
#include <QProcess>
#include <QSharedMemory>
#include <QString>
#include <QTranslator>

#include "./globalsetting.h"
#include "./hotkeymanager.h"
#include "./mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    NEFilter filter;
    a.installNativeEventFilter(&filter);

    HotkeyManager hm;
    hm.registerHotkey(filter);

    Globalsetting &gls = Globalsetting::getInstance();

    ConfigManager *cfgm = new ConfigManager();
    if (!cfgm->checkConfig()) {
        return 1;
    }

    QTranslator translator;
    if (cfgm->getLanguage() == "zh_CN") {
        if (translator.load(":/i18n/open_ra2ob_qt6_zh_CN")) {
            a.installTranslator(&translator);
        }
    }

    static QSharedMemory *singleApp = new QSharedMemory("SingleApp");
    if (!singleApp->create(1)) {
        QMessageBox::information(NULL, QObject::tr("tips"),
                                 QObject::tr("The program is already running"));
        qApp->quit();
        return -1;
    }

    gls.c.top_panel_opacity = cfgm->getOpacity();

    MainWindow w(nullptr, cfgm);
    w.show();

    Ra2ob::Game &g = Ra2ob::Game::getInstance();
    g.startLoop();

    int ret = a.exec();

    delete singleApp;
    a.removeNativeEventFilter(&filter);
    hm.releaseHotkey();

    if (ret == 773) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }

    return ret;
}
