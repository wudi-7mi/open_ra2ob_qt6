#include "ob.h"
#include "ui_ob.h"

Ob::Ob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ob)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

}

Ob::~Ob()
{
    delete ui;
}
