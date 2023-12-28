#include "./producingblock.h"

#include "./ui_producingblock.h"

ProducingBlock::ProducingBlock(QWidget *parent) : QWidget(parent), ui(new Ui::ProducingBlock) {
    ui->setupUi(this);
}

ProducingBlock::~ProducingBlock() { delete ui; }
