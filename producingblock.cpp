#include "./producingblock.h"

#include <QFile>
#include <QPainter>
#include <QRect>

#include "./globalsetting.h"
#include "./layoutsetting.h"
#include "./ui_producingblock.h"

ProducingBlock::ProducingBlock(QWidget *parent) : QWidget(parent), ui(new Ui::ProducingBlock) {
    ui->setupUi(this);

    lb_status = new QOutlineLabel(this);
    lb_number = new QOutlineLabel(this);

    QFont font;
    font.setFamily("OPlusSans 3.0 Medium");
    font.setPointSize(10);

    lb_status->setFont(font);
    lb_status->setOutline(Qt::white, QColor(30, 27, 24), 3, true);

    font.setPointSize(9);
    lb_number->setFont(font);
    lb_number->setOutline(Qt::white, QColor(30, 27, 24), 3, true);
}

ProducingBlock::~ProducingBlock() {
    clean = true;
    update();
    delete ui;
}

void ProducingBlock::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    Globalsetting *gls = &Globalsetting::getInstance();

    if (clean) {
        const QRect &rect = this->rect();

        painter.eraseRect(rect);
        painter.end();

        return;
    }

    painter.fillRect(QRect(0, 0, this->width(), this->height()), blockColor);

    QOutlineLabel *t = lb_status;
    t->setText(blockStatus);
    t->adjustSize();
    int childX = (this->width() - t->width()) / 2;
    t->setGeometry(childX - 1, layout::PRODUCING_STATUS_Y, t->width() + 2, t->height());
    t->show();

    if (blockNumber > 1) {
        t = lb_number;
        t->setText(QString::number(blockNumber));
        t->adjustSize();
        childX = (this->width() - t->width()) / 2;
        t->setGeometry(childX - 1, layout::PRODUCING_NUMBER_Y, t->width() + 2, t->height());
        t->show();
    }

    int i = 0;
    while (i < blockProgress) {
        painter.fillRect(QRect(layout::PRODUCING_PROGRESS_X + i, layout::PRODUCING_PROGRESS_Y, 1,
                               layout::PRODUCING_PROGRESS_H),
                         getDarkerColor(blockColor));
        i++;
    }
    while (i < complete) {
        painter.fillRect(QRect(layout::PRODUCING_PROGRESS_X + i, layout::PRODUCING_PROGRESS_Y, 1,
                               layout::PRODUCING_PROGRESS_H),
                         gls->c.producing_stripe);
        i++;
    }

    painter.end();
}

void ProducingBlock::initBlock(QString name) {
    blockName = name;
    setImage(name);
}

void ProducingBlock::setProgress(int progress) { blockProgress = progress; }

void ProducingBlock::setStatus(int status) {
    if (blockProgress == 54) {
        blockStatus = "Ready";
    } else if (status == 1) {
        blockStatus = "On Hold";
    } else {
        blockStatus = "";
    }
}

void ProducingBlock::setNumber(int number) { blockNumber = number; }

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

QColor ProducingBlock::getDarkerColor(QColor qc) {
    int red   = qc.red();
    int green = qc.green();
    int blue  = qc.blue();

    int delta = 50;

    int darkerRed   = qMax(0, red - delta);
    int darkerGreen = qMax(0, green - delta);
    int darkerBlue  = qMax(0, blue - delta);

    return QColor(darkerRed, darkerGreen, darkerBlue);
}
