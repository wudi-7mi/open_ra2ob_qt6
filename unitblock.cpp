#include "unitblock.h"
#include "ui_unitblock.h"

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

void Unitblock::initUnit(QString name)
{
    unit_name = name;
    initImg(name);
}

void Unitblock::initImg(QString name)
{
    QLabel *img = ui->img;
    QString img_str = ":/obicons/assets/obicons/" + name + ".png";

    img->setPixmap(QPixmap(img_str));
}

void Unitblock::updateNumber(QString number)
{
    QLabel *num = ui->number;
    num->setText(number);
}
