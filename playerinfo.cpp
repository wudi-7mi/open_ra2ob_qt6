#include "playerinfo.h"
#include "ui_playerinfo.h"

PlayerInfo::PlayerInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerInfo)
{
    ui->setupUi(this);

    g = &Ra2ob::Game::getInstance();
    ui->lb_playerName->setStyleSheet("color: #ffffff;");
    ui->lb_balance->setStyleSheet("color: #ffffff;");
    ui->lb_credit->setStyleSheet("color: #ffffff;");
}

PlayerInfo::~PlayerInfo()
{
    delete ui;
}

void PlayerInfo::setLeftMode() {
    return;
}

void PlayerInfo::setRightMode() {
    return;
}

void PlayerInfo::setAll(int index) {
    setPlayerNameByIndex(index);
    setBalanceByIndex(index);
    setCreditByIndex(index);
}

void PlayerInfo::setPlayerNameByIndex(int index) {
    std::string name = g->_gameInfo.players[index].panel.playerNameUtf;
    ui->lb_playerName->setText(QString::fromUtf8(name));
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

}
