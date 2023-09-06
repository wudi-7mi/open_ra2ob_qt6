#ifndef OB_H
#define OB_H

#include <QWidget>

namespace Ui {
class Ob;
}

class Ob : public QWidget
{
    Q_OBJECT

public:
    explicit Ob(QWidget *parent = nullptr);
    ~Ob();

private:
    Ui::Ob *ui;
    void paintEvent(QPaintEvent *);
    void paintTopPanel(QPainter &painter, QColor lColor, QColor rColor);
    void paintRightPanel(QPainter &painter);
    void paintLeftPanel(QPainter &painter);
};

#endif // OB_H
