#include "./ob.h"

#include <QDebug>
#include <QPainter>
#include <QScreen>
#include <QShortcut>
#include <QTimer>
#include <fstream>

#include "./ui_ob.h"

using json = nlohmann::json;

Ob::Ob(QWidget *parent) : QWidget(parent), ui(new Ui::Ob) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint |
                         Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QScreen *screen = QGuiApplication::primaryScreen();
    this->setGeometry(screen->geometry());

    gls = &Globalsetting::getInstance();
    setLayoutByScreen();

    g = &Ra2ob::Game::getInstance();

    initPanel();
    initUnitblocks();
    initIfBar();

    setPanelByScreen();
    setUnitblocksByScreen();

    QTimer *detectGameTimer = new QTimer();
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(detectGame()));
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(toggleOb()));
    detectGameTimer->setInterval(500);
    detectGameTimer->start();

    QShortcut *sc_switch_screen = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(sc_switch_screen, &QShortcut::activated, this, &Ob::switchScreen);
}

Ob::~Ob() { delete ui; }

void Ob::paintEvent(QPaintEvent *) {
    QPainter *painter = new QPainter(this);

    paintTopPanel(painter, topPanelOffset, 0, topPanelWidth, topPanelHeight);
    paintRightPanel(painter, rightOffset);
    // paintLeftPanel(painter);
    paintBottomPanel(painter);

    painter->end();
}

void Ob::paintTopPanel(QPainter *painter, int offsetX, int offsetY, int pWidth, int pHeight) {
    painter->setOpacity(gls->c.top_panel_opacity);
    int pBottom = 5;
    int wCenter = gls->l.top_m + offsetX;

    QString qs_l  = QString::fromStdString("#" + qs_1);
    QString qs_r  = QString::fromStdString("#" + qs_2);
    QColor lColor = QColor(qs_l);
    QColor rColor = QColor(qs_r);

    QLinearGradient leftGradient(QPointF(wCenter - pWidth / 2, pHeight * 4), QPointF(wCenter, 0));
    leftGradient.setColorAt(0, lColor);
    leftGradient.setColorAt(0.8, Qt::black);

    painter->fillRect(wCenter - pWidth / 2, 0, pWidth / 2, pHeight, leftGradient);

    QLinearGradient rightGradient(QPointF(wCenter, 0), QPointF(wCenter + pWidth / 2, pHeight * 4));
    rightGradient.setColorAt(0.2, Qt::black);
    rightGradient.setColorAt(1, rColor);

    painter->fillRect(wCenter, 0, pWidth / 2, pHeight, rightGradient);

    QLinearGradient bottomGradient(QPointF(wCenter - pWidth / 2 - 100, 0),
                                   QPointF(wCenter + pWidth / 2 + 100, 0));
    bottomGradient.setColorAt(0, lColor);
    bottomGradient.setColorAt(1, rColor);

    painter->fillRect(wCenter - pWidth / 2, pHeight, pWidth, pBottom, bottomGradient);

    painter->setOpacity(1);

    QPixmap pixmap;
    int iconSide = gls->l.icon_side;
    int iconX    = wCenter - iconSide / 2;
    int iconY    = gls->l.top_h / 2 - iconSide / 2;

    QString iconPath = QString("%1%2%3").arg(":/icon/assets/icons/icon_").arg(iconSide).arg(".png");

    pixmap.load(iconPath);
    painter->drawPixmap(iconX, iconY, iconSide, iconSide, pixmap);

    return;
}

