#include "./configmanager.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include "./messagebox.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {
    _config_path = "./config/config.json";
    gls          = &Globalsetting::getInstance();
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

bool ConfigManager::writeConfig(const QJsonObject &jsonobj) {
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

/*
 * Must call ConfigManager::checkConfig() first!
 */
bool ConfigManager::setLanguage(QString language) {
    _config_json["Language"] = language;
    return writeConfig(_config_json);
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

bool ConfigManager::setOpacity(float opacity) {
    _config_json["Scoreboard Opacity"] = opacity;
    return writeConfig(_config_json);
}

float ConfigManager::getOpacity() {
    if (_config_json.contains("Scoreboard Opacity")) {
        QJsonValue opacityValue = _config_json["Scoreboard Opacity"];

        if (opacityValue.isDouble()) {
            return opacityValue.toDouble();
        }
    }

    return 0.0;
}

bool ConfigManager::setSidepanelColor(QColor qc) {
    _config_json["Sidepanel Color"] = qc.name();
    return writeConfig(_config_json);
}

QColor ConfigManager::getSidepanelColor() {
    if (_config_json.contains("Sidepanel Color")) {
        QJsonValue sidepanelColor = _config_json["Sidepanel Color"];

        if (sidepanelColor.isString()) {
            QColor c;

            c.setNamedColor(sidepanelColor.toString());
            return c;
        }
    }

    return QColor("midnightblue");
}

QJsonArray ConfigManager::getPlayernameList() {
    if (_config_json.contains("Players")) {
        QJsonValue player_json = _config_json["Players"];

        if (player_json.type() == QJsonValue::Array) {
            QJsonArray player_list = player_json.toArray();

            return player_list;
        }
    }
    return QJsonArray();
}

QString ConfigManager::findNameByNickname(QString nickname) {
    QJsonArray player_list = getPlayernameList();
    QString name;
    for (auto it : player_list) {
        if (it.type() == QJsonValue::Object) {
            QJsonObject p       = it.toObject();
            QString jnickname   = p.value("nickname").toString();
            QString jplayername = p.value("playername").toString();

            if (QString::compare(nickname, jnickname) == 0) {
                name = jplayername;
            }
        }
    }
    return name;
}

void ConfigManager::giveValueToGlobalsetting() { gls->player_list = getPlayernameList(); }

bool ConfigManager::insertPlayer(QString nickname, QString playername) {
    QJsonArray player_list = getPlayernameList();
    QJsonObject p;

    p.insert("nickname", nickname);
    p.insert("playername", playername);

    player_list.append(p);
    _config_json["Players"] = player_list;
    giveValueToGlobalsetting();
    return writeConfig(_config_json);
}

bool ConfigManager::overwritePlayer(QString nickname, QString playername) {
    QJsonArray player_list = getPlayernameList();
    QJsonObject p;

    p.insert("nickname", nickname);
    p.insert("playername", playername);

    for (int i = 0; i < player_list.size(); ++i) {
        QJsonObject obj   = player_list[i].toObject();
        QString jnickname = obj.value("nickname").toString();
        if (QString::compare(jnickname, nickname) == 0) {
            player_list.removeAt(i);
            player_list.insert(i, p);
            break;
        }
    }

    _config_json["Players"] = player_list;
    giveValueToGlobalsetting();
    return writeConfig(_config_json);
}

bool ConfigManager::updatePlayer(QString nickname, QString playername) {
    QString found = findNameByNickname(nickname);

    if (found.isEmpty()) {
        return insertPlayer(nickname, playername);
    }
    return overwritePlayer(nickname, playername);
}
