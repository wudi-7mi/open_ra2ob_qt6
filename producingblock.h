#ifndef PRODUCINGBLOCK_H
#define PRODUCINGBLOCK_H

#include <QWidget>
#include <string>

#include "./qoutlinelabel.h"

namespace Ui {
class ProducingBlock;
}

class ProducingBlock : public QWidget {
    Q_OBJECT

public:
    explicit ProducingBlock(QWidget *parent = nullptr);
    ~ProducingBlock();
    void paintEvent(QPaintEvent *);

    void initBlock(QString name);
    void setProgress(int progress);
    void setStatus(int status);
    void setNumber(int number);
    void setImage(QString name);
    void setcolor(std::string color);
    QColor getDarkerColor(QColor qc);

    int complete = 54;
    int blockProgress;
    QString blockName;
    QString blockStatus;
    int blockNumber;
    QColor blockColor;
    bool clean = false;

    QOutlineLabel *lb_status;
    QOutlineLabel *lb_number;

private:
    Ui::ProducingBlock *ui;
};

#endif  // PRODUCINGBLOCK_H
