#ifndef QOUTLINELABEL_H
#define QOUTLINELABEL_H

#include <QLabel>

class QOutlineLabel : public QLabel {
public:
    explicit QOutlineLabel(QWidget *parent = nullptr);
    void setOutline(QColor cf, QColor co, int ow, bool anti);

protected:
    void paintEvent(QPaintEvent *);
    QColor cFill          = Qt::white;
    QColor cOutline       = Qt::black;
    int outlineWidth      = 2;
    bool antiAntialiasing = true;
};

#endif  // QOUTLINELABEL_H
