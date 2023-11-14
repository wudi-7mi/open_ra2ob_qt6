#include "unitblock.h"
#include "ui_unitblock.h"

#include <QFile>
#include <iostream>

Unitblock::Unitblock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Unitblock)
{
    ui->setupUi(this);
}

Unitblock::~Unitblock()
{
    delete ui;
}

void Unitblock::initUnit(QString name, int index)
{
    unit_name = name;
    player_index = index;
    initImg(name);
}

void Unitblock::initImg(QString name)
{
    QLabel *img = ui->img;
    QString img_str = ":/obicons/assets/obicons/" + name + ".png";

    QFile qf(img_str);

    if (!qf.exists()) {
        img->setPixmap(QPixmap(":/obicons/assets/obicons/unit_placeholder.png"));
        return;
    }

    img->setPixmap(QPixmap(img_str));
}

void Unitblock::updateNumber(QString number)
{
    QLabel *num = ui->number;
    num->setText(number);

    num->adjustSize();

    int childX = (this->width() - num->width()) / 2;

    num->setGeometry(childX, num->y(), num->width(), num->height());
}

void Unitblock::setColor(QString color)
{
    QString defaultStyle = ui->bg->styleSheet();

    ui->bg->setStyleSheet(defaultStyle + "background-color: " + color + ";");
}
