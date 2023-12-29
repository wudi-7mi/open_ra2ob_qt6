#include "./producingblock.h"

#include <QFile>
#include <QRect>
#include <QPainter>

#include "./ui_producingblock.h"

ProducingBlock::ProducingBlock(QWidget *parent) : QWidget(parent), ui(new Ui::ProducingBlock) {
    ui->setupUi(this);
}

ProducingBlock::~ProducingBlock() {
    clean = true;
    update();
    delete ui;
}

void ProducingBlock::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (clean) {
        const QRect &rect = this->rect();

        painter.eraseRect(rect);
        painter.end();

        return;
    }

    painter.fillRect(
        QRect(0, 0, this->width(), this->height()), blockColor
        );

    QLabel *t = ui->lb_text;
    if (blockStatus != "") {
        t->setText(blockStatus);
        t->adjustSize();

        int childX = (this->width() - t->width()) / 2;
        t->setGeometry(childX, t->y(), t->width(), t->height());
        t->show();
        return;
    }

    t->hide();

    int i = 0;
    while (i < blockProgress) {
        painter.fillRect(QRect(8 + i, 57, 1, 12), QColor("white"));
        i++;
    }
    while (i < complete) {
        painter.fillRect(QRect(8 + i, 57, 1, 12), QColor("black"));
        i++;
    }

    painter.end();
}

void ProducingBlock::initBlock(QString name) {
    blockName = name;
    setImage(name);
}

void ProducingBlock::setProgress(int progress) {
    blockProgress = progress;
}

void ProducingBlock::setStatus(int status) {
    if (blockProgress == 54) {
        blockStatus = "Ready";
    } else if (status == 1) {
        blockStatus = "On Hold";
    } else {
        blockStatus = "";
    }
}

void ProducingBlock::setImage(QString name) {
    QString img_str = ":/obicons/assets/obicons/" + name + ".png";

    QFile qf(img_str);

    if (!qf.exists()) {
        ui->lb_img->setPixmap(QPixmap(":/obicons/assets/obicons/unit_placeholder_trans.png"));
        return;
    }

    ui->lb_img->setPixmap(QPixmap(img_str));
}

void ProducingBlock::setcolor(std::string color) {
    blockColor = QString::fromStdString("#" + color);
}
