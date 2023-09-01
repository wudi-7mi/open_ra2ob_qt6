#include "ob.h"
#include "ui_ob.h"

Ob::Ob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ob)
{
    ui->setupUi(this);
}

Ob::~Ob()
{
    delete ui;
}
