#include "./playerinfo.h"

#include <QFile>
#include <QString>
#include <string>

#include "./ui_playerinfo.h"

PlayerInfo::PlayerInfo(QWidget* parent) : QWidget(parent), ui(new Ui::PlayerInfo) {
    ui->setupUi(this);

    g = &Ra2ob::Game::getInstance();
    ui->lb_playerName->setStyleSheet("color: #ffffff;");
    ui->lb_balance->setStyleSheet("color: #ffffff;");
    ui->lb_credit->setStyleSheet("color: #ffffff;");
}

PlayerInfo::~PlayerInfo() { delete ui; }

void PlayerInfo::setMirror() {
    for (auto* child : this->findChildren<QWidget*>()) {
        child->setGeometry(this->width() - child->x() - child->width(), child->y(), child->width(),
                           child->height());
    }
    ui->lb_balance->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->lb_credit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mirrored = true;
}

void PlayerInfo::setAll(int index) {
    setPlayerNameByIndex(index);
    setBalanceByIndex(index);
    setCreditByIndex(index);
    setCountryByIndex(index);
    setPowerByIndex(index);
}

void PlayerInfo::setPlayerNameByIndex(int index) {
    std::string name = g->_gameInfo.players[index].panel.playerNameUtf;

    ui->lb_playerName->setText(QString::fromUtf8(name));
    ui->lb_playerName->adjustSize();

    int c_x = ui->lb_country->x();

    if (mirrored) {
        int c_w    = ui->lb_country->width();
        int mid    = (this->width() + c_x + c_w) / 2;
        int childX = mid - ui->lb_playerName->width() / 2;

        ui->lb_playerName->setGeometry(childX, ui->lb_playerName->y(), ui->lb_playerName->width(),
                                       ui->lb_playerName->height());
        return;
    }

    int mid    = c_x / 2;
    int childX = mid - ui->lb_playerName->width() / 2;

    ui->lb_playerName->setGeometry(childX, ui->lb_playerName->y(), ui->lb_playerName->width(),
                                   ui->lb_playerName->height());
}

void PlayerInfo::setBalanceByIndex(int index) {
    int num = g->_gameInfo.players[index].panel.balance;
    ui->lb_balance->setText(QString::number(num));
}

void PlayerInfo::setCreditByIndex(int index) {
    int num = g->_gameInfo.players[index].panel.creditSpent;
    ui->lb_credit->setText(QString::number(num));
}

void PlayerInfo::setCountryByIndex(int index) {
    std::string country = g->_gameInfo.players[index].panel.country;
    QString qCountry    = QString::fromStdString(country);
    QString img_str     = ":/countries/assets/countries/" + qCountry + ".png";

    QFile qf(img_str);

    if (!qf.exists()) {
        ui->lb_country->setPixmap(QPixmap(":/countries/assets/countries/Americans.png"));
        return;
    }

    ui->lb_country->setPixmap(QPixmap(img_str));
}

void PlayerInfo::setPowerByIndex(int index) {
    int powerDrain  = g->_gameInfo.players[index].panel.powerDrain;
    int powerOutput = g->_gameInfo.players[index].panel.powerOutput;

    if (powerDrain == 0 && powerOutput == 0) {
        ui->pb_power->setMaximum(1);
        ui->pb_power->setValue(1);
        ui->pb_power->setStyleSheet(
            "QProgressBar { background-color : grey; } QProgressBar::chunk { background-color: "
            "green; }");
        return;
    }

    ui->pb_power->setMaximum(powerOutput);
    ui->pb_power->setValue(powerDrain);
    if (powerOutput > powerDrain) {
        ui->pb_power->setStyleSheet(
            "QProgressBar { background-color : grey; } QProgressBar::chunk { background-color: "
            "green; }");
    } else {
        ui->pb_power->setValue(powerOutput);
        ui->pb_power->setStyleSheet(
            "QProgressBar { background-color : grey; } QProgressBar::chunk { background-color: "
            "red; }");
    }
}

int PlayerInfo::getInsufficientFund(int index) {
    int num = g->_gameInfo.players[index].panel.balance;
    if (num < 20) {
        return 0;
    }
    return 1;
}
