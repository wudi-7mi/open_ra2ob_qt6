#include "ob.h"
#include "ui_ob.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <string>
#include <fstream>
#include <vector>

#include "ra2ob.hpp"


using json = nlohmann::json;

Ob::Ob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ob)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);

    g = &Ra2ob::getInstance();

    QLabel* lb_name_01 = new QLabel(this);
    QLabel* lb_name_02 = new QLabel(this);

    QTimer* detectGameTimer = new QTimer();
    detectGameTimer->setInterval(1000);
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(detectGame()));
    detectGameTimer->start();
//    qw->setGeometry(1763, 158, 78, 100 * 10);

}

Ob::~Ob()
{
    delete ui;
}

void Ob::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

//    paintTopPanel(
//        *painter,
//        QColor("#ffff00"),
//        QColor("#0000ff"),
//        -150,
//        0,
//        800,
//        80
//    );
    paintTopPanel(
        *painter,
        QColor("#ffff00"),
        QColor("#0000ff")
    );
    // paintRightPanel(*painter, -425);
    paintRightPanel(*painter);
    // paintLeftPanel(*painter);

    painter->end();
}

void Ob::paintTopPanel(
    QPainter &painter,
    QColor lColor,
    QColor rColor,
    int offsetX,
    int offsetY,
    int pWidth,
    int pHeight,
    int rightPanelWidth
)
{
    painter.setOpacity(0.8);
    int pBottom = 5;
    int wCenter = (width() - rightPanelWidth) / 2 + offsetX;

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
    int iconWidth = 100;
    int iconX = wCenter - iconWidth / 2;
    int iconY = 0;

    pixmap.load((QString(":/icon/icon_100.png")));
    painter.drawPixmap(iconX, iconY, iconWidth, iconWidth, pixmap);

    return;
}


void Ob::paintRightPanel(
    QPainter &painter,
    int offsetX,
    int offsetY,
    int rightPanelWidth,
    int rightPanelHeight
)
{
    QColor fill(QColor("midnightblue"));
    painter.fillRect(QRect(offsetX + 1752, offsetY + 0, rightPanelWidth, 48), fill);
    painter.fillRect(QRect(offsetX + 1752, offsetY + 48, 12, 110), fill);
    painter.fillRect(QRect(offsetX + 1905, offsetY + 48, 15, 110), fill);
    painter.fillRect(QRect(offsetX + 1752, offsetY + 158, rightPanelWidth, 922), fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter.setPen(border);
    painter.drawRect(QRect(offsetX + 1763, offsetY + 47, 142, 111));

    return;
}

void Ob::paintLeftPanel(QPainter &painter)
{
    return;
}

void Ob::refreshData()
{
    return;
}

void Ob::rearrangeUnitblocks()
{
    if (!g->_view.m_gameValid) {
        return;
    }

    json view = g->_view.viewToJsonFull();

    if (view["valid_players"].empty()) {
        return;
    }

    try {
        player_list = { view["valid_players"][0], view["valid_players"][1] };
    } catch(json::type_error err) {
        std::cerr << "No two valid players." << std::endl;
        return;
    }

    for (auto& ub : unitblock_list_player_0) {
        delete ub;
    }
    unitblock_list_player_0.clear();

    for (auto& ub : unitblock_list_player_1) {
        delete ub;
    }
    unitblock_list_player_1.clear();

    for (int i = 0; i < 2; i++) {
        auto unit_info = view["player_info"][i]["unit_info"];

        for (auto& u : unit_info.items()) {

            std::string v_str = u.value();

            int v = std::stoi(v_str);

            if (v <= 0) {
                continue;
            }

            Unitblock* ub = new Unitblock(this);

            ub->initUnit(QString::fromStdString(u.key()), i);

            updateView(*ub);

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
        ub->setGeometry(1757, 158 + i * 57, 78, 60);
        ub->setColor("#008787");
        ub->show();
    }

    for (int i = 0; i < unitblock_list_player_1.length(); i++) {
        Unitblock* ub = unitblock_list_player_1[i];
        ub->setGeometry(1757 + 78, 158 + i * 57, 78, 60);
        ub->setColor("#ff8080");
        ub->show();
    }
}

void Ob::setName(int index) {
    json panel_info = g->_view.getPlayerUnitInfo(index);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    std::cout << panel_info["Player Name"];

//    std::wstring wstr = convert.from_bytes(panel_info["Player Name"]);
//    QString player_name = QString::fromStdWString(wstr);

//    qDebug() << player_name;
}

void Ob::setCountry(int index) {

}

void Ob::setColor(int index) {

}

void Ob::updateView(Unitblock& ub)
{
    json unit_info = g->_view.getPlayerUnitInfo(ub.player_index);
    std::string n_std = unit_info[ub.unit_name.toStdString()];

    ub.updateNumber(QString::fromStdString(n_std));
}

void Ob::updateBalance(int index)
{

}

void Ob::updateCredit(int index)
{

}

void Ob::updatePower(int index)
{

}

void Ob::detectGame()
{
    if (g->_view.m_gameValid) {
        rearrangeUnitblocks();
        sortUnitblocks();
//        setName(1);
//        setName(2);
    }
}

