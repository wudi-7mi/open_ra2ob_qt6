#include "./qoutlinelabel.h"

#include <QPainter>
#include <QPainterPath>

QOutlineLabel::QOutlineLabel(QWidget *parent) : QLabel(parent) {}

void QOutlineLabel::setOutline(QColor cf, QColor co, int ow, bool anti) {
    cFill            = cf;
    cOutline         = co;
    outlineWidth     = ow;
    antiAntialiasing = anti;
}

void QOutlineLabel::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    if (antiAntialiasing) {
        painter.setRenderHint(QPainter::Antialiasing);
    }

    QFont font   = this->font();
    QString text = this->text();

    QPainterPath path;
    QRect rect = this->contentsRect();
    path.addText(rect.bottomLeft(), font, text);

    painter.setPen(QPen(cOutline, outlineWidth));
    painter.drawPath(path);

    painter.fillPath(path, cFill);

    painter.end();
}
