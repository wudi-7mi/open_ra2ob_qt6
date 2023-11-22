#ifndef UNITBLOCK_H
#define UNITBLOCK_H

#include <QWidget>
#include <string>

namespace Ui {
class Unitblock;
}

class Unitblock : public QWidget {
    Q_OBJECT

public:
    explicit Unitblock(QWidget *parent = nullptr);
    ~Unitblock();
    void initUnit(QString name);
    void setName(QString name);
    void setImage(QString name);
    void setNumber(int n);
    void setColor(std::string color);
    void setEmpty();
    QPixmap getRadius(QPixmap src, int radius);

    QString unit_name;

private:
    Ui::Unitblock *ui;
};

#endif  // UNITBLOCK_H
