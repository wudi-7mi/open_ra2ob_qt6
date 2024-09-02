#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>

#include "./globalsetting.h"

class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);

    QJsonObject _config_json;
    QString _config_path;
    Globalsetting *gls;

    bool checkConfig();
    bool verifyConfig();
    bool writeConfig(const QJsonObject &jsonobj);
    bool setLanguage(QString language);
    QString getLanguage();
    bool setOpacity(float opacity);
    float getOpacity();
    bool setSidepanelColor(QColor qc);
    QColor getSidepanelColor();
    QString getBuildingqueuePosition();
    void givePositionToGlobalsetting();
    bool setBuildingqueuePosition(QString position);
    QJsonArray getPlayernameList();
    bool updatePlayer(QString nickname, QString playername);
    void giveValueToGlobalsetting();
    QString findNameByNickname(QString nickname);

private:
    bool insertPlayer(QString nickname, QString playername);
    bool overwritePlayer(QString nickname, QString playername);
};

#endif  // CONFIGMANAGER_H
