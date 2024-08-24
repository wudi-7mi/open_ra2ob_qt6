#include "./ob.h"

#include <QDebug>
#include <QMouseEvent>
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

    paintTopPanel(painter);
    paintRightPanel(painter);
    // paintLeftPanel(painter);
    paintBottomPanel(painter);

    painter->end();
}

void Ob::paintTopPanel(QPainter *painter) {
    painter->setOpacity(gls->c.top_panel_opacity);
    int pBottom = 5;
    int wCenter = gls->l.top_m;
    int pWidth  = gls->l.top_w;
    int pHeight = gls->l.top_h;

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

    // Draw Time
    int frame          = g->_gameInfo.currentFrame;
    std::string s_time = Ra2ob::convertFrameToTimeString(frame, Ra2ob::GAMESPEED);

    int fsize = 10;
    if (lb_time == nullptr) {
        lb_time = new QLabel(this);
        lb_time->setFont(QFont(layout::OPPO_M, fsize));
        lb_time->setStyleSheet("color: #ffffff;");
        lb_time->show();
    }

    lb_time->setText(QString::fromStdString(s_time));
    lb_time->adjustSize();
    int childX = wCenter - lb_time->width() / 2;
    lb_time->setGeometry(childX, gls->l.top_h - fsize * 2, lb_time->width(), lb_time->height());

    // Draw Scoreboard
    QString p1_score = QString::number(gls->sb.p1_score);
    QString p2_score = QString::number(gls->sb.p2_score);

    //    std::string p1_name = g->_gameInfo.players[p1_index].panel.playerNameUtf;
    //    QString p1_nickname = QString::fromUtf8(p1_name);

    //    std::string p2_name = g->_gameInfo.players[p2_index].panel.playerNameUtf;
    //    QString p2_nickname = QString::fromUtf8(p2_name);

    //    if (QString::compare(p1_nickname, gls->sb.p2_nickname) == 0
    //        && QString::compare(p2_nickname, gls->sb.p1_nickname) == 0) {
    //        p1_score = QString::number(gls->sb.p2_score);
    //        p2_score = QString::number(gls->sb.p1_score);
    //    }

    int fscsize = 32;

    if (gls->sb.p1_score >= 10 && gls->sb.p2_score < 10) {
        p2_score = "0" + p2_score;
        fscsize  = 32;
    }
    if (gls->sb.p2_score >= 10 && gls->sb.p1_score < 10) {
        p1_score = "0" + p1_score;
        fscsize  = 32;
    }

    QFont fsc = QFont(layout::OPPO_B, fscsize);
    fsc.setBold(true);

    if (lb_p1_score == nullptr) {
        lb_p1_score = new QLabel(this);
        lb_p1_score->setFont(fsc);
        lb_p1_score->setStyleSheet("color: #ffffff;");
        lb_p1_score->show();
        lb_p1_score->installEventFilter(this);
    } else {
        lb_p1_score->setFont(fsc);
    }

    if (lb_p2_score == nullptr) {
        lb_p2_score = new QLabel(this);
        lb_p2_score->setFont(fsc);
        lb_p2_score->setStyleSheet("color: #ffffff;");
        lb_p2_score->show();
        lb_p2_score->installEventFilter(this);
    } else {
        lb_p2_score->setFont(fsc);
    }

    if (lb_score_col == nullptr) {
        lb_score_col = new QLabel(this);
        lb_score_col->setFont(fsc);
        lb_score_col->setStyleSheet("color: #ffffff;");
        lb_score_col->setText(":");
        lb_score_col->adjustSize();
    }
    { lb_score_col->setFont(fsc); }

    int yoffset   = 0;
    int coloffset = 6;
    int x         = wCenter - lb_score_col->width() / 2;
    int y         = (gls->l.top_h - lb_score_col->height()) / 2 - yoffset - 2;
    lb_score_col->setGeometry(x, y, lb_score_col->width(), lb_score_col->height());
    lb_score_col->show();

    int y_score;

    lb_p1_score->setText(p1_score);
    lb_p1_score->adjustSize();
    int x1 = lb_score_col->x() - lb_p1_score->width() - 2;
    lb_p2_score->setText(p2_score);
    lb_p2_score->adjustSize();
    int x2 = lb_score_col->x() + lb_score_col->width() + 2;

    y_score = (gls->l.top_h - lb_p1_score->height()) / 2 - yoffset;

    lb_p1_score->setGeometry(x1, y_score, lb_p1_score->width(), lb_p1_score->height());
    lb_p2_score->setGeometry(x2, y_score, lb_p2_score->width(), lb_p2_score->height());

    // Draw exchange icon
    if (lb_exchange == nullptr) {
        lb_exchange = new QLabel(this);
        QPixmap p;
        p.load(QString(":/panels/assets/panels/exchange.png"));
        p.scaled(14, 14, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        lb_exchange->setPixmap(p);
        lb_exchange->installEventFilter(this);
    }

    int ix = wCenter - 7;
    int iy = 2;
    lb_exchange->setGeometry(ix, iy, 14, 14);
    lb_exchange->show();

    return;
}

void Ob::paintRightPanel(QPainter *painter) {
    QColor fill(gls->c.sidepanel_color);
    painter->fillRect(QRect(gls->l.right_x, 0, gls->l.right_w, gls->l.right_header_h), fill);
    painter->fillRect(QRect(gls->l.right_x, gls->l.right_header_h, gls->l.right_leftborder_w,
                            gls->l.right_border_h),
                      fill);
    painter->fillRect(QRect(gls->l.right_x + gls->l.right_rightborder_x, gls->l.right_header_h,
                            gls->l.right_rightborder_x, gls->l.right_border_h),
                      fill);
    painter->fillRect(QRect(gls->l.right_x, gls->l.right_header_h + gls->l.right_border_h,
                            gls->l.right_w, gls->l.right_bottom_h),
                      fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter->setPen(border);
    painter->drawRect(QRect(gls->l.map_x, gls->l.map_y, gls->l.map_w, gls->l.map_h));

    int wCenter = (gls->l.w - gls->l.right_x) / 2 + gls->l.right_x;

    QPixmap pixmap;
    int iconSide = gls->l.icon_side;
    int iconX    = wCenter - iconSide / 2;
    int iconY    = gls->l.right_header_h / 2 - iconSide / 2;

    pixmap.load(QString(":/icon/assets/icons/icon_32.png"));
    pixmap = pixmap.scaled(gls->l.icon_side, gls->l.icon_side);
    painter->drawPixmap(iconX, iconY, iconSide, iconSide, pixmap);

    return;
}

void Ob::paintLeftPanel(QPainter *painter) { return; }

void Ob::paintBottomPanel(QPainter *painter) {
    if (!gls->s.show_bottom_panel) {
        credit_1->setText("");
        credit_2->setText("");
        return;
    }

    QColor f_bg(gls->c.sidepanel_color);
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

    painter->fillRect(QRect(0, gls->l.bottom_y1, sx, gls->l.bottom_credit_h), lColor);
    painter->fillRect(QRect(0, gls->l.bottom_y2, sx, gls->l.bottom_credit_h), rColor);

    QPen lBorder(lColor);
    lBorder.setWidth(2);
    painter->setPen(lBorder);
    painter->drawRect(QRect(0, gls->l.bottom_y1, gls->l.right_x - 1, gls->l.bottom_credit_h));
    QPen rBorder(rColor);
    rBorder.setWidth(2);
    painter->setPen(rBorder);
    painter->drawRect(QRect(0, gls->l.bottom_y2, gls->l.right_x - 1, gls->l.bottom_credit_h));

    if ((gls->l.right_x - sx) / (insufficient_fund_bar_1.length() + 1) < w) {
        w = 3;
    }
    if ((gls->l.right_x - sx) / (insufficient_fund_bar_1.length() + 1) < w) {
        w = 1;
    }
    for (int ifund : insufficient_fund_bar_1) {
        if (ifund == 0) {
            painter->fillRect(QRect(sx_1, gls->l.bottom_fill_y1, w, gls->l.bottom_fill_h), f_r);
        } else {
            painter->fillRect(QRect(sx_1, gls->l.bottom_fill_y1, w, gls->l.bottom_fill_h), f_g);
        }
        sx_1 += w;
    }
    for (int ifund : insufficient_fund_bar_2) {
        if (ifund == 0) {
            painter->fillRect(QRect(sx_2, gls->l.bottom_fill_y2, w, gls->l.bottom_fill_h), f_r);
        } else {
            painter->fillRect(QRect(sx_2, gls->l.bottom_fill_y2, w, gls->l.bottom_fill_h), f_g);
        }
        sx_2 += w;
    }

    if (playersNumber != 2) {
        return;
    }

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

    // paint map name
    if (lb_mapname == nullptr) {
        lb_mapname = new QOutlineLabel(this);
        lb_mapname->setFont(QFont(layout::OPPO_M, 12));
        lb_mapname->setStyleSheet("color: white");
    }

    QString mapName = QString::fromUtf8(g->_gameInfo.mapNameUtf);
    if (mapName.length() == 1) {
        return;
    }

    lb_mapname->setText(mapName);
    lb_mapname->adjustSize();

    int tw = lb_mapname->width();
    int px = 0;
    int py = gls->l.bottom_y - 28 - 1;

    painter->setOpacity(gls->c.top_panel_opacity);
    painter->fillRect(QRect(px, py, tw + 10, 28), gls->c.sidepanel_color);
    painter->setOpacity(1);

    lb_mapname->setGeometry(px + 5, py - 7, tw, 28);
    lb_mapname->show();
    return;
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
        ub->setGeometry(gls->l.unit_x + rightOffset, gls->l.unit_y + i * gls->l.unit_hs,
                        gls->l.unit_w, gls->l.unit_h);
        ub->show();
        ubs_p1.push_back(ub);
    }

    for (int i = 0; i < gls->l.unitblocks; i++) {
        Unitblock *ub = new Unitblock(this);
        ub->initUnit("null");
        ub->setColor(qs_2);
        ub->setEmpty();
        ub->setGeometry(gls->l.unit_x + gls->l.unit_ws + rightOffset,
                        gls->l.unit_y + i * gls->l.unit_hs, gls->l.unit_w, gls->l.unit_h);
        ub->show();
        ubs_p2.push_back(ub);
    }
}

void Ob::initIfBar(bool clean) {
    if (clean) {
        insufficient_fund_bar_1.clear();
        insufficient_fund_bar_2.clear();
    }

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
    credit_1->setGeometry(gls->l.bottom_credit_x, gls->l.bottom_credit_y1, gls->l.bottom_credit_w,
                          gls->l.bottom_credit_h);
    credit_1->setStyleSheet("color: white");

    credit_2->setFont(font);
    credit_2->setText("0.0k");
    credit_2->setGeometry(gls->l.bottom_credit_x, gls->l.bottom_credit_y2, gls->l.bottom_credit_w,
                          gls->l.bottom_credit_h);
    credit_2->setStyleSheet("color: white");
}

void Ob::refreshUbs() {
    Ra2ob::tagGameInfo &gi = g->_gameInfo;

    if (!gi.valid) {
        return;
    }

    if (playersNumber != 2) {
        std::cerr << "No two valid players.\n";
        return;
    }

    Ra2ob::tagUnitsInfo &uInfo = gi.players[p1_index].units;

    int j = 0;

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
    uInfo   = gi.players[p2_index].units;
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

    this->pi_1->rearrange();
    this->pi_2->rearrange();
    this->pi_2->setMirror();
}

void Ob::refreshPlayerStatus() {
    playersNumber = 0;
    playersIndex.clear();

    for (int i = 0; i < Ra2ob::MAXPLAYER; i++) {
        if (g->_gameInfo.players[i].valid) {
            playersNumber++;
            playersIndex.push_back(i);
        }
    }

    if (playersNumber == 2) {
        p1_index = playersIndex[0];
        p2_index = playersIndex[1];
    }
}

void Ob::refreshPanel() {
    if (playersNumber != 2) {
        return;
    }

    pi_1->setAll(p1_index);
    pi_2->setAll(p2_index);

    std::string p1_name = g->_gameInfo.players[p1_index].panel.playerNameUtf;
    QString p1_nickname = QString::fromUtf8(p1_name);

    std::string p2_name = g->_gameInfo.players[p2_index].panel.playerNameUtf;
    QString p2_nickname = QString::fromUtf8(p2_name);

    gls->sb.p1_nickname = p1_nickname;
    gls->sb.p2_nickname = p2_nickname;

    int p_if_1 = pi_1->getInsufficientFund(p1_index);
    int p_if_2 = pi_2->getInsufficientFund(p2_index);

    insufficient_fund_bar_1.push_back(p_if_1);
    insufficient_fund_bar_2.push_back(p_if_2);
}

void Ob::refreshProducingBlock() {
    if (playersNumber != 2) {
        return;
    }

    int p1_index = playersIndex[0];
    int p2_index = playersIndex[1];

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
        pb->setGeometry(gls->l.producingblock_x + gls->l.producingblock_ws * i,
                        gls->l.producingblock_y1, pb->width(), pb->height());
        pb->show();
        i++;
    }

    i = 0;
    for (auto &pb : pb_2) {
        pb->setGeometry(gls->l.producingblock_x + gls->l.producingblock_ws * i,
                        gls->l.producingblock_y2, pb->width(), pb->height());
        pb->show();
        i++;
    }
}

