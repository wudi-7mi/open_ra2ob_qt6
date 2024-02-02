#include "./mainwindow.h"

#include <QCloseEvent>
#include <QHideEvent>
#include <QPainter>
#include <QProcess>

#include "./configmanager.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, ConfigManager *cfgm)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->General->installEventFilter(this);

    this->setWindowIcon(QIcon(":/icon/assets/icons/icon_16.png"));
    this->setWindowTitle(tr("open_ra2ob - An Opensource Ra2 observer"));
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);

    detectShortcutStatus();

    ui->lb_github->setText("<a href=\"https://github.com/wudi-7mi/open_ra2ob_qt6\">" +
                           tr("Github") + "</a>");
    ui->lb_feedback->setText("<a href=\"https://txc.qq.com/products/627191\">" + tr("Feedback") +
                             "</a>");
    ui->lb_bilibili->setText("<a href=\"https://space.bilibili.com/1361231649\">" +
                             tr("My bilibili") + "</a>");

    gls = &Globalsetting::getInstance();

    _cfgm = cfgm;
    if (cfgm == nullptr) {
        _cfgm = new ConfigManager();
        _cfgm->checkConfig();
    }

    initLanguage(_cfgm->getLanguage());
    ui->btn_reload->hide();
    connect(ui->rb_English, &QRadioButton::toggled, this, &MainWindow::onRbEnglishClicked);
    connect(ui->rb_Chinese, &QRadioButton::toggled, this, &MainWindow::onRbChineseClicked);
    connect(ui->btn_reload, &QPushButton::clicked, this, &MainWindow::onBtnReloadClicked);

    ui->hs_opacity->setMaximum(10);
    ui->hs_opacity->setMinimum(0);
    ui->hs_opacity->setSingleStep(1);
    ui->hs_opacity->setPageStep(1);
    ui->hs_opacity->setValue((_cfgm->getOpacity() * 10) / 1);
    connect(ui->hs_opacity, SIGNAL(valueChanged(int)), this, SLOT(onOpacityChanged(int)));

    QFont f(layout::OPPO_M, 10);
    ui->lb_author->setFont(f);
    ui->lb_bilibili->setFont(f);
    ui->lb_feedback->setFont(f);
    ui->lb_github->setFont(f);
    ui->lb_name->setFont(f);
    ui->lb_version->setFont(f);

    cd = new QColorDialog();
    cd->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);
    gls->c.sidepanel_color = _cfgm->getSidepanelColor();
    connect(cd, &QColorDialog::currentColorChanged, this, &MainWindow::onColorChanged);
    connect(ui->btn_sidebar, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btn_sidebar_reset, &QPushButton::clicked, this,
            &MainWindow::onSidebarResetButtonClicked);

    tray = new Tray(this);
    connect(tray, SIGNAL(showSetting()), this, SLOT(showSetting()));
    connect(tray, SIGNAL(quit()), this, SLOT(quit()));
    tray->setupTray();

    ob = new Ob();
}

void MainWindow::detectShortcutStatus() {
    Globalsetting &gls = Globalsetting::getInstance();

    ui->lb_status_1->setText(tr("Normal"));
    ui->lb_status_1->setStyleSheet("color:green;");

    ui->lb_status_2->setText(tr("Normal"));
    ui->lb_status_2->setStyleSheet("color:green;");

    ui->lb_status_3->setText(tr("Normal"));
    ui->lb_status_3->setStyleSheet("color:green;");

    ui->lb_status_4->setText(tr("Normal"));
    ui->lb_status_4->setStyleSheet("color:green;");

    ui->lb_status_5->setText(tr("Normal"));
    ui->lb_status_5->setStyleSheet("color:green;");

    if (!gls.s.sc_ctrl_alt_h) {
        ui->lb_status_1->setText(tr("Occupied"));
        ui->lb_status_1->setStyleSheet("color:red;");
    }
    if (!gls.s.sc_ctrl_alt_pageup) {
        ui->lb_status_2->setText(tr("Occupied"));
        ui->lb_status_2->setStyleSheet("color:red;");
    }
    if (!gls.s.sc_ctrl_alt_pagedown) {
        ui->lb_status_3->setText(tr("Occupied"));
        ui->lb_status_3->setStyleSheet("color:red;");
    }
    if (!gls.s.sc_ctrl_alt_j) {
        ui->lb_status_4->setText(tr("Occupied"));
        ui->lb_status_4->setStyleSheet("color:red;");
    }
}

void MainWindow::drawPreview(QWidget *widget) {
    QPainter painter(widget);

    QImage image(":/setting/assets/setting/opacitypreview.png");
    painter.drawImage(ui->lb_preview->geometry(), image);
    int px = ui->lb_preview->x();
    int py = ui->lb_preview->y();
    int pw = ui->lb_preview->width() / 2;
    int ph = ui->lb_preview->height();

    QRect r(px, py, pw, ph);
    painter.setOpacity(gls->c.top_panel_opacity);
    painter.fillRect(r, gls->c.preview_color);
    painter.setOpacity(1);

    QRect r_bottom(px, py + ph, pw * 2 + 1, 26);
    painter.fillRect(r_bottom, gls->c.sidepanel_color);
    update();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Paint) {
        if (ui->tabWidget->currentWidget() == ui->General) {
            drawPreview(ui->General);
        }
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::initLanguage(QString language) {
    if (language == "zh_CN") {
        ui->rb_Chinese->setChecked(true);
        return;
    }
    ui->rb_English->setChecked(true);
}

void MainWindow::onRbEnglishClicked() {
    ui->btn_reload->show();
    _cfgm->setLanguage("en_US");
}

void MainWindow::onRbChineseClicked() {
    ui->btn_reload->show();
    _cfgm->setLanguage("zh_CN");
}

void MainWindow::onBtnReloadClicked() { qApp->exit(773); }

void MainWindow::onOpacityChanged(int opacity) {
    float value = opacity * 1.0 / 10;

    _cfgm->setOpacity(value);
    gls->c.top_panel_opacity = value;
}

void MainWindow::onSidebarButtonClicked() { cd->show(); }

void MainWindow::onSidebarResetButtonClicked() {
    QColor color("midnightblue");
    _cfgm->setSidepanelColor(color);
    gls->c.sidepanel_color = color;
    ui->btn_sidebar_reset->setDisabled(true);
}

void MainWindow::onColorChanged(const QColor &color) {
    _cfgm->setSidepanelColor(color);
    gls->c.sidepanel_color = color;
    ui->btn_sidebar_reset->setDisabled(false);
}

void MainWindow::showSetting() {
    this->showNormal();
    this->activateWindow();
}

void MainWindow::quit() { qApp->quit(); }

void MainWindow::hideEvent(QHideEvent *event) {
    this->hide();
    event->ignore();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    this->hide();
    event->ignore();
}

MainWindow::~MainWindow() { delete ui; }
