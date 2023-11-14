#ifndef OB_H
#define OB_H

#include <QWidget>

#include "unitblock.h"

#include "ra2ob.hpp"

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
        QColor lColor,
        QColor rColor,
        int offsetX = 0,
        int offsetY = 0,
        int pWidth = 1000,
        int pHeight = 100,
        int rightPanelWidth = 168
    );
    void paintRightPanel(
        QPainter &painter,
        int offsetX = 0,
        int offsetY = 0,
        int rightPanelWidth = 168,
        int rightPanelHeight = 1080
    );
    void paintLeftPanel(QPainter &painter);

    void refreshData();

    void rearrangeUnitblocks();
    void sortUnitblocks();

    void setName(int index);
    void setCountry(int index);
    void setColor(int index);

    void updateView(Unitblock& ub);
    void updateBalance(int index);
    void updateCredit(int index);
    void updatePower(int index);

    QVector<Unitblock*> unitblock_list_player_0;
    QVector<Unitblock*> unitblock_list_player_1;
    QVector<int> player_list;
    Ra2ob* g;

private slots:
    void detectGame();
};

#endif // OB_H
