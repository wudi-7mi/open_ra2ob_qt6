#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QWidget>

#include "Ra2ob/Ra2ob"

namespace Ui {
class PlayerInfo;
}

class PlayerInfo : public QWidget {
    Q_OBJECT

public:
    explicit PlayerInfo(QWidget *parent = nullptr);
    ~PlayerInfo();

    void setLeftMode();
    void setRightMode();

    void setAll(int index);
    void setPlayerNameByIndex(int index);
    void setBalanceByIndex(int index);
    void setCreditByIndex(int index);
    void setCountryByIndex(int index);

    Ra2ob::Game *g;

private:
    Ui::PlayerInfo *ui;
};

#endif  // PLAYERINFO_H
