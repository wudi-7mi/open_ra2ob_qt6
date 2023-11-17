#include "./ob.h"
#include "./ui_ob.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <string>
#include <fstream>
#include <vector>

#include "./layoutsetting.h"

#include "./Ra2ob/Ra2ob"


using json = nlohmann::json;

Ob::Ob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ob)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);

    g = &Ra2ob::Game::getInstance();

    pi_1 = new PlayerInfo(this);
    pi_2 = new PlayerInfo(this);

    pi_1->setGeometry(layout::SC1K_TOP_M - 440 - 40, 0, 440, 100);
    pi_2->setGeometry(layout::SC1K_TOP_M + 40, 0, 440, 100);

    QTimer* detectGameTimer = new QTimer();
    detectGameTimer->setInterval(1000);
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(detectGame()));
    detectGameTimer->start();

}

Ob::~Ob()
{
    delete ui;
}

void Ob::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

    paintTopPanel(
        *painter,
        -150,
        0,
        topPanelWidth,
        topPanelHeight
    );
    paintRightPanel(*painter, rightOffset);
    // paintLeftPanel(*painter);

    painter->end();
}

void Ob::paintTopPanel(
    QPainter &painter,
    int offsetX,
    int offsetY,
    int pWidth,
    int pHeight
)
{
    painter.setOpacity(1);
    int pBottom = 5;
    int wCenter = (width() - layout::RIGHT_W) / 2 + offsetX;

    QString qs_l = QString::fromStdString("#" + qs_1);
    QString qs_r = QString::fromStdString("#" + qs_2);
    QColor lColor = QColor(qs_l);
    QColor rColor = QColor(qs_r);

    QLinearGradient leftGradient(QPointF(wCenter - pWidth / 2, pHeight * 4), QPointF(wCenter, 0));
    leftGradient.setColorAt(0, lColor);
    leftGradient.setColorAt(0.8, Qt::black);

    painter.fillRect(wCenter - pWidth / 2, 0, pWidth / 2, pHeight, leftGradient);

    QLinearGradient rightGradient(QPointF(wCenter, 0), QPointF(wCenter + pWidth / 2, pHeight * 4));
    rightGradient.setColorAt(0.2, Qt::black);
    rightGradient.setColorAt(1, rColor);

    painter.fillRect(wCenter, 0, pWidth / 2, pHeight, rightGradient);

    QLinearGradient bottomGradient(QPointF(wCenter - pWidth / 2 - 100, 0), QPointF(wCenter + pWidth / 2 + 100, 0));
    bottomGradient.setColorAt(0, lColor);
    bottomGradient.setColorAt(1, rColor);

    painter.fillRect(wCenter - pWidth / 2, pHeight, pWidth, pBottom, bottomGradient);

    painter.setOpacity(1);

    QPixmap pixmap;
    int iconWidth = 80;
    int iconX = wCenter - iconWidth / 2;
    int iconY = 0;

    pixmap.load((QString(":/icon/icon_100.png")));
    painter.drawPixmap(iconX, iconY, iconWidth, iconWidth, pixmap);

    return;
}


