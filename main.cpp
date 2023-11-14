#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QProcess>
#include <QString>

#include "ra2ob.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigManager *cfgm = new ConfigManager();
    cfgm->checkConfig();

    QTranslator translator;
    if (cfgm->getLanguage() == "zh_CN")
    {
        if (translator.load(":/i18n/open_ra2ob_qt6_zh_CN"))
        {
            a.installTranslator(&translator);
        }
    }

    MainWindow w(nullptr, cfgm);
    w.show();

    Ra2ob& g = Ra2ob::getInstance();
    g.startLoop(false);

    int ret = a.exec();

    if (ret == 773) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }

    return ret;
}
