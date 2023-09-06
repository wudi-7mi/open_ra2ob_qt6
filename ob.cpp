#include "ob.h"
#include "ui_ob.h"

#include <QPainter>

Ob::Ob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ob)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

Ob::~Ob()
{
    delete ui;
}

void Ob::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

    paintTopPanel(*painter, QColor("#ffff00"), QColor("#0000ff"));
    paintRightPanel(*painter);
    paintLeftPanel(*painter);

    painter->end();
}

void Ob::paintTopPanel(QPainter &painter, QColor lColor, QColor rColor)
{
    painter.setOpacity(0.8);
    int pWidth = 1000;
    int pHeight = 100;
    int pBottom = 5;
    int wCenter = (width() - 168) / 2;

    QLinearGradient leftGradient(QPointF(wCenter - pWidth / 2, pHeight * 4), QPointF(wCenter, 0));
    leftGradient.setColorAt(0, lColor);
    leftGradient.setColorAt(0.8, Qt::black);

    painter.fillRect(wCenter - pWidth / 2, 0, pWidth / 2, pHeight, leftGradient);

    QLinearGradient rightGradient(QPointF(wCenter, 0), QPointF(wCenter + pWidth / 2, pHeight * 4));
    rightGradient.setColorAt(0.2, Qt::black);
    rightGradient.setColorAt(1, rColor);

    painter.fillRect(wCenter, 0, pWidth / 2, pHeight, rightGradient);

    QLinearGradient bottomGradient(QPointF(wCenter - pWidth / 2 - 100, 0), QPointF(wCenter + pWidth / 2 + 100, 0));
    bottomGradient.setColorAt(0, lColor);
    bottomGradient.setColorAt(1, rColor);

    painter.fillRect(wCenter - pWidth / 2, pHeight, pWidth, pBottom, bottomGradient);

    painter.setOpacity(1);

    QPixmap pixmap;
    int iconWidth = 100;
    int iconX = wCenter - iconWidth / 2;
    int iconY = 0;

    pixmap.load((QString(":/icon/icon_100.png")));
    painter.drawPixmap(iconX, iconY, iconWidth, iconWidth, pixmap);

    return;
}


void Ob::paintRightPanel(QPainter &painter)
{
    QColor fill(QColor("midnightblue"));
    painter.fillRect(QRect(1752, 0, 168, 48), fill);
    painter.fillRect(QRect(1752, 48, 12, 110), fill);
    painter.fillRect(QRect(1905, 48, 15, 110), fill);
    painter.fillRect(QRect(1752, 158, 168, 922), fill);

    QPen border(Qt::gray);
    border.setWidth(1);

    painter.setPen(border);
    painter.drawRect(QRect(1763, 47, 142, 111));

    return;
}


void Ob::paintLeftPanel(QPainter &painter)
{
    return;
}

