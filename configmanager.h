#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QString>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

    QJsonObject _config_json;
    QString _config_path;

    bool checkConfig();
    bool verifyConfig();
    bool setLanguage(QString language);
    QString getLanguage();

signals:

public slots:
};

#endif // CONFIGMANAGER_H
