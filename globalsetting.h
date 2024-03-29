#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QColor>

struct Layout {
    int w;
    int h;
    int unitblocks;
    int m;
    int right_x;
    int map_x;
    int map_y;
    int right_bottom_h;
    int top_m;
    int top_h;
    int top_w;
    int unit_x;
    int unit_y;
    int icon_side;
    int bottom_y;
    int bottom_y1;
    int bottom_y2;
    int bottom_fill_y1;
    int bottom_fill_y2;
    int bottom_credit_y1;
    int bottom_credit_y2;
};

struct Colors {
    float top_panel_opacity = 1;
    QColor producing_stripe = QColor(30, 30, 30);
};

struct Status {
    bool show_all_panel    = true;
    bool show_top_panel    = true;
    bool show_bottom_panel = true;

    bool sc_ctrl_alt_h        = true;
    bool sc_ctrl_alt_pageup   = true;
    bool sc_ctrl_alt_pagedown = true;
    bool sc_ctrl_alt_j        = true;
    bool sc_ctrl_s            = true;
};

class Globalsetting {
public:
    static Globalsetting &getInstance();

    Globalsetting(const Globalsetting &)            = delete;
    Globalsetting &operator=(const Globalsetting &) = delete;

    void loadLayoutSetting(int preset = 1, int width = 0, int height = 0);
    void dumpLayout();

    Layout l;
    Colors c;
    Status s;

protected:
    Globalsetting()  = default;
    ~Globalsetting() = default;
};

#endif  // GLOBALSETTING_H
