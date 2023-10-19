#ifndef UNITBLOCK_H
#define UNITBLOCK_H

#include <QWidget>

namespace Ui {
class Unitblock;
}

class Unitblock : public QWidget
{
    Q_OBJECT

public:
    explicit Unitblock(QWidget *parent = nullptr);
    ~Unitblock();
    void initUnit(QString name);
    void initImg(QString name);
    void updateNumber(QString num);

    QString unit_name;

private:
    Ui::Unitblock *ui;
};

#endif // UNITBLOCK_H