void Ob::paintRightPanel(QPainter *painter, int offsetX, int offsetY) {
    QColor fill(QColor("midnightblue"));
    painter->fillRect(
        QRect(offsetX + gls->l.right_x, offsetY + 0, layout::RIGHT_W, layout::RIGHT_HEADER_H),
        fill);
    painter->fillRect(QRect(offsetX + gls->l.right_x, offsetY + layout::RIGHT_HEADER_H,
                            layout::RIGHT_LEFTBORDER_W, layout::RIGHT_BORDER_H),
                      fill);
    painter->fillRect(QRect(offsetX + gls->l.right_x + layout::RIGHT_RIGHTBORDER_X,
                            offsetY + layout::RIGHT_HEADER_H, layout::RIGHT_RIGHTBORDER_W,
                            layout::RIGHT_BORDER_H),
                      fill);
    painter->fillRect(
        QRect(offsetX + gls->l.right_x, offsetY + layout::RIGHT_HEADER_H + layout::RIGHT_BORDER_H,
              layout::RIGHT_W, gls->l.right_bottom_h),
        fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter->setPen(border);
    painter->drawRect(
        QRect(offsetX + gls->l.map_x, offsetY + gls->l.map_y, layout::MAP_W, layout::MAP_H));

    return;
}

void Ob::paintLeftPanel(QPainter *painter) { return; }

void Ob::paintBottomPanel(QPainter *painter) {
    if (!gls->s.show_bottom_panel) {
        credit_1->setText("");
        credit_2->setText("");
        return;
    }

    QColor f_bg(QColor("midnightblue"));
    QColor f_g(QColor("green"));
    QColor f_r(QColor("red"));
    int w    = 5;
    int sx   = 60;
    int sx_1 = sx;
    int sx_2 = sx;

    QString qs_l  = QString::fromStdString("#" + qs_1);
    QString qs_r  = QString::fromStdString("#" + qs_2);
    QColor lColor = QColor(qs_l);
    QColor rColor = QColor(qs_r);

    painter->fillRect(QRect(0, gls->l.bottom_y, gls->l.right_x, 32), f_bg);

    painter->fillRect(QRect(0, gls->l.bottom_y1, sx, layout::BOTTOM_CREDIT_H), lColor);
    painter->fillRect(QRect(0, gls->l.bottom_y2, sx, layout::BOTTOM_CREDIT_H), rColor);

    QPen lBorder(lColor);
    lBorder.setWidth(2);
    painter->setPen(lBorder);
    painter->drawRect(QRect(0, gls->l.bottom_y1, gls->l.right_x - 1, layout::BOTTOM_CREDIT_H));
    QPen rBorder(rColor);
    rBorder.setWidth(2);
    painter->setPen(rBorder);
    painter->drawRect(QRect(0, gls->l.bottom_y2, gls->l.right_x - 1, layout::BOTTOM_CREDIT_H));

    if ((gls->l.right_x - sx) / (insufficient_fund_bar_1.length() + 1) < w) {
        w = 3;
    }
    if ((gls->l.right_x - sx) / (insufficient_fund_bar_1.length() + 1) < w) {
        w = 1;
    }
    for (int ifund : insufficient_fund_bar_1) {
        if (ifund == 0) {
            painter->fillRect(QRect(sx_1, gls->l.bottom_fill_y1, w, layout::BOTTOM_FILL_H), f_r);
        } else {
            painter->fillRect(QRect(sx_1, gls->l.bottom_fill_y1, w, layout::BOTTOM_FILL_H), f_g);
        }
        sx_1 += w;
    }
    for (int ifund : insufficient_fund_bar_2) {
        if (ifund == 0) {
            painter->fillRect(QRect(sx_2, gls->l.bottom_fill_y2, w, layout::BOTTOM_FILL_H), f_r);
        } else {
            painter->fillRect(QRect(sx_2, gls->l.bottom_fill_y2, w, layout::BOTTOM_FILL_H), f_g);
        }
        sx_2 += w;
    }

    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        return;
    }
    int p1_index = validPlayerIndex[0];
    int p2_index = validPlayerIndex[1];

    Ra2ob::tagGameInfo &gi = g->_gameInfo;

    int c_1       = gi.players[p1_index].panel.creditSpent;
    int ck_1      = c_1 / 1000;
    int ch_1      = c_1 % 1000 / 100;
    QString res_1 = QString::number(ck_1) + "." + QString::number(ch_1) + "k";

    int c_2       = gi.players[p2_index].panel.creditSpent;
    int ck_2      = c_2 / 1000;
    int ch_2      = c_2 % 1000 / 100;
    QString res_2 = QString::number(ck_2) + "." + QString::number(ch_2) + "k";

    credit_1->setText(res_1);
    credit_2->setText(res_2);
}

void Ob::initPanel() {
    this->pi_1 = new PlayerInfo(this);
    this->pi_2 = new PlayerInfo(this);
    pi_2->setMirror();
}

void Ob::initUnitblocks() {
    // Clear Unitblocks First
    for (auto *ub : ubs_p1) {
        delete ub;
    }
    ubs_p1.clear();
    for (auto *ub : ubs_p2) {
        delete ub;
    }
    ubs_p2.clear();

    for (int i = 0; i < gls->l.unitblocks; i++) {
        Unitblock *ub = new Unitblock(this);
        ub->initUnit("null");
        ub->setColor(qs_1);
        ub->setEmpty();
        ubs_p1.push_back(ub);
    }

    for (int i = 0; i < gls->l.unitblocks; i++) {
        Unitblock *ub = new Unitblock(this);
        ub->initUnit("null");
        ub->setColor(qs_2);
        ub->setEmpty();
        ubs_p2.push_back(ub);
    }
}

