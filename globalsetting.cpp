#include "./globalsetting.h"

#include <iostream>

#include "./layoutsetting.h"

Globalsetting& Globalsetting::getInstance() {
    static Globalsetting instance{};
    return instance;
}

void Globalsetting::loadLayoutSetting(int width, int height, float ratio) {
    l.ratio = ratio;

    l.w                    = width;
    l.h                    = height;
    l.m                    = l.w / 2;
    l.right_x              = l.w - layout::RIGHT_W * ratio;
    l.right_w              = layout::RIGHT_W * ratio;
    l.right_header_h       = layout::RIGHT_HEADER_H * ratio;
    l.right_leftborder_w   = layout::RIGHT_LEFTBORDER_W * ratio;
    l.right_rightborder_w  = layout::RIGHT_RIGHTBORDER_W * ratio;
    l.right_rightborder_x  = layout::RIGHT_RIGHTBORDER_X * ratio;
    l.right_border_h       = layout::RIGHT_BORDER_H * ratio;
    l.right_mapname_h      = layout::RIGHT_MAPNAME_H * ratio;
    l.map_x                = l.w - layout::RIGHT_W * ratio + layout::RIGHT_LEFTBORDER_W * ratio - 1;
    l.map_y                = layout::RIGHT_HEADER_H * ratio - 1;
    l.map_w                = layout::MAP_W * ratio;
    l.map_h                = layout::MAP_H * ratio;
    l.right_bottom_h       = l.h - layout::MAP_H * ratio - l.map_y;
    l.top_m                = l.right_x / 2;
    l.top_w                = 920;
    l.top_h                = 80;
    l.top_playername_y     = layout::TOP_PLAYERNAME_Y;
    l.top_country_x        = layout::TOP_COUNTRY_X;
    l.top_country_y        = layout::TOP_COUNTRY_Y;
    l.top_country_w        = layout::TOP_COUNTRY_W;
    l.top_country_h        = layout::TOP_COUNTRY_H;
    l.top_i_x              = layout::TOP_I_X;
    l.top_i_wh             = layout::TOP_I_WH;
    l.top_ibalance_y       = layout::TOP_IBALANCE_Y;
    l.top_ipower_y         = layout::TOP_IPOWER_Y;
    l.top_balance_x        = layout::TOP_BALANCE_X;
    l.top_balance_y        = layout::TOP_BALANCE_Y;
    l.top_balance_w        = layout::TOP_BALANCE_W;
    l.top_balance_h        = layout::TOP_BALANCE_H;
    l.top_power_x          = layout::TOP_POWER_X;
    l.top_power_y          = layout::TOP_POWER_Y;
    l.top_power_w          = layout::TOP_POWER_W;
    l.top_power_h          = layout::TOP_POWER_H;
    l.if_bar_h             = layout::IF_BAR_H * ratio;
    l.unit_x               = l.right_x + 11 * ratio;
    l.unit_y               = layout::RIGHT_HEADER_H * ratio + layout::MAP_H * ratio + 6 * ratio;
    l.unit_w               = layout::UNIT_W * ratio;
    l.unit_ws              = layout::UNIT_WS * ratio;
    l.unit_h               = layout::UNIT_H * ratio;
    l.unit_hs              = layout::UNIT_HS * ratio;
    l.unit_bg_y            = 40 * ratio;
    l.unit_bg_h            = layout::UNIT_BG_H * ratio;
    l.unitblocks           = l.right_bottom_h / (layout::UNIT_HS * ratio);
    l.unitblock_y          = layout::UNITBLOCK_Y;
    l.icon_side            = 32 * ratio;
    l.bottom_h             = layout::BOTTOM_H * ratio;
    l.bottom_y             = l.h - l.bottom_h;
    l.bottom_y1            = l.bottom_y;
    l.bottom_y2            = l.bottom_y + (layout::BOTTOM_HS + 3) * ratio;
    l.bottom_hs            = layout::BOTTOM_HS * ratio;
    l.bottom_fill_y1       = l.bottom_y1 + 1;
    l.bottom_fill_y2       = l.bottom_y2 + 1;
    l.bottom_fill_h        = layout::BOTTOM_FILL_H * ratio;
    l.bottom_credit_x      = layout::BOTTOM_CREDIT_X;
    l.bottom_credit_y1     = l.bottom_y1;
    l.bottom_credit_y2     = l.bottom_y2;
    l.bottom_credit_w      = layout::BOTTOM_CREDIT_W * ratio;
    l.bottom_credit_h      = layout::BOTTOM_CREDIT_H * ratio;
    l.producingblock_x     = l.top_m + l.top_w / 2 + layout::PRODUCINGBLOCK_X;
    l.producingblock_wh    = 54;
    l.producingblock_ws    = layout::PRODUCINGBLOCK_WS;
    l.producingblock_y1    = layout::PRODUCINGBLOCK_Y1;
    l.producingblock_y2    = layout::PRODUCINGBLOCK_Y2;
    l.producing_img_x      = 5;
    l.producing_img_y      = 5;
    l.producing_img_w      = 44;
    l.producing_img_h      = 35;
    l.producing_status_y   = layout::PRODUCING_STATUS_Y;
    l.producing_number_y   = layout::PRODUCING_NUMBER_Y;
    l.producing_progress_x = layout::PRODUCING_PROGRESS_X;
    l.producing_progress_y = layout::PRODUCING_PROGRESS_Y;
    l.producing_progress_w = layout::PRODUCING_PROGRESS_W;
    l.producing_progress_h = layout::PRODUCING_PROGRESS_H;
    return;
}

QString Globalsetting::findNameByNickname(QString nickname) {
    QString name;

    if (nickname.isEmpty()) {
        return name;
    }

    const QJsonArray& c_player_list = player_list;
    for (auto it : c_player_list) {
        if (it.type() == QJsonValue::Object) {
            QJsonObject p       = it.toObject();
            QString jnickname   = p.value("nickname").toString();
            QString jplayername = p.value("playername").toString();

            if (QString::compare(nickname, jnickname) == 0) {
                name = jplayername;
            }
        }
    }
    return name;
}
