#include "./ob.h"

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

    g = &Ra2ob::Game::getInstance();

    initPanel();
    initUnitblocks();

    QTimer *detectGameTimer = new QTimer();
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(detectGame()));
    connect(detectGameTimer, SIGNAL(timeout()), this, SLOT(toggleOb()));
    detectGameTimer->setInterval(500);
    detectGameTimer->start();

    QScreen *screen = QGuiApplication::primaryScreen();
    this->setScreen(screen);

    QShortcut *shortcut         = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_H), this);
    QShortcut *sc_switch_screen = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &Ob::hideOb);
    connect(sc_switch_screen, &QShortcut::activated, this, &Ob::switchScreen);
}

Ob::~Ob() { delete ui; }

void Ob::paintEvent(QPaintEvent *) {
    QPainter *painter = new QPainter(this);

    paintTopPanel(painter, topPanelOffset, 0, topPanelWidth, topPanelHeight);
    paintRightPanel(painter, rightOffset);
    // paintLeftPanel(painter);

    painter->end();
}

void Ob::paintTopPanel(QPainter *painter, int offsetX, int offsetY, int pWidth, int pHeight) {
    painter->setOpacity(1);
    int pBottom = 5;
    int wCenter = layout::SC1K_TOP_M + offsetX;

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
    int iconSide = layout::SC1K_ICON_SIDE;
    int iconX    = wCenter - iconSide / 2;
    int iconY    = layout::SC1K_TOP_H / 2 - iconSide / 2;

    QString iconPath = QString("%1%2%3").arg(":/icon/assets/icons/icon_").arg(iconSide).arg(".png");

    pixmap.load(iconPath);
    painter->drawPixmap(iconX, iconY, iconSide, iconSide, pixmap);

    return;
}

void Ob::paintRightPanel(QPainter *painter, int offsetX, int offsetY) {
    QColor fill(QColor("midnightblue"));
    painter->fillRect(
        QRect(offsetX + layout::SC1K_RIGHT_X, offsetY + 0, layout::RIGHT_W, layout::RIGHT_HEADER_H),
        fill);
    painter->fillRect(QRect(offsetX + layout::SC1K_RIGHT_X, offsetY + layout::RIGHT_HEADER_H,
                            layout::RIGHT_LEFTBORDER_W, layout::RIGHT_BORDER_H),
                      fill);
    painter->fillRect(QRect(offsetX + layout::SC1K_RIGHT_X + layout::RIGHT_RIGHTBORDER_START,
                            offsetY + layout::RIGHT_HEADER_H, layout::RIGHT_RIGHTBORDER_W,
                            layout::RIGHT_BORDER_H),
                      fill);
    painter->fillRect(QRect(offsetX + layout::SC1K_RIGHT_X,
                            offsetY + layout::RIGHT_HEADER_H + layout::RIGHT_BORDER_H,
                            layout::RIGHT_W, layout::SC1K_RIGHT_BOTTOM_H),
                      fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter->setPen(border);
    painter->drawRect(QRect(offsetX + layout::SC1K_MAP_X, offsetY + layout::SC1K_MAP_Y,
                            layout::MAP_W, layout::MAP_H));

    return;
}

void Ob::initPanel() {
    this->pi_1 = new PlayerInfo(this);
    this->pi_2 = new PlayerInfo(this);

    this->pi_1->setGeometry(layout::SC1K_TOP_M - layout::SC1K_TOP_W / 2, 0, layout::SC1K_TOP_W / 2,
                            layout::SC1K_TOP_H);
    this->pi_2->setGeometry(layout::SC1K_TOP_M, 0, layout::SC1K_TOP_W / 2, layout::SC1K_TOP_H);
    pi_2->setMirror();
}

void Ob::initUnitblocks() {
    for (int i = 0; i < layout::SC1K_UNITBLOCKS; i++) {
        Unitblock *ub = new Unitblock(this);
        ub->initUnit("null");
        ub->setGeometry(layout::SC1K_UNIT_X + rightOffset,
                        layout::SC1K_UNIT_Y + i * layout::SC1K_UNIT_Hs, layout::SC1K_UNIT_W,
                        layout::SC1K_UNIT_H);
        ub->setColor(qs_1);
        ub->setEmpty();
        ub->show();
        ubs_p1.push_back(ub);
    }

    for (int i = 0; i < layout::SC1K_UNITBLOCKS; i++) {
        Unitblock *ub = new Unitblock(this);
        ub->initUnit("null");
        ub->setGeometry(layout::SC1K_UNIT_X + layout::SC1K_UNIT_Ws + rightOffset,
                        layout::SC1K_UNIT_Y + i * layout::SC1K_UNIT_Hs, layout::SC1K_UNIT_W,
                        layout::SC1K_UNIT_H);
        ub->setColor(qs_2);
        ub->setEmpty();
        ub->show();
        ubs_p2.push_back(ub);
    }
}

void Ob::refreshUbs() {
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

    Ra2ob::tagUnitsInfo uInfo = players[0].units;

    int j;
    j = 0;

    auto it_unit = uInfo.units.begin();
    while (it_unit != uInfo.units.end() && j < layout::SC1K_UNITBLOCKS) {
        if (it_unit->num != 0) {
            ubs_p1[j]->setNumber(it_unit->num);
            ubs_p1[j]->setName(QString::fromStdString(it_unit->unitName));
            ubs_p1[j]->setColor(qs_1);
            j++;
        }
        ++it_unit;
    }

    while (j < layout::SC1K_UNITBLOCKS) {
        ubs_p1[j]->setEmpty();
        ubs_p1[j]->setColor(qs_1);
        j++;
    }

    j       = 0;
    uInfo   = players[1].units;
    it_unit = uInfo.units.begin();
    while (it_unit != uInfo.units.end() && j < layout::SC1K_UNITBLOCKS) {
        if (it_unit->num != 0) {
            ubs_p2[j]->setNumber(it_unit->num);
            ubs_p2[j]->setName(QString::fromStdString(it_unit->unitName));
            ubs_p2[j]->setColor(qs_2);
            j++;
        }
        ++it_unit;
    }

    while (j < layout::SC1K_UNITBLOCKS) {
        ubs_p2[j]->setEmpty();
        ubs_p2[j]->setColor(qs_2);
        j++;
    }
}

void Ob::paintLeftPanel(QPainter *painter) { return; }

void Ob::sortUnitblocks() { return; }

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

int Ob::getValidPlayerIndex(std::vector<int> *vpi) {
    Ra2ob::tagGameInfo gi = g->_gameInfo;

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
    if (g->_gameInfo.valid) {
        refreshUbs();
        refreshPanel();
        setPlayerColor();
        this->update();
    }
}

void Ob::toggleOb() {
    Ra2ob::Game &g = Ra2ob::Game::getInstance();

    if (g._gameInfo.valid && !forceHideOb) {
        this->show();
        return;
    }

    this->hide();
}

void Ob::hideOb() {
    if (forceHideOb) {
        forceHideOb = !forceHideOb;
        this->show();
        return;
    }

    forceHideOb = !forceHideOb;
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
}