void Ob::paintRightPanel(
    QPainter &painter,
    int offsetX,
    int offsetY
)
{
    QColor fill(QColor("midnightblue"));
    painter.fillRect(QRect(
                         offsetX + layout::SC1K_RIGHT_X,
                         offsetY + 0,
                         layout::RIGHT_W,
                         layout::RIGHT_HEADER_H
                         ), fill);
    painter.fillRect(QRect(
                         offsetX + layout::SC1K_RIGHT_X,
                         offsetY + layout::RIGHT_HEADER_H,
                         layout::RIGHT_LEFTBORDER_W,
                         layout::RIGHT_BORDER_H
                         ), fill);
    painter.fillRect(QRect(
                         offsetX + layout::SC1K_RIGHT_X + layout::RIGHT_RIGHTBORDER_START,
                         offsetY + layout::RIGHT_HEADER_H,
                         layout::RIGHT_RIGHTBORDER_W,
                         layout::RIGHT_BORDER_H
                         ), fill);
    painter.fillRect(QRect(
                         offsetX + layout::SC1K_RIGHT_X,
                         offsetY + layout::RIGHT_HEADER_H + layout::RIGHT_BORDER_H,
                         layout::RIGHT_W,
                         layout::SC1K_RIGHT_BOTTOM_H
                         ), fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter.setPen(border);
    painter.drawRect(QRect(
        offsetX + layout::SC1K_MAP_X,
        offsetY + layout::SC1K_MAP_Y,
        layout::MAP_W,
        layout::MAP_H
        ));

    return;
}

void Ob::paintLeftPanel(QPainter &painter)
{
    return;
}

void Ob::rearrangeUnitblocks()
{
    Ra2ob::tagGameInfo gi = g->_gameInfo;

    if (!gi.valid) {
        return;
    }

    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        std::cerr << "No two valid players.\n";
        return;
    }

    std::vector<Ra2ob::tagPlayer> players;
    players.push_back(gi.players[validPlayerIndex[0]]);
    players.push_back(gi.players[validPlayerIndex[1]]);

    for (auto& it : unitblock_list_player_0) {
        delete it;
    }
    unitblock_list_player_0.clear();
    for (auto& it : unitblock_list_player_1) {
        delete it;
    }
    unitblock_list_player_1.clear();

    for (int i = 0; i < 2; i++) {

        Ra2ob::tagUnitsInfo ui = players[i].units;

        for (auto& u : ui.units) {
            if (u.num == 0) {
                continue;
            }

            Unitblock* ub = new Unitblock(this);

            ub->initUnit(QString::fromStdString(u.unitName), i);
            ub->updateNumber(u.num);

            if (i == 0) {
                unitblock_list_player_0.push_back(ub);
            } else {
                unitblock_list_player_1.push_back(ub);
            }
        }
    }

}

void Ob::sortUnitblocks() {
    for (int i = 0; i < unitblock_list_player_0.length(); i++) {
        Unitblock* ub = unitblock_list_player_0[i];
        ub->setGeometry(
            layout::SC1K_UNIT_X + rightOffset,
            layout::SC1K_UNIT_Y + i * layout::SC1K_UNIT_Hs,
            layout::SC1K_UNIT_W,
            layout::SC1K_UNIT_H);
        ub->setColor(qs_1);
        ub->show();
    }

    for (int i = 0; i < unitblock_list_player_1.length(); i++) {
        Unitblock* ub = unitblock_list_player_1[i];
        ub->setGeometry(
            layout::SC1K_UNIT_X + layout::SC1K_UNIT_W + rightOffset,
            layout::SC1K_UNIT_Y + i * layout::SC1K_UNIT_Hs,
            layout::SC1K_UNIT_W,
            layout::SC1K_UNIT_H);
        ub->setColor(qs_2);
        ub->show();
    }
}

void Ob::refreshPanel() {
    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        return;
    }

    int p1_index = validPlayerIndex[0];
    int p2_index = validPlayerIndex[1];

    pi_1->setAll(p1_index);
    pi_2->setAll(p2_index);

}

void Ob::setPlayerColor() {
    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        return;
    }
    int p1_index = validPlayerIndex[0];
    int p2_index = validPlayerIndex[1];

    Ra2ob::tagGameInfo gi = g->_gameInfo;

    qs_1 = gi.players[p1_index].panel.color;
    qs_2 = gi.players[p2_index].panel.color;
}

int Ob::getValidPlayerIndex(std::vector<int>* vpi)
{
    Ra2ob::tagGameInfo gi = g->_gameInfo;

    int validPlayerNum = 0;
    for (int i = 0; i < Ra2ob::MAXPLAYER; i++) {
        if (gi.players[i].valid) {
            validPlayerNum ++;
            vpi->push_back(i);
        }
    }

    return validPlayerNum;
}

void Ob::detectGame()
{
    if (g->_gameInfo.valid) {
        rearrangeUnitblocks();
        sortUnitblocks();
        refreshPanel();
        setPlayerColor();
        this->update();
    }
}

