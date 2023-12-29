#ifndef PRODUCINGBLOCK_H
#define PRODUCINGBLOCK_H

#include <QWidget>
#include <QPaintEvent>
#include <string>

namespace Ui {
class ProducingBlock;
}

class ProducingBlock : public QWidget {
    Q_OBJECT

public:
    explicit ProducingBlock(QWidget *parent = nullptr);
    ~ProducingBlock();
    void paintEvent(QPaintEvent *event);

    void initBlock(QString name);
    void setProgress(int progress);
    void setStatus(int status);
    void setImage(QString name);
    void setcolor(std::string color);

    int complete = 54;
    int blockProgress;
    QString blockName;
    QString blockStatus;
    QColor blockColor;
    bool clean = false;

private:
    Ui::ProducingBlock *ui;
};

#endif  // PRODUCINGBLOCK_H
