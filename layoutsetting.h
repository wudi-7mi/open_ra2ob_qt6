#ifndef LAYOUTSETTING_H
#define LAYOUTSETTING_H

namespace layout {

// DO NOT CHANGE THIS! THIS IS FIXED!
const int RIGHT_W                 = 168;
const int RIGHT_HEADER_H          = 48;
const int RIGHT_LEFTBORDER_W      = 12;
const int RIGHT_RIGHTBORDER_W     = 15;
const int RIGHT_RIGHTBORDER_START = 153;
const int RIGHT_BORDER_H          = 110;
const int MAP_W                   = 142;
const int MAP_H                   = 111;

// 1920*1080 Screen
const int SC1K_UNITBLOCKS     = 15;
const int SC1K_M              = 1920 / 2;
const int SC1K_RIGHT_X        = 1920 - RIGHT_W;
const int SC1K_RIGHT_BOTTOM_H = 922;
const int SC1K_MAP_X          = 1763;
const int SC1K_MAP_Y          = 47;
const int SC1K_TOP_M          = SC1K_RIGHT_X / 2;
const int SC1K_TOP_H          = 90;
const int SC1K_TOP_W          = 920;
const int SC1K_UNIT_X         = SC1K_RIGHT_X + 11;
const int SC1K_UNIT_Y         = 163;
const int SC1K_UNIT_W         = 69;
const int SC1K_UNIT_H         = 55;
const int SC1K_UNIT_Ws        = SC1K_UNIT_W + 5;
const int SC1K_UNIT_Hs        = SC1K_UNIT_H + 6;
const int SC1K_ICON_SIDE      = 64;

// Style
const char BOTTOM_RADIUS_6[] = "border-bottom-left-radius: 6px;border-bottom-right-radius: 6px;";
const char RADIUS_6[]        = "border-radius: 6px;";
const char BOTTOM_RADIUS_8[] = "border-bottom-left-radius: 8px;border-bottom-right-radius: 8px;";
const char RADIUS_8[]        = "border-radius: 8px;";

// Color
const char COLOR_DEFAULT[] = "262728";

};  // end of namespace layout

#endif  // LAYOUTSETTING_H
