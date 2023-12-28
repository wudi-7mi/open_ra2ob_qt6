#include <QApplication>
#include <QLocale>
#include <QProcess>
#include <QString>
#include <QTranslator>

#include "./globalsetting.h"
#include "./mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

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

    MainWindow w(nullptr, cfgm);
    w.show();

    Ra2ob::Game &g = Ra2ob::Game::getInstance();
    g.startLoop();

    Globalsetting &gls = Globalsetting::getInstance();

    int ret = a.exec();

    if (ret == 773) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }

    return ret;
}
