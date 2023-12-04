#include "./configmanager.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include "./messagebox.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {
    _config_path = "./config/config.json";
}

bool ConfigManager::checkConfig() {
    QFile config_file(_config_path);

    if (config_file.exists()) {
        if (config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray config_data = config_file.readAll();
            config_file.close();

            QJsonDocument json_doc = QJsonDocument::fromJson(config_data);
            if (json_doc.isObject()) {
                _config_json = json_doc.object();
                return true;
            }
            MessageBox::errorQuit("Initialization", "Read config file error: Not a json file.");
            return false;
        }
        MessageBox::errorQuit("Initialization", "Failed to open config file for reading.");
        return false;
    }
    MessageBox::errorQuit("Initialization", "No such file: " + _config_path);
    return false;
}

bool ConfigManager::verifyConfig() { return true; }

/*
 * Must call ConfigManager::checkConfig() first!
 */
bool ConfigManager::setLanguage(QString language) {
    _config_json["Language"] = language;

    QJsonDocument new_json_doc(_config_json);
    QByteArray new_data = new_json_doc.toJson(QJsonDocument::Indented);

    QFile config_file(_config_path);

    if (!config_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        MessageBox::errorQuit("Config", "Failed to open config.json for writing.");
        return false;
    }

    config_file.write(new_data);
    config_file.close();
    return true;
}

QString ConfigManager::getLanguage() {
    if (_config_json.contains("Language")) {
        QJsonValue languageValue = _config_json["Language"];

        if (languageValue.isString()) {
            return languageValue.toString();
        }
    }

    return QString("zh_CN");
}
