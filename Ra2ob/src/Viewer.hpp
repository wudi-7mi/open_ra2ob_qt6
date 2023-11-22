#ifndef RA2OB_SRC_VIEWER_HPP_
#define RA2OB_SRC_VIEWER_HPP_

#include <sstream>
#include <string>
#include <vector>

#include "./Datatypes.hpp"
#include "./json.hpp"

using json = nlohmann::json;

namespace Ra2ob {

class Viewer {
public:
    Viewer();
    ~Viewer();

    json exportJson(tagGameInfo gi, int mode = 0);
    void print(tagGameInfo gi, int mode = 0, int indent = 0);
    std::string uint32ToHex(uint32_t num);
    std::vector<std::string> vecToHex(std::vector<uint32_t> source);
};

inline Viewer::Viewer() {}

/**
 * mode 0 - brief, 1 - full, 2 - debug
 */
inline json Viewer::exportJson(tagGameInfo gi, int mode) {
    json j;

    if (mode > 0) {
        j["debug"]["playerBase"]   = vecToHex(gi.debug.playerBase);
        j["debug"]["buildingBase"] = vecToHex(gi.debug.buildingBase);
        j["debug"]["infantryBase"] = vecToHex(gi.debug.infantryBase);
        j["debug"]["tankBase"]     = vecToHex(gi.debug.tankBase);
        j["debug"]["aircraftBase"] = vecToHex(gi.debug.aircraftBase);
        j["debug"]["houseType"]    = vecToHex(gi.debug.houseType);
    }

    if (mode == 2) {
        return j;
    }

    for (auto& p : gi.players) {
        json jp;

        if (mode == 0 && !p.valid) {
            continue;
        }

        jp["panel"]["playerName"]  = p.panel.playerNameUtf;
        jp["panel"]["balance"]     = p.panel.balance;
        jp["panel"]["creditSpent"] = p.panel.creditSpent;
        jp["panel"]["powerDrain"]  = p.panel.powerDrain;
        jp["panel"]["powerOutput"] = p.panel.powerOutput;
        jp["panel"]["color"]       = p.panel.color;
        jp["panel"]["country"]     = p.panel.country;

        for (auto& u : p.units.units) {
            json ju;

            if (mode == 0 && u.num == 0) {
                continue;
            }

            ju["unitName"] = u.unitName;
            ju["num"]      = u.num;

            if (mode == 1) {
                ju["index"] = u.index;
            }

            jp["units"].push_back(ju);
        }

        // Todo: Add buildings.

        j["players"].push_back(jp);
    }

    return j;
}

/**
 * mode 0 - brief, 1 - full, 2 - debug
 */
inline void Viewer::print(tagGameInfo gi, int mode, int indent) {
    if (mode == 2) {
        json j;

        j["debug"]["playerBase"]   = vecToHex(gi.debug.playerBase);
        j["debug"]["buildingBase"] = vecToHex(gi.debug.buildingBase);
        j["debug"]["infantryBase"] = vecToHex(gi.debug.infantryBase);
        j["debug"]["tankBase"]     = vecToHex(gi.debug.tankBase);
        j["debug"]["aircraftBase"] = vecToHex(gi.debug.aircraftBase);
        j["debug"]["houseType"]    = vecToHex(gi.debug.houseType);

        std::cout << j.dump() << std::endl;
        return;
    }

    for (auto& p : gi.players) {
        if (mode == 0 && !p.valid) {
            continue;
        }

        int cval = std::stoi(p.panel.color, 0, 16);

        std::cout << p.panel.playerName;

        if (COLORMAP.find(cval) == COLORMAP.end()) {
            std::cout << " ";
        } else {
            std::cout << " " << COLORMAP.at(cval) << "  " << STYLE_OFF;
        }

        std::cout << " " << p.panel.country;
        std::cout << " Balance: " << p.panel.balance;
        std::cout << " Power: " << p.panel.powerDrain << " / " << p.panel.powerOutput;
        std::cout << " Credit: " << p.panel.creditSpent;

        std::cout << "\n";

        for (auto& u : p.units.units) {
            if (mode == 0 && u.num == 0) {
                continue;
            }

            std::cout << u.unitName << ": " << u.num;

            if (mode == 1) {
                std::cout << " index=" << u.index;
            }

            std::cout << "\n";
        }

        // Todo: Add buildings.

        std::cout << STR_RULER << std::endl;
    }
}

inline std::string Viewer::uint32ToHex(uint32_t num) {
    std::stringstream ss;
    ss << std::hex << num;
    return ss.str();
}

inline std::vector<std::string> Viewer::vecToHex(std::vector<uint32_t> source) {
    std::vector<std::string> ret;

    for (auto& it : source) {
        ret.push_back(uint32ToHex(it));
    }

    return ret;
}

inline Viewer::~Viewer() {}

}  // namespace Ra2ob

#endif  // RA2OB_SRC_VIEWER_HPP_
