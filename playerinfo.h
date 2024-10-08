#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QWidget>

#include "./globalsetting.h"
#include "Ra2ob/Ra2ob"

namespace Ui {
class PlayerInfo;
}

class PlayerInfo : public QWidget {
    Q_OBJECT

public:
    explicit PlayerInfo(QWidget *parent = nullptr);
    ~PlayerInfo();

    void setMirror();

    void setAll(int index);
    void setPlayerNameByIndex(int index);
    void setBalanceByIndex(int index);
    void setCountryByIndex(int index);
    void setPowerByIndex(int index);

    void rearrange();

    int getInsufficientFund(int index);

    Ra2ob::Game *g;
    Globalsetting *gls;
    bool mirrored = false;

private:
    Ui::PlayerInfo *ui;
};

#endif  // PLAYERINFO_H
