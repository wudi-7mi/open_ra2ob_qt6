#include "./playerinfo.h"

#include <QFile>
#include <QString>
#include <string>

#include "./layoutsetting.h"
#include "./ui_playerinfo.h"

PlayerInfo::PlayerInfo(QWidget* parent) : QWidget(parent), ui(new Ui::PlayerInfo) {
    ui->setupUi(this);

    g   = &Ra2ob::Game::getInstance();
    gls = &Globalsetting::getInstance();

    ui->lb_playerName->setStyleSheet("color: #ffffff;");
    QFont f = QFont(layout::OPPO_B, 22);
    f.setBold(true);
    f.setStyleStrategy(QFont::PreferAntialias);
    ui->lb_playerName->setFont(f);

    ui->lb_balance->setStyleSheet("color: #ffffff;");
    f = QFont(layout::OPPO_M, 10);
    f.setStyleStrategy(QFont::PreferAntialias);
    ui->lb_balance->setFont(f);

    rearrange();
}

PlayerInfo::~PlayerInfo() { delete ui; }

void PlayerInfo::setMirror() {
    for (auto* child : this->findChildren<QWidget*>()) {
        child->setGeometry(this->width() - child->x() - child->width(), child->y(), child->width(),
                           child->height());
    }
    ui->lb_balance->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mirrored = true;
}

void PlayerInfo::setAll(int index) {
    setPlayerNameByIndex(index);
    setBalanceByIndex(index);
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
        ui->pb_power->setMaximum(1000);
        ui->pb_power->setValue(1);
        ui->pb_power->setStyleSheet(
            "QProgressBar { background-color : grey; } QProgressBar::chunk { background-color: "
            "green; }");
        return;
    }

    ui->pb_power->setMaximum(powerOutput);
    ui->pb_power->setValue(powerDrain);
    if (powerOutput > powerDrain && powerOutput * 0.85 < powerDrain) {
        ui->pb_power->setStyleSheet(
            "QProgressBar { background-color : grey; } QProgressBar::chunk { background-color: "
            "yellow; }");
    } else if (powerOutput > powerDrain) {
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

void PlayerInfo::rearrange() {
    ui->lb_playerName->setGeometry(0, gls->l.top_playername_y, gls->l.top_w / 2, gls->l.top_h);
    ui->lb_country->setGeometry(gls->l.top_country_x, gls->l.top_country_y, gls->l.top_country_w,
                                gls->l.top_country_h);
    ui->lb_img_balance->setGeometry(gls->l.top_i_x, gls->l.top_ibalance_y, gls->l.top_i_wh,
                                    gls->l.top_i_wh);
    ui->lb_img_power->setGeometry(gls->l.top_i_x, gls->l.top_ipower_y, gls->l.top_i_wh,
                                  gls->l.top_i_wh);
    ui->lb_balance->setGeometry(gls->l.top_balance_x, gls->l.top_balance_y, gls->l.top_balance_w,
                                gls->l.top_balance_h);
    ui->pb_power->setGeometry(gls->l.top_power_x, gls->l.top_power_y, gls->l.top_power_w,
                              gls->l.top_power_h);
}

int PlayerInfo::getInsufficientFund(int index) {
    int num = g->_gameInfo.players[index].panel.balance;
    if (num < 20) {
        return 0;
    }
    return 1;
}
