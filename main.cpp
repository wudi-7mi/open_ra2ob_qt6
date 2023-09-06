#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include "ob.h"

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
//    w.show();

    Ob ob(nullptr);
    ob.show();

    return a.exec();
}