void Ob::initIfBar() {
    insufficient_fund_bar_1.clear();
    insufficient_fund_bar_2.clear();

    if (credit_1 == nullptr) {
        credit_1 = new QOutlineLabel(this);
    }

    if (credit_2 == nullptr) {
        credit_2 = new QOutlineLabel(this);
    }

    QFont font;
    font.setFamily(layout::OPPO_M);
    font.setPointSize(10);

    credit_1->setFont(font);
    credit_1->setText("0.0k");
    credit_1->setGeometry(layout::BOTTOM_CREDIT_X, gls->l.bottom_credit_y1, layout::BOTTOM_CREDIT_W,
                          layout::BOTTOM_CREDIT_H);
    credit_1->setStyleSheet("color: white");

    credit_2->setFont(font);
    credit_2->setText("0.0k");
    credit_2->setGeometry(layout::BOTTOM_CREDIT_X, gls->l.bottom_credit_y2, layout::BOTTOM_CREDIT_W,
                          layout::BOTTOM_CREDIT_H);
    credit_2->setStyleSheet("color: white");
}

void Ob::refreshUbs() {
    Ra2ob::tagGameInfo &gi = g->_gameInfo;

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

    Ra2ob::tagUnitsInfo &uInfo = players[0].units;

    int j;
    j = 0;

    auto it_unit = uInfo.units.begin();
    while (it_unit != uInfo.units.end() && j < gls->l.unitblocks) {
        if (it_unit->show == 0) {
            ++it_unit;
            continue;
        }
        if (it_unit->num != 0) {
            ubs_p1[j]->setNumber(it_unit->num);
            ubs_p1[j]->setName(QString::fromStdString(it_unit->unitName));
            ubs_p1[j]->setColor(qs_1);
            j++;
        }
        ++it_unit;
    }

    while (j < gls->l.unitblocks) {
        ubs_p1[j]->setEmpty();
        ubs_p1[j]->setColor(qs_1);
        j++;
    }

    j       = 0;
    uInfo   = players[1].units;
    it_unit = uInfo.units.begin();
    while (it_unit != uInfo.units.end() && j < gls->l.unitblocks) {
        if (it_unit->show == 0) {
            ++it_unit;
            continue;
        }
        if (it_unit->num != 0) {
            ubs_p2[j]->setNumber(it_unit->num);
            ubs_p2[j]->setName(QString::fromStdString(it_unit->unitName));
            ubs_p2[j]->setColor(qs_2);
            j++;
        }
        ++it_unit;
    }

    while (j < gls->l.unitblocks) {
        ubs_p2[j]->setEmpty();
        ubs_p2[j]->setColor(qs_2);
        j++;
    }
}

void Ob::setPanelByScreen() {
    this->pi_1->setGeometry(gls->l.top_m - gls->l.top_w / 2, 0, gls->l.top_w / 2, gls->l.top_h);
    this->pi_2->setGeometry(gls->l.top_m, 0, gls->l.top_w / 2, gls->l.top_h);
}

void Ob::setUnitblocksByScreen() {
    int i = 0;

    for (auto &ub : ubs_p1) {
        ub->setGeometry(gls->l.unit_x + rightOffset, gls->l.unit_y + i * layout::UNIT_HS,
                        layout::UNIT_W, layout::UNIT_H);
        ub->show();
        i++;
    }

    i = 0;
    for (auto &ub : ubs_p2) {
        ub->setGeometry(gls->l.unit_x + layout::UNIT_WS + rightOffset,
                        gls->l.unit_y + i * layout::UNIT_HS, layout::UNIT_W, layout::UNIT_H);
        ub->show();
        i++;
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

    int p_if_1 = pi_1->getInsufficientFund(p1_index);
    int p_if_2 = pi_2->getInsufficientFund(p2_index);

    insufficient_fund_bar_1.push_back(p_if_1);
    insufficient_fund_bar_2.push_back(p_if_2);
}

void Ob::refreshProducingBlock() {
    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        return;
    }
    int p1_index = validPlayerIndex[0];
    int p2_index = validPlayerIndex[1];

    // Clear previous ProducingBlock
    for (auto *pb : pb_1) {
        delete pb;
    }
    pb_1.clear();
    for (auto *pb : pb_2) {
        delete pb;
    }
    pb_2.clear();

    Ra2ob::tagGameInfo &gi = g->_gameInfo;

    Ra2ob::tagBuildingInfo &bi_1 = gi.players[p1_index].building;
    for (auto &bn : bi_1.list) {
        ProducingBlock *pb = new ProducingBlock(this);
        pb->initBlock(QString::fromStdString(bn.name));
        pb->setcolor(qs_1);
        pb->setProgress(bn.progress);
        pb->setStatus(bn.status);
        pb->setNumber(bn.number);
        pb_1.push_back(pb);
    }

    Ra2ob::tagBuildingInfo &bi_2 = gi.players[p2_index].building;
    for (auto &bn : bi_2.list) {
        ProducingBlock *pb = new ProducingBlock(this);
        pb->initBlock(QString::fromStdString(bn.name));
        pb->setcolor(qs_2);
        pb->setProgress(bn.progress);
        pb->setStatus(bn.status);
        pb->setNumber(bn.number);
        pb_2.push_back(pb);
    }

    int i = 0;
    for (auto &pb : pb_1) {
        pb->setGeometry(layout::PRODUCINGBLOCK_X + layout::PRODUCINGBLOCK_WS * i,
                        layout::PRODUCINGBLOCK_Y1, pb->width(), pb->height());
        pb->show();
        i++;
    }

    i = 0;
    for (auto &pb : pb_2) {
        pb->setGeometry(layout::PRODUCINGBLOCK_X + layout::PRODUCINGBLOCK_WS * i,
                        layout::PRODUCINGBLOCK_Y2, pb->width(), pb->height());
        pb->show();
        i++;
    }
}

