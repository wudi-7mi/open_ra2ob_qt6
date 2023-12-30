#include "./globalsetting.h"

#include <iostream>

#include "./layoutsetting.h"

Globalsetting& Globalsetting::getInstance() {
    static Globalsetting instance{};
    return instance;
}

void Globalsetting::loadSetting(int preset, int width, int height) {
    if (preset == 0) {
        // Todo: calculate layout
        return;
    }

    if (preset == 1) {
        l.w                = layout::SC1K_W;
        l.h                = layout::SC1K_H;
        l.unitblocks       = layout::SC1K_UNITBLOCKS;
        l.m                = layout::SC1K_M;
        l.right_x          = layout::SC1K_RIGHT_X;
        l.map_x            = layout::SC1K_MAP_X;
        l.map_y            = layout::SC1K_MAP_Y;
        l.right_bottom_h   = layout::SC1K_RIGHT_BOTTOM_H;
        l.top_m            = layout::SC1K_TOP_M;
        l.top_h            = layout::SC1K_TOP_H;
        l.top_w            = layout::SC1K_TOP_W;
        l.unit_x           = layout::SC1K_UNIT_X;
        l.unit_y           = layout::SC1K_UNIT_Y;
        l.unit_w           = layout::SC1K_UNIT_W;
        l.unit_h           = layout::SC1K_UNIT_H;
        l.unit_ws          = layout::SC1K_UNIT_Ws;
        l.unit_hs          = layout::SC1K_UNIT_Hs;
        l.icon_side        = layout::SC1K_ICON_SIDE;
        l.bottom_y         = layout::SC1K_BOTTOM_Y;
        l.bottom_y1        = layout::SC1K_BOTTOM_Y1;
        l.bottom_y2        = layout::SC1K_BOTTOM_Y2;
        l.bottom_fill_y1   = layout::SC1K_BOTTOM_FILL_Y1;
        l.bottom_fill_y2   = layout::SC1K_BOTTOM_FILL_Y2;
        l.bottom_credit_y1 = layout::SC1K_BOTTOM_CREDIT_Y1;
        l.bottom_credit_y2 = layout::SC1K_BOTTOM_CREDIT_Y2;
    }

    if (preset == 2) {
        l.w                = layout::SC2K_W;
        l.h                = layout::SC2K_H;
        l.unitblocks       = layout::SC2K_UNITBLOCKS;
        l.m                = layout::SC2K_M;
        l.right_x          = layout::SC2K_RIGHT_X;
        l.map_x            = layout::SC2K_MAP_X;
        l.map_y            = layout::SC2K_MAP_Y;
        l.right_bottom_h   = layout::SC2K_RIGHT_BOTTOM_H;
        l.top_m            = layout::SC2K_TOP_M;
        l.top_h            = layout::SC2K_TOP_H;
        l.top_w            = layout::SC2K_TOP_W;
        l.unit_x           = layout::SC2K_UNIT_X;
        l.unit_y           = layout::SC2K_UNIT_Y;
        l.unit_w           = layout::SC2K_UNIT_W;
        l.unit_h           = layout::SC2K_UNIT_H;
        l.unit_ws          = layout::SC2K_UNIT_Ws;
        l.unit_hs          = layout::SC2K_UNIT_Hs;
        l.icon_side        = layout::SC2K_ICON_SIDE;
        l.bottom_y         = layout::SC2K_BOTTOM_Y;
        l.bottom_y1        = layout::SC2K_BOTTOM_Y1;
        l.bottom_y2        = layout::SC2K_BOTTOM_Y2;
        l.bottom_fill_y1   = layout::SC2K_BOTTOM_FILL_Y1;
        l.bottom_fill_y2   = layout::SC2K_BOTTOM_FILL_Y2;
        l.bottom_credit_y1 = layout::SC2K_BOTTOM_CREDIT_Y1;
        l.bottom_credit_y2 = layout::SC2K_BOTTOM_CREDIT_Y2;
    }

    dumpLayout();
}

void Globalsetting::dumpLayout() {
    std::cout << "\nw              : " << l.w;
    std::cout << "\nh              : " << l.h;
    std::cout << "\nunitblocks     : " << l.unitblocks;
    std::cout << "\nm              : " << l.m;
    std::cout << "\nright_x        : " << l.right_x;
    std::cout << "\nmap_x          : " << l.map_x;
    std::cout << "\nmap_y          : " << l.map_y;
    std::cout << "\nright_bottom_h : " << l.right_bottom_h;
    std::cout << "\ntop_m          : " << l.top_m;
    std::cout << "\ntop_h          : " << l.top_h;
    std::cout << "\ntop_w          : " << l.top_w;
    std::cout << "\nunit_x         : " << l.unit_x;
    std::cout << "\nunit_y         : " << l.unit_y;
    std::cout << "\nunit_w         : " << l.unit_w;
    std::cout << "\nunit_h         : " << l.unit_h;
    std::cout << "\nunit_ws        : " << l.unit_ws;
    std::cout << "\nunit_hs        : " << l.unit_hs;
    std::cout << "\nicon_side      : " << l.icon_side;
}
