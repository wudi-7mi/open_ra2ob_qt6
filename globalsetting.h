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
    int unit_w;
    int unit_h;
    int unit_ws;
    int unit_hs;
    int icon_side;
};

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