void Ob::setPlayerColor() {
    if (playersNumber != 2) {
        return;
    }

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
    qreal ratio = this->screen()->devicePixelRatio();
    int w       = this->screen()->geometry().width() * ratio;
    int h       = this->screen()->geometry().height() * ratio;

    gls->loadLayoutSetting(w, h, ratio);
}

bool Ob::checkEqual(const std::string &p1, const std::string &p2, const std::string &p1prev,
                    const std::string &p2prev) {
    bool p1EqualsPrev = (p1 == p1prev) || (p1 == p2prev);
    bool p2EqualsPrev = (p2 == p1prev) || (p2 == p2prev);
    return p1EqualsPrev && p2EqualsPrev;
}

bool Ob::eventFilter(QObject *watched, QEvent *event) {
    if (watched == lb_p1_score) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                gls->sb.p1_score += 1;
                return true;
            } else {
                if (gls->sb.p1_score != 0) {
                    gls->sb.p1_score -= 1;
                    return true;
                }
            }
        }
    }
    if (watched == lb_p2_score) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                gls->sb.p2_score += 1;
                return true;
            } else {
                if (gls->sb.p2_score != 0) {
                    gls->sb.p2_score -= 1;
                    return true;
                }
            }
        }
    }
    if (watched == lb_exchange) {
        if (event->type() == QEvent::MouseButtonPress) {
            int t            = gls->sb.p1_score;
            gls->sb.p1_score = gls->sb.p2_score;
            gls->sb.p2_score = t;
        }
    }
    return false;
}

void Ob::detectGame() {
    if (g->_gameInfo.valid && (g->_gameInfo.isObserver || Ra2ob::GOODINTENTION)) {
        gls->game_start = true;
    }

    if (!g->_gameInfo.valid) {
        gls->game_end = true;
    }

    if (g->_gameInfo.isGameOver || g->_gameInfo.currentFrame < 5) {
        initIfBar();
        return;
    }

    if (g->_gameInfo.valid && (g->_gameInfo.isObserver || Ra2ob::GOODINTENTION)) {
        refreshPlayerStatus();
        refreshUbs();
        refreshPanel();
        refreshProducingBlock();
        setPlayerColor();
        if (gls->game_start && gls->game_end) {
            gls->game_start = false;
            gls->game_end   = false;
            gls->game_numbers++;
            std::cout << "te\n" << std::endl;
            emit playernameNeedsUpdate();
        }
        // This this->update is used to refresh panel's transparency.
        this->update();
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

    if (!(g->_gameInfo.isObserver || Ra2ob::GOODINTENTION)) {
        this->hide();
        return;
    }

    if (g->_gameInfo.isGamePaused) {
        this->hide();
        return;
    }

    if (g->_gameInfo.valid) {
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
    initIfBar(false);
}
