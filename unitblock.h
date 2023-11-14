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
    void initUnit(QString name, int index);
    void initImg(QString name);
    void updateNumber(QString num);
    void setColor(QString color);

    QString unit_name;
    int player_index;

private:
    Ui::Unitblock *ui;
};

#endif // UNITBLOCK_H
