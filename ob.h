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
    void paintTopPanel(QPainter *painter, int offsetX = 0, int offsetY = 0, int pWidth = 1000,
                       int pHeight = 100);
    void paintRightPanel(QPainter *painter, int offsetX = 0, int offsetY = 0);
    void paintLeftPanel(QPainter *painter);
    void paintBottomPanel(QPainter *painter);

    void initPanel();
    void initUnitblocks();
    void initIfBar();
    void refreshUbs();

    void setPanelByScreen();
    void setUnitblocksByScreen();
    void refreshPanel();
    void refreshProducingBlock();

    void setPlayerColor();

    void setLayoutByScreen();

    int getValidPlayerIndex(std::vector<int> *vpi);

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
    QLabel *credit_1 = nullptr;
    QLabel *credit_2 = nullptr;

    bool forceHideOb = false;

    int rightOffset    = 0;
    int topPanelOffset = 0;
    int topPanelWidth  = layout::SC1K_TOP_W;
    int topPanelHeight = layout::SC1K_TOP_H;
    QVector<int> player_list;
    Ra2ob::Game *g;
    Globalsetting *gls;

private slots:
    void detectGame();
    void toggleOb();
    void hideOb();
    void switchScreen();
};

#endif  // OB_H
