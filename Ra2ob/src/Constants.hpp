#ifndef RA2OB_SRC_CONSTANTS_HPP_
#define RA2OB_SRC_CONSTANTS_HPP_

#include <map>
#include <string>

namespace Ra2ob {

// Basic game information

constexpr int MAXPLAYER    = 8;
constexpr int GAMESPEED    = 59;
constexpr int INVALIDCLASS = 0xffffffffu;

// Memory size

constexpr int BOOLSIZE        = 0x1;
constexpr int NUMSIZE         = 0x4;
constexpr int PTRSIZE         = 0x4;
constexpr int COLORSIZE       = 0x6;
constexpr int STRNAMESIZE     = 0x20;
constexpr int STRCOUNTRYSIZE  = 0x19;
constexpr int STRUNITNAMESIZE = 0x20;

// Basic offsets

constexpr int GAMEFRAMEOFFSET          = 0xa8ed84;
constexpr int FIXEDOFFSET              = 0xa8b230;
constexpr int CLASSBASEARRAYOFFSET     = 0xa8022c;
constexpr int PLAYERBASEARRAYPTROFFSET = 0x1180;

constexpr int HOUSETYPEOFFSET        = 0x34;
constexpr int COLOROFFSET            = 0x56F9;
constexpr int INFANTRYSELFHEALOFFSET = 0x164;
constexpr int UNITSELFHEALOFFSET     = 0x168;
constexpr int CURRENTPLAYEROFFSET    = 0x1ec;
constexpr int ISDEFEATEDOFFSET       = 0x1f5;
constexpr int ISGAMEOVEROFFSET       = 0x1f6;
constexpr int ISWINNEROFFSET         = 0x1f7;

constexpr int BUILDINGOFFSET = 0x5554;
constexpr int TANKOFFSET     = 0x5568;
constexpr int INFANTRYOFFSET = 0x557c;
constexpr int AIRCRAFTOFFSET = 0x5590;

constexpr int STRNAMEOFFSET    = 0x1602a;
constexpr int STRCOUNTRYOFFSET = 0x24;

constexpr int WINOFFSET  = 0x1f7;
constexpr int LOSEOFFSET = 0x1f8;

constexpr int UNITSAFE = 4096;

// Production Offsets

constexpr int P_AIRCRAFTOFFSET       = 0x53AC;
constexpr int P_BUILDINGFIRSTOFFSET  = 0x53BC;
constexpr int P_BUILDINGSECONDOFFSET = 0x53CC;
constexpr int P_INFANTRYOFFSET       = 0x53B0;
constexpr int P_TANKOFFSET           = 0x53B4;
constexpr int P_SHIPOFFSET           = 0x53B8;

constexpr int P_TIMEOFFSET        = 0x24;
constexpr int P_STATUSOFFSET      = 0x70;
constexpr int P_CURRENTOFFSET     = 0x58;
constexpr int P_QUEUEPTROFFSET    = 0x44;
constexpr int P_QUEUELENGTHOFFSET = 0x50;

constexpr int P_INFANTRYTYPEOFFSET = 0x6C0;
constexpr int P_BUILDINGTYPEOFFSET = 0x520;
constexpr int P_UNITTYPEOFFSET     = 0x6C4;

constexpr int P_ARRAYINDEXOFFSET = 0xDF8;

// Time Ints

constexpr int T_DETECTTIME = 1000;
constexpr int T_FETCHTIME  = 500;
constexpr int T_PRINTTIME  = 500;

// Color Codes

constexpr int C_YELLOW   = 0xE0D838;
constexpr int C_YELLOW2  = 0xF0F840;
constexpr int C_PURPLE   = 0x9848B8;
constexpr int C_GREEN    = 0x58CC50;
constexpr int C_RED      = 0xF84C48;
constexpr int C_ORANGE   = 0xF8B048;
constexpr int C_PINK     = 0xF8ACE8;
constexpr int C_SKYBLUE  = 0x58D4E0;
constexpr int C_SKYBLUE2 = 0x58D0E0;
constexpr int C_BLUE     = 0x487CC8;

// Console Color Codes

constexpr char STYLE_OFF[]       = "\033[0m";
constexpr char STYLE_BLACK[]     = "\033[30m";
constexpr char STYLE_DARKBLUE[]  = "\033[34m";
constexpr char STYLE_DARKGREEN[] = "\033[32m";
constexpr char STYLE_LIGHTBLUE[] = "\033[36m";
constexpr char STYLE_DARKRED[]   = "\033[31m";
constexpr char STYLE_PURPLE[]    = "\033[35m";
constexpr char STYLE_ORANGE[]    = "\033[33m";
constexpr char STYLE_LIGHTGRAY[] = "\033[37m";
constexpr char STYLE_GRAY[]      = "\033[90m";
constexpr char STYLE_BLUE[]      = "\033[94m";
constexpr char STYLE_GREEN[]     = "\033[92m";
constexpr char STYLE_CYAN[]      = "\033[96m";
constexpr char STYLE_RED[]       = "\033[91m";
constexpr char STYLE_PINK[]      = "\033[95m";
constexpr char STYLE_YELLOW[]    = "\033[93m";
constexpr char STYLE_WHITE[]     = "\033[97m";

constexpr char STYLE_BG_BLACK[]     = "\033[40m";
constexpr char STYLE_BG_DARKBLUE[]  = "\033[44m";
constexpr char STYLE_BG_DARKGREEN[] = "\033[42m";
constexpr char STYLE_BG_LIGHTBLUE[] = "\033[46m";
constexpr char STYLE_BG_DARKRED[]   = "\033[41m";
constexpr char STYLE_BG_PURPLE[]    = "\033[45m";
constexpr char STYLE_BG_ORANGE[]    = "\033[43m";
constexpr char STYLE_BG_LIGHTGRAY[] = "\033[47m";
constexpr char STYLE_BG_GRAY[]      = "\033[100m";
constexpr char STYLE_BG_BLUE[]      = "\033[104m";
constexpr char STYLE_BG_GREEN[]     = "\033[102m";
constexpr char STYLE_BG_CYAN[]      = "\033[106m";
constexpr char STYLE_BG_RED[]       = "\033[101m";
constexpr char STYLE_BG_PINK[]      = "\033[105m";
constexpr char STYLE_BG_YELLOW[]    = "\033[103m";
constexpr char STYLE_BG_WHITE[]     = "\033[107m";

const std::map<int, std::string> COLORMAP = {
    {C_YELLOW, STYLE_BG_YELLOW},     {C_YELLOW2, STYLE_BG_YELLOW},
    {C_PURPLE, STYLE_BG_PURPLE},     {C_GREEN, STYLE_BG_GREEN},
    {C_RED, STYLE_BG_RED},           {C_ORANGE, STYLE_BG_ORANGE},
    {C_PINK, STYLE_BG_PINK},         {C_SKYBLUE2, STYLE_BG_LIGHTBLUE},
    {C_SKYBLUE, STYLE_BG_LIGHTBLUE}, {C_BLUE, STYLE_BG_DARKBLUE},
};

// Country Map

const std::map<std::string, std::string> COUNTRYMAP = {
    {"Americans", "Americans"}, {"Alliance", "Korea"},    {"French", "France"},
    {"Germans", "Germans"},     {"British", "British"},   {"Africans", "Libya"},
    {"Arabs", "Iraq"},          {"Russians", "Russians"}, {"Confederation", "Cuba"},
    {"YuriCountry", "Yuri"},
};

// Enums

enum class UnitType : int { Building = 4, Tank = 3, Infantry = 2, Aircraft = 1, Unknown = 0 };
enum class Version : int { Yr = 1, Ra2 = 0 };

// Files

constexpr char F_PANELOFFSETS[] = "./config/panel_offsets.json";
constexpr char F_UNITOFFSETS[]  = "./config/unit_offsets.json";

// Strings

constexpr char STR_RULER[] = "=====";
constexpr int RULER_MULT   = 20;

// Limitation

constexpr bool GOODINTENTION = false;

}  // end of namespace Ra2ob

#endif  // RA2OB_SRC_CONSTANTS_HPP_
