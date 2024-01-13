#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

    QJsonObject _config_json;
    QString _config_path;

    bool checkConfig();
    bool verifyConfig();
    bool writeConfig(const QJsonObject &jsonobj);
    bool setLanguage(QString language);
    QString getLanguage();
    bool setOpacity(float opacity);
    float getOpacity();
};

#endif  // CONFIGMANAGER_H
