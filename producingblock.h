#ifndef PRODUCINGBLOCK_H
#define PRODUCINGBLOCK_H

#include <QWidget>
#include <string>

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
    void setName(QString name);
    void setImage(QString name);
    void setcolor(std::string color);

    int blockProgress;
    QString blockName;
    QString blockStatus;

private:
    Ui::ProducingBlock *ui;
};

#endif  // PRODUCINGBLOCK_H
