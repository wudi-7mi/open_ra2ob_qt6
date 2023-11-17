#include "./mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QScreen>
#include <QProcess>
#include <QString>

#include "./Ra2ob/Ra2ob"

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

    Ra2ob::Game& g = Ra2ob::Game::getInstance();
    g.startLoop();

    int screenCount = QGuiApplication::screens().size();
    QList<QScreen*> screens = QGuiApplication::screens();

    for (auto& s : screens) {
        qDebug() << "Name: " << s->name();
        qDebug() << "Geometry: " << s->geometry();
        qDebug() << "Available Geometry " << s->availableGeometry();
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    qDebug() << "Primary Screen: " << screen->name();

    int ret = a.exec();

    if (ret == 773) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }

    return ret;
}
