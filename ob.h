#ifndef OB_H
#define OB_H

#include <QWidget>
#include <QColor>

#include "./unitblock.h"
#include "./playerinfo.h"

#include "./Ra2ob/Ra2ob"

namespace Ui {
class Ob;
}

class Ob : public QWidget
{
    Q_OBJECT

public:
    explicit Ob(QWidget *parent = nullptr);
    ~Ob();

private:
    Ui::Ob *ui;
    void paintEvent(QPaintEvent *);
    void paintTopPanel(
        QPainter &painter,
        int offsetX = 0,
        int offsetY = 0,
        int pWidth = 1000,
        int pHeight = 100
    );
    void paintRightPanel(
        QPainter &painter,
        int offsetX = 0,
        int offsetY = 0
    );
    void paintLeftPanel(QPainter &painter);

    void rearrangeUnitblocks();
    void sortUnitblocks();
    void refreshPanel();

    void setPlayerColor();

    int getValidPlayerIndex(std::vector<int>* vpi);

    QVector<Unitblock*> unitblock_list_player_0;
    QVector<Unitblock*> unitblock_list_player_1;
    PlayerInfo* pi_1;
    PlayerInfo* pi_2;
    std::string qs_1 = "ffff00";
    std::string qs_2 = "0000ff";

//    int rightOffset = -425;
//    int topPanelOffset = -150;
    int rightOffset = 0;
    int topPanelOffset = 0;
    int topPanelWidth = 800;
    int topPanelHeight = 80;
    QVector<int> player_list;
    Ra2ob::Game* g;

private slots:
    void detectGame();
};

#endif // OB_H
