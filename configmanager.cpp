#include "configmanager.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    QString source_file_path = __FILE__;
    QFileInfo fileInfo(source_file_path);
    QString source_dir = fileInfo.absolutePath();

    qDebug() << "Source Code Directory: " << source_dir;

    QString file_name = "config.json";
    _config_path = QDir(source_dir).filePath(file_name);
}


bool ConfigManager::checkConfig()
{
    QFile config_file(_config_path);

    if (config_file.exists())
    {
        if (config_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QByteArray config_data = config_file.readAll();
            config_file.close();

            QJsonDocument json_doc = QJsonDocument::fromJson(config_data);
            if (json_doc.isObject())
            {
                _config_json = json_doc.object();
                qDebug() << "Reading config data: " << _config_json;
                return true;
            }
            qDebug() << "Config file error: Not a json file.";
            return false;
        }
        qDebug() << "Failed to open config file for reading.";
        return false;
    }
    qDebug() << "No such file: config.json";
    return false;
}


bool ConfigManager::verifyConfig()
{
    return true;
}


/*
 * Must call ConfigManager::checkConfig() first!
 */
bool ConfigManager::setLanguage(QString language)
{
    qDebug() << _config_json;
    _config_json["Language"] = language;

    QJsonDocument new_json_doc(_config_json);
    QByteArray new_data = new_json_doc.toJson(QJsonDocument::Indented);

    QFile config_file(_config_path);

    if (!config_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug() <<  "Failed to open config.json for writing.";
        return false;
    }

    config_file.write(new_data);
    qDebug() <<  "Write to config Successful.";
    config_file.close();
    return true;
}


QString ConfigManager::getLanguage()
{
    if (_config_json.contains("Language")) {
        QJsonValue languageValue = _config_json["Language"];

        if (languageValue.isString()) {
            return languageValue.toString();
        }
    }

    return QString("zh_CN");
}
