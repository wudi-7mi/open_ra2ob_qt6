#ifndef LAYOUTSETTING_H
#define LAYOUTSETTING_H

namespace layout {

// DO NOT CHANGE THIS! THIS IS FIXED!
constexpr int RIGHT_W                 = 168;
constexpr int RIGHT_HEADER_H          = 48;
constexpr int RIGHT_LEFTBORDER_W      = 12;
constexpr int RIGHT_RIGHTBORDER_W     = 15;
constexpr int RIGHT_RIGHTBORDER_START = 153;
constexpr int RIGHT_BORDER_H          = 110;
constexpr int MAP_W                   = 142;
constexpr int MAP_H                   = 111;
constexpr int IF_BAR_H                = 16;

// 1920*1080 Screen
constexpr int SC1K_W              = 1920;
constexpr int SC1K_H              = 1080;
constexpr int SC1K_UNITBLOCKS     = 15;
constexpr int SC1K_M              = SC1K_W / 2;
constexpr int SC1K_RIGHT_X        = SC1K_W - RIGHT_W;
constexpr int SC1K_MAP_X          = SC1K_W - RIGHT_W + RIGHT_LEFTBORDER_W - 1;
constexpr int SC1K_MAP_Y          = RIGHT_HEADER_H - 1;
constexpr int SC1K_RIGHT_BOTTOM_H = SC1K_H - MAP_H - SC1K_MAP_Y;
constexpr int SC1K_TOP_M          = SC1K_RIGHT_X / 2;
constexpr int SC1K_TOP_H          = 90;
constexpr int SC1K_TOP_W          = 920;
constexpr int SC1K_UNIT_X         = SC1K_RIGHT_X + 11;
constexpr int SC1K_UNIT_Y         = RIGHT_HEADER_H + MAP_H + 6;
constexpr int SC1K_UNIT_W         = 69;
constexpr int SC1K_UNIT_H         = 55;
constexpr int SC1K_UNIT_Ws        = SC1K_UNIT_W + 5;
constexpr int SC1K_UNIT_Hs        = SC1K_UNIT_H + 6;
constexpr int SC1K_ICON_SIDE      = 64;

// 2560*1440 Screen
constexpr int SC2K_W              = 2560;
constexpr int SC2K_H              = 1440;
constexpr int SC2K_UNITBLOCKS     = 21;
constexpr int SC2K_M              = SC2K_W / 2;
constexpr int SC2K_RIGHT_X        = SC2K_W - RIGHT_W;
constexpr int SC2K_MAP_X          = SC2K_W - RIGHT_W + RIGHT_LEFTBORDER_W - 1;
constexpr int SC2K_MAP_Y          = RIGHT_HEADER_H - 1;
constexpr int SC2K_RIGHT_BOTTOM_H = SC2K_H - MAP_H - SC2K_MAP_Y;
constexpr int SC2K_TOP_M          = SC2K_RIGHT_X / 2;
constexpr int SC2K_TOP_H          = 90;
constexpr int SC2K_TOP_W          = 920;
constexpr int SC2K_UNIT_X         = SC2K_RIGHT_X + 11;
constexpr int SC2K_UNIT_Y         = RIGHT_HEADER_H + MAP_H + 6;
constexpr int SC2K_UNIT_W         = 69;
constexpr int SC2K_UNIT_H         = 55;
constexpr int SC2K_UNIT_Ws        = SC2K_UNIT_W + 5;
constexpr int SC2K_UNIT_Hs        = SC2K_UNIT_H + 6;
constexpr int SC2K_ICON_SIDE      = 64;

// Style
constexpr char BOTTOM_RADIUS_6[] =
    "border-bottom-left-radius: 6px;border-bottom-right-radius: 6px;";
constexpr char RADIUS_6[] = "border-radius: 6px;";
constexpr char BOTTOM_RADIUS_8[] =
    "border-bottom-left-radius: 8px;border-bottom-right-radius: 8px;";
constexpr char RADIUS_8[] = "border-radius: 8px;";

// Color
constexpr char COLOR_DEFAULT[] = "262728";

};  // end of namespace layout

#endif  // LAYOUTSETTING_H
