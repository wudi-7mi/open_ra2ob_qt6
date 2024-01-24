#ifndef LAYOUTSETTING_H
#define LAYOUTSETTING_H

namespace layout {

// DO NOT CHANGE THIS! THIS IS FIXED!
constexpr int RIGHT_W              = 168;
constexpr int RIGHT_HEADER_H       = 48;
constexpr int RIGHT_LEFTBORDER_W   = 12;
constexpr int RIGHT_RIGHTBORDER_W  = 15;
constexpr int RIGHT_RIGHTBORDER_X  = 153;
constexpr int RIGHT_BORDER_H       = 110;
constexpr int RIGHT_MAPNAME_H      = 12;
constexpr int MAP_W                = 142;
constexpr int MAP_H                = 111;
constexpr int TOP_W                = 920;
constexpr int TOP_H                = 80;
constexpr int IF_BAR_H             = 16;
constexpr int BOTTOM_HS            = 14;
constexpr int BOTTOM_FILL_H        = 12;
constexpr int BOTTOM_CREDIT_X      = 10;
constexpr int BOTTOM_CREDIT_W      = 50;
constexpr int BOTTOM_CREDIT_H      = 14;
constexpr int UNIT_W               = 69;
constexpr int UNIT_WS              = 74;
constexpr int UNIT_H               = 55;
constexpr int UNIT_HS              = 61;
constexpr int UNITBLOCK_Y          = 32;
constexpr int PRODUCINGBLOCK_X     = 20;
constexpr int PRODUCINGBLOCK_WS    = 75;
constexpr int PRODUCINGBLOCK_Y1    = 20;
constexpr int PRODUCINGBLOCK_Y2    = 100;
constexpr int PRODUCING_STATUS_Y   = 15;
constexpr int PRODUCING_NUMBER_Y   = 35;
constexpr int PRODUCING_PROGRESS_X = 8;
constexpr int PRODUCING_PROGRESS_Y = 57;
constexpr int PRODUCING_PROGRESS_H = 8;

// 1920*1080 Screen
constexpr int SC1K_W                = 1920;
constexpr int SC1K_H                = 1080;
constexpr int SC1K_M                = SC1K_W / 2;
constexpr int SC1K_RIGHT_X          = SC1K_W - RIGHT_W;
constexpr int SC1K_MAP_X            = SC1K_W - RIGHT_W + RIGHT_LEFTBORDER_W - 1;
constexpr int SC1K_MAP_Y            = RIGHT_HEADER_H - 1;
constexpr int SC1K_RIGHT_BOTTOM_H   = SC1K_H - MAP_H - SC1K_MAP_Y;
constexpr int SC1K_UNITBLOCKS       = SC1K_RIGHT_BOTTOM_H / UNIT_HS;
constexpr int SC1K_TOP_M            = SC1K_RIGHT_X / 2;
constexpr int SC1K_TOP_H            = 80;
constexpr int SC1K_TOP_W            = 920;
constexpr int SC1K_UNIT_X           = SC1K_RIGHT_X + 11;
constexpr int SC1K_UNIT_Y           = RIGHT_HEADER_H + MAP_H + 6;
constexpr int SC1K_ICON_SIDE        = 64;
constexpr int SC1K_BOTTOM_Y         = SC1K_H - 32;
constexpr int SC1K_BOTTOM_Y1        = SC1K_BOTTOM_Y;
constexpr int SC1K_BOTTOM_Y2        = SC1K_BOTTOM_Y + BOTTOM_HS + 3;
constexpr int SC1K_BOTTOM_FILL_Y1   = SC1K_BOTTOM_Y1 + 1;
constexpr int SC1K_BOTTOM_FILL_Y2   = SC1K_BOTTOM_Y2 + 1;
constexpr int SC1K_BOTTOM_CREDIT_Y1 = SC1K_BOTTOM_Y1;
constexpr int SC1K_BOTTOM_CREDIT_Y2 = SC1K_BOTTOM_Y2;

// 2560*1440 Screen
constexpr int SC2K_W                = 2560;
constexpr int SC2K_H                = 1440;
constexpr int SC2K_M                = SC2K_W / 2;
constexpr int SC2K_RIGHT_X          = SC2K_W - RIGHT_W;
constexpr int SC2K_MAP_X            = SC2K_W - RIGHT_W + RIGHT_LEFTBORDER_W - 1;
constexpr int SC2K_MAP_Y            = RIGHT_HEADER_H - 1;
constexpr int SC2K_RIGHT_BOTTOM_H   = SC2K_H - MAP_H - SC2K_MAP_Y;
constexpr int SC2K_UNITBLOCKS       = SC2K_RIGHT_BOTTOM_H / UNIT_HS;
constexpr int SC2K_TOP_M            = SC2K_RIGHT_X / 2;
constexpr int SC2K_TOP_H            = 80;
constexpr int SC2K_TOP_W            = 920;
constexpr int SC2K_UNIT_X           = SC2K_RIGHT_X + 11;
constexpr int SC2K_UNIT_Y           = RIGHT_HEADER_H + MAP_H + 6;
constexpr int SC2K_ICON_SIDE        = 64;
constexpr int SC2K_BOTTOM_Y         = SC2K_H - 32;
constexpr int SC2K_BOTTOM_Y1        = SC2K_BOTTOM_Y;
constexpr int SC2K_BOTTOM_Y2        = SC2K_BOTTOM_Y + BOTTOM_HS + 3;
constexpr int SC2K_BOTTOM_FILL_Y1   = SC2K_BOTTOM_Y1 + 1;
constexpr int SC2K_BOTTOM_FILL_Y2   = SC2K_BOTTOM_Y2 + 1;
constexpr int SC2K_BOTTOM_CREDIT_Y1 = SC2K_BOTTOM_Y1;
constexpr int SC2K_BOTTOM_CREDIT_Y2 = SC2K_BOTTOM_Y2;

// Style
constexpr char BOTTOM_RADIUS_6[] =
    "border-bottom-left-radius: 6px;border-bottom-right-radius: 6px;";
constexpr char RADIUS_6[] = "border-radius: 6px;";
constexpr char BOTTOM_RADIUS_8[] =
    "border-bottom-left-radius: 8px;border-bottom-right-radius: 8px;";
constexpr char RADIUS_8[] = "border-radius: 8px;";

// Font
constexpr char OPPO_M[] = "OPlusSans 3.0 Medium";
constexpr char OPPO[]   = "OPlusSans 3.0";

// Color
constexpr char COLOR_DEFAULT[] = "262728";

};  // end of namespace layout

#endif  // LAYOUTSETTING_H
