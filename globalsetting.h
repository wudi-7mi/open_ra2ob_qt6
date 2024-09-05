#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QColor>
#include <QJsonArray>
#include <QJsonObject>

struct Layout {
    float ratio;
    int w;
    int h;
    int m;
    int right_x;
    int right_w;
    int right_header_h;
    int right_leftborder_w;
    int right_rightborder_w;
    int right_rightborder_x;
    int right_border_h;
    int right_mapname_h;
    int map_x;
    int map_y;
    int map_w;
    int map_h;
    int right_bottom_h;
    int top_m;
    int top_w;
    int top_h;
    int top_playername_y;
    int top_country_x;
    int top_country_y;
    int top_country_w;
    int top_country_h;
    int top_i_x;
    int top_i_wh;
    int top_ibalance_y;
    int top_ipower_y;
    int top_balance_x;
    int top_balance_y;
    int top_balance_w;
    int top_balance_h;
    int top_power_x;
    int top_power_y;
    int top_power_w;
    int top_power_h;
    int if_bar_h;
    int unit_x;
    int unit_y;
    int unit_w;
    int unit_ws;
    int unit_h;
    int unit_hs;
    int unit_bg_y;
    int unit_bg_h;
    int unitblocks;
    int unitblock_y;
    int icon_side;
    int bottom_h;
    int bottom_y;
    int bottom_y1;
    int bottom_y2;
    int bottom_hs;
    int bottom_fill_y1;
    int bottom_fill_y2;
    int bottom_fill_h;
    int bottom_credit_x;
    int bottom_credit_y1;
    int bottom_credit_y2;
    int bottom_credit_w;
    int bottom_credit_h;
    int producingblock_x;
    int producingblock_w;
    int producingblock_h;
    int producingblock_ws;
    int producingblock_hs;
    int producingblock_y1;
    int producingblock_y2;
    int producingblock_space;
    int producing_img_x;
    int producing_img_y;
    int producing_img_w;
    int producing_img_h;
    int producing_status_y;
    int producing_number_y;
    int producing_progress_x;
    int producing_progress_y;
    int producing_progress_w;
    int producing_progress_h;
};

struct Colors {
    float top_panel_opacity       = 1;
    QColor producing_stripe_color = QColor(30, 30, 30);
    QColor sidepanel_color        = QColor(25, 25, 112);
    QColor preview_color          = QColor("red");
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

struct ScoreBoard {
    QString p1_nickname, p1_playername;
    QString p2_nickname, p2_playername;
    int p1_score = 0;
    int p2_score = 0;
};

struct GameLog {
    int game_index;
    QString p1_nickname, p2_nickname;
    int game_duration;
    int game_start_time;
};

class Globalsetting {
public:
    static Globalsetting &getInstance();

    Globalsetting(const Globalsetting &)            = delete;
    Globalsetting &operator=(const Globalsetting &) = delete;

    void loadLayoutSetting(int width = 0, int height = 0, float ratio = 1);
    QString findNameByNickname(QString nickname);

    Layout l;
    Colors c;
    Status s;
    ScoreBoard sb;
    QJsonArray player_list;
    int game_numbers = 0;
    bool game_start  = false;
    bool game_end    = false;
    QList<GameLog> game_log;
    QString buildingQueuePosition = "Left";

protected:
    Globalsetting()  = default;
    ~Globalsetting() = default;
};

#endif  // GLOBALSETTING_H
