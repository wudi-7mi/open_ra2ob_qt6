#ifndef OB_H
#define OB_H

#include <QColor>
#include <QWidget>
#include <string>
#include <vector>

#include "./globalsetting.h"
#include "./layoutsetting.h"
#include "./playerinfo.h"
#include "./producingblock.h"
#include "./qoutlinelabel.h"
#include "./unitblock.h"

namespace Ui {
class Ob;
}

class Ob : public QWidget {
    Q_OBJECT

public:
    explicit Ob(QWidget *parent = nullptr);
    ~Ob();

private:
    Ui::Ob *ui;
    void paintEvent(QPaintEvent *);
    void paintTopPanel(QPainter *painter);
    void paintRightPanel(QPainter *painter);
    void paintLeftPanel(QPainter *painter);
    void paintBottomPanel(QPainter *painter);

    void initPanel();
    void initUnitblocks();
    void initIfBar(bool clean = true);

    void refreshUbs();
    void refreshPlayerStatus();
    void refreshPanel();
    void refreshProducingBlock();

    void setPlayerColor();
    void setPanelByScreen();
    void setLayoutByScreen();

    bool checkEqual(const std::string &p1, const std::string &p2, const std::string &p1prev,
                    const std::string &p2prev);

    bool eventFilter(QObject *watched, QEvent *event);

    QVector<int> playersIndex;
    int playersNumber;
    int p1_index = -1;
    int p2_index = -1;
    QVector<Unitblock *> ubs_p1;
    QVector<Unitblock *> ubs_p2;
    PlayerInfo *pi_1;
    PlayerInfo *pi_2;
    std::string qs_1 = layout::COLOR_DEFAULT;
    std::string qs_2 = layout::COLOR_DEFAULT;
    QVector<ProducingBlock *> pb_1;
    QVector<ProducingBlock *> pb_2;
    QVector<int> insufficient_fund_bar_1;
    QVector<int> insufficient_fund_bar_2;
    QOutlineLabel *credit_1   = nullptr;
    QOutlineLabel *credit_2   = nullptr;
    QOutlineLabel *lb_mapname = nullptr;
    QLabel *lb_time           = nullptr;
    QLabel *lb_p1_score       = nullptr;
    QLabel *lb_p2_score       = nullptr;
    QLabel *lb_score_col      = nullptr;
    QLabel *lb_exchange       = nullptr;

    bool forceHideOb = false;

    int rightOffset    = 0;
    int topPanelOffset = 0;
    int topPanelWidth  = layout::TOP_W;
    int topPanelHeight = layout::TOP_H;
    QVector<int> player_list;
    Ra2ob::Game *g;
    Globalsetting *gls;

public slots:
    void detectGame();
    void toggleOb();
    void switchScreen();

signals:
    void playernameNeedsUpdate();
};

#endif  // OB_H
