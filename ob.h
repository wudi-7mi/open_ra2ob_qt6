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
};

#endif // OB_H
