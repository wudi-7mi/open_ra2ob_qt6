#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

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
    float top_panel_opacity = 0.5;
};

struct Status {};

class Globalsetting {
public:
    static Globalsetting &getInstance();

    Globalsetting(const Globalsetting &)            = delete;
    Globalsetting &operator=(const Globalsetting &) = delete;

    void loadSetting(int preset = 1, int width = 0, int height = 0);
    void dumpLayout();

    Layout l;

protected:
    Globalsetting()  = default;
    ~Globalsetting() = default;
};

#endif  // GLOBALSETTING_H