void Ob::setPlayerColor() {
    std::vector<int> validPlayerIndex;
    int validPlayerNum = getValidPlayerIndex(&validPlayerIndex);

    if (validPlayerNum != 2) {
        return;
    }
    int p1_index = validPlayerIndex[0];
    int p2_index = validPlayerIndex[1];

    Ra2ob::tagGameInfo &gi = g->_gameInfo;

    qs_1 = gi.players[p1_index].panel.color;
    qs_2 = gi.players[p2_index].panel.color;

    if (qs_1 == "0") {
        qs_1 = layout::COLOR_DEFAULT;
    }
    if (qs_2 == "0") {
        qs_2 = layout::COLOR_DEFAULT;
    }
}

void Ob::setLayoutByScreen() {
    if (this->screen()->geometry().width() == layout::SC1K_W &&
        this->screen()->geometry().height() == layout::SC1K_H) {
        gls->loadLayoutSetting(1);
    } else if (this->screen()->geometry().width() == layout::SC2K_W &&
               this->screen()->geometry().height() == layout::SC2K_H) {
        gls->loadLayoutSetting(2);
    } else {
        gls->loadLayoutSetting(0, geometry().width(), geometry().height());
    }
}

int Ob::getValidPlayerIndex(std::vector<int> *vpi) {
    Ra2ob::tagGameInfo &gi = g->_gameInfo;

    int validPlayerNum = 0;
    for (int i = 0; i < Ra2ob::MAXPLAYER; i++) {
        if (gi.players[i].valid) {
            validPlayerNum++;
            vpi->push_back(i);
        }
    }

    return validPlayerNum;
}

void Ob::detectGame() {
    if (g->_gameInfo.isGameOver || g->_gameInfo.currentFrame < 5) {
        return;
    }
    if (g->_gameInfo.valid && (g->_gameInfo.isObserver || Ra2ob::GOODINTENTION)) {
        refreshUbs();
        refreshPanel();
        refreshProducingBlock();
        setPlayerColor();
        this->update();
    } else {
        // Todo: initialize
        initIfBar();
    }
}

void Ob::toggleOb() {
    if (!gls->s.show_all_panel) {
        this->hide();
        return;
    }

    if (g->_gameInfo.isGameOver || g->_gameInfo.currentFrame < 5) {
        this->hide();
        return;
    }

    int validPlayerNum = 0;
    for (int i = 0; i < Ra2ob::MAXPLAYER; i++) {
        if (g->_gameInfo.players[i].valid) {
            validPlayerNum++;
        }
    }
    if (validPlayerNum != 2) {
        this->hide();
        return;
    }

    if (g->_gameInfo.valid && (g->_gameInfo.isObserver || Ra2ob::GOODINTENTION)) {
        this->show();
        return;
    }

    this->hide();
}

void Ob::switchScreen() {
    auto screens = QGuiApplication::screens();

    int currentScreenIndex = 0;
    for (auto s : screens) {
        if (s == this->screen()) {
            break;
        }
        currentScreenIndex++;
    }

    int nextScreenIndex = (currentScreenIndex + 1) % screens.length();
    this->setGeometry(screens[nextScreenIndex]->geometry());

    setLayoutByScreen();

    setPanelByScreen();
    initUnitblocks();
    initIfBar();
    setUnitblocksByScreen();
}
