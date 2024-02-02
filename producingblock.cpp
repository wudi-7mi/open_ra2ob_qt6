#include "./producingblock.h"

#include <QFile>
#include <QPainter>
#include <QRect>

#include "./layoutsetting.h"
#include "./ui_producingblock.h"

ProducingBlock::ProducingBlock(QWidget *parent) : QWidget(parent), ui(new Ui::ProducingBlock) {
    ui->setupUi(this);

    gls = &Globalsetting::getInstance();

    lb_status = new QOutlineLabel(this);
    lb_number = new QOutlineLabel(this);

    QFont font;
    font.setFamily(layout::OPPO_M);
    font.setPointSize(10 / gls->l.ratio);

    lb_status->setFont(font);
    lb_status->setOutline(Qt::white, QColor(30, 27, 24), 3, true);

    font.setPointSize(9 / gls->l.ratio);
    lb_number->setFont(font);
    lb_number->setOutline(Qt::white, QColor(30, 27, 24), 3, true);

    this->setGeometry(0, 0, gls->l.producingblock_wh, gls->l.producingblock_wh);
    ui->lb_img->setGeometry(gls->l.producing_img_x, gls->l.producing_img_y, gls->l.producing_img_w,
                            gls->l.producing_img_h);
}

ProducingBlock::~ProducingBlock() {
    clean = true;
    update();
    delete ui;
}

void ProducingBlock::paintEvent(QPaintEvent *) {
    QPainter painter(this);

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
    t->setGeometry(childX - 1, gls->l.producing_status_y, t->width() + 2, t->height());
    t->show();

    if (blockNumber > 1) {
        t = lb_number;
        t->setText(QString::number(blockNumber));
        t->adjustSize();
        childX = (this->width() - t->width()) / 2;
        t->setGeometry(childX - 1, gls->l.producing_number_y, t->width() + 2, t->height());
        t->show();
    }

    int i = 0;
    while (i < blockProgress / gls->l.ratio) {
        painter.fillRect(QRect(gls->l.producing_progress_x + i, gls->l.producing_progress_y, 1,
                               gls->l.producing_progress_h),
                         getDarkerColor(blockColor));
        i++;
    }
    while (i < complete / gls->l.ratio) {
        painter.fillRect(QRect(gls->l.producing_progress_x + i, gls->l.producing_progress_y, 1,
                               gls->l.producing_progress_h),
                         gls->c.producing_stripe_color);
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
