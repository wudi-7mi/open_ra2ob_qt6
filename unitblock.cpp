#include "./unitblock.h"

#include <QFile>
#include <QPainter>
#include <QPainterPath>

#include "./layoutsetting.h"
#include "./ui_unitblock.h"

Unitblock::Unitblock(QWidget *parent) : QWidget(parent), ui(new Ui::Unitblock) {
    ui->setupUi(this);
}

Unitblock::~Unitblock() { delete ui; }

void Unitblock::initUnit(QString name) {
    unit_name = name;
    setImage(name);
}

void Unitblock::setName(QString name) {
    unit_name = name;
    setImage(name);
}

void Unitblock::setImage(QString name) {
    QString img_str = ":/obicons/assets/obicons/" + name + ".png";

    QFile qf(img_str);

    if (!qf.exists()) {
        ui->img->setPixmap(QPixmap(":/obicons/assets/obicons/unit_placeholder_trans.png"));
        return;
    }

    ui->img->setPixmap(getRadius(QPixmap(img_str), 8));
}

QPixmap Unitblock::getRadius(QPixmap src, int radius) {
    QPixmap dest(src.width(), src.height());
    dest.fill(Qt::transparent);

    QPainter painter(&dest);

    QPainterPath path;
    QRect rect(0, 0, src.width(), src.height());
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, src.width(), src.height(), src);

    return dest;
}

void Unitblock::setNumber(int n) {
    QLabel *num = ui->number;
    num->setText(QString::number(n));
    num->adjustSize();

    int childX = (this->width() - num->width()) / 2;
    num->setGeometry(childX, num->y(), num->width(), num->height());

    num->show();

    ui->bg->show();
}

void Unitblock::setColor(std::string color) {
    QString q_color = QString::fromStdString("#" + color);

    ui->bg->setStyleSheet("background-color: " + q_color + ";" + layout::BOTTOM_RADIUS_8);
    ui->border->setStyleSheet("border: 1px solid " + q_color + ";" + layout::RADIUS_8);
}

void Unitblock::setEmpty() {
    QString img_str = ":/obicons/assets/obicons/unit_placeholder_trans.png";
    ui->img->setPixmap(QPixmap(img_str));

    ui->number->hide();
    ui->bg->hide();
}
