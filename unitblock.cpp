#include "./unitblock.h"

#include <QFile>
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>

#include "./layoutsetting.h"
#include "./ui_unitblock.h"

Unitblock::Unitblock(QWidget *parent) : QWidget(parent), ui(new Ui::Unitblock) {
    ui->setupUi(this);
    lb_num = new QOutlineLabel(this);

    gls = &Globalsetting::getInstance();

    rearrange();
}

Unitblock::~Unitblock() {
    delete lb_num;
    delete ui;
}

void Unitblock::initUnit(QString name) {
    unit_name = name;
    setImage(name);
}

void Unitblock::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    //    QPen lBorder(QColor(20,20,20));
    //    lBorder.setWidth(2);
    //    painter.setPen(lBorder);

    //    QString text = lb_num->text();
    //    QFont font = lb_num->font();

    //    QFontMetrics fm(font);
    //    QSize ts = fm.size(Qt::TextSingleLine, text);

    //    painter.drawRect(QRect(lb_num->x(), lb_num->y() - 10, ts.width(), ts.height()));

    painter.end();
}

void Unitblock::setName(QString name) {
    unit_name = name;
    setImage(name);
}

void Unitblock::setImage(QString name) {
    QString img_str = ":/obicons/assets/obicons/" + name + ".png";

    QFile qf(img_str);

    int w = gls->l.unit_w;
    int h = gls->l.unit_h;

    if (!qf.exists()) {
        QPixmap p = QPixmap(":/obicons/assets/obicons/unit_placeholder_trans.png");
        p         = p.scaled(w, h);
        ui->img->setPixmap(p);
        ui->img->setGeometry(0, 0, w, h);
        return;
    }

    QPixmap p = QPixmap(img_str);

    p = p.scaled(w, h);
    ui->img->setPixmap(getRadius(p, 8 * gls->l.ratio));
    ui->img->setGeometry(0, 0, w, h);
}

QPixmap Unitblock::getRadius(QPixmap src, int radius) {
    int w = src.width();
    int h = src.height();

    QPixmap dest(w, h);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);

    QPainterPath path;
    QRect rect(0, 0, w, h);
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, w, h, src);

    painter.end();

    return dest;
}

void Unitblock::setNumber(int n) {
    QFont font;
    font.setFamily(layout::OPPO_M);
    font.setPointSize(11 * gls->l.ratio);

    lb_num->setFont(font);
    lb_num->setOutline(Qt::white, QColor(30, 27, 24), 2, true);
    lb_num->setText(QString::number(n));
    lb_num->adjustSize();
    int cX = (this->width() - lb_num->width()) / 2;
    int cY = gls->l.unit_bg_y + gls->l.unit_bg_h - lb_num->height();

    lb_num->setGeometry(cX, cY - 1, lb_num->width(), lb_num->height());
    lb_num->show();

    ui->bg->show();
}

void Unitblock::setColor(std::string color) {
    QString q_color = QString::fromStdString("#" + color);

    QString rad = QString::number(8 * gls->l.ratio);

    ui->bg->setStyleSheet("background-color: " + q_color + ";" + "border-bottom-left-radius:" +
                          rad + "px;border-bottom-right-radius:" + rad + "px;");
    ui->border->setStyleSheet("border: 1px solid " + q_color + ";" + "border-radius: " + rad +
                              "px;");
}

void Unitblock::setEmpty() {
    unit_name       = "";
    QString img_str = ":/obicons/assets/obicons/unit_placeholder_trans.png";
    ui->img->setPixmap(QPixmap(img_str));

    lb_num->hide();
    ui->bg->hide();
}

void Unitblock::rearrange() {
    ui->bg->setGeometry(0, gls->l.unit_bg_y, gls->l.unit_w, gls->l.unit_bg_h);
    ui->border->setGeometry(0, 0, gls->l.unit_w, gls->l.unit_h);
    ui->img->setGeometry(0, 0, gls->l.unit_w, gls->l.unit_h);
}
