#ifndef RA2OB_SRC_VIEWER_HPP_
#define RA2OB_SRC_VIEWER_HPP_

#include <sstream>
#include <string>
#include <vector>

#include "./Datatypes.hpp"
#include "./third_party/json.hpp"

using json = nlohmann::json;

namespace Ra2ob {

class Viewer {
public:
    Viewer();
    ~Viewer();

    json exportJson(tagGameInfo gi, int mode = 0);
    void print(tagGameInfo gi, int mode = 0, int indent = 0);
    std::string uint32ToHex(uint32_t num);
    std::array<std::string, MAXPLAYER> vecToHex(const std::array<uint32_t, MAXPLAYER>& source);
};

inline Viewer::Viewer() {}

/**
 * mode 0 - brief, 1 - full, 2 - debug
 */
inline json Viewer::exportJson(tagGameInfo gi, int mode) {
    json j;

    if (mode == 2) {
        j["debug"]["playerBase"]   = vecToHex(gi.debug.playerBase);
        j["debug"]["buildingBase"] = vecToHex(gi.debug.buildingBase);
        j["debug"]["infantryBase"] = vecToHex(gi.debug.infantryBase);
        j["debug"]["tankBase"]     = vecToHex(gi.debug.tankBase);
        j["debug"]["aircraftBase"] = vecToHex(gi.debug.aircraftBase);
        j["debug"]["houseType"]    = vecToHex(gi.debug.houseType);
    }

    if (!(GOODINTENTION || gi.isObserver)) {
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
        jp["panel"]["color"]       = "#" + p.panel.color;
        jp["panel"]["country"]     = p.panel.country;

        for (auto& u : p.units.units) {
            json ju;

            if (mode == 0 && u.num == 0) {
                continue;
            }

            if (u.show == false) {
                continue;
            }

            ju["unitName"] = u.unitName;
            ju["num"]      = u.num;

            if (mode == 1) {
                ju["index"] = u.index;
            }

            jp["units"].push_back(ju);
        }

        if (!p.building.list.empty()) {
            json jb;

            for (auto& b : p.building.list) {
                jb["name"]     = b.name;
                jb["progress"] = b.progress;
                if (b.progress == 54) {
                    jb["status"] = "Ready";
                } else if (b.status == 1) {
                    jb["status"] = "On Hold";
                } else {
                    jb["status"] = "Building";
                }
            }

            jp["producingList"] = jb;
        }

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

    if (!(GOODINTENTION || gi.isObserver)) {
        std::cout << "This player is not observer.";
        return;
    }

    if (gi.currentFrame < 5) {
        std::cout << "Game preparing.";
        return;
    }

    if (gi.isGameOver) {
        std::cout << "Game Over.";
        return;
    }

    std::cout << "Game Version: ";
    if (gi.gameVersion == "Yr") {
        std::cout << "Yr. | ";
    } else {
        std::cout << "Ra2. | ";
    }
    std::cout << "Game Frame: " << gi.currentFrame << "\n";

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

        if (p.status.infantrySelfHeal || p.status.unitSelfHeal) {
            std::cout << " Auto Repair: ";
        }
        if (p.status.infantrySelfHeal) {
            std::cout << "[Infantry+] ";
        }
        if (p.status.unitSelfHeal) {
            std::cout << "[Tank+] ";
        }

        std::cout << "\n";

        for (auto& u : p.units.units) {
            if (mode == 0 && u.num == 0) {
                continue;
            }

            if (u.num < 0 || u.num > UNITSAFE) {
                continue;
            }

            if (u.show == false) {
                continue;
            }

            std::cout << u.unitName << ": " << u.num;

            if (mode == 1) {
                std::cout << " index=" << u.index;
            }

            std::cout << "\n";
        }

        if (!p.building.list.empty()) {
            std::cout << "Producing List: "
                      << "\n";

            for (auto& b : p.building.list) {
                std::cout << b.name << " " << b.progress << "/54 ";
                if (b.progress == 54) {
                    std::cout << "Ready ";
                } else if (b.status == 1) {
                    std::cout << "On Hold ";
                } else {
                    std::cout << "Building ";
                }

                if (b.number > 1) {
                    std::cout << "[" << b.number << "]";
                }

                std::cout << "\n";
            }
        }

        for (int i = 0; i < RULER_MULT; i++) {
            std::cout << STR_RULER;
        }

        std::cout << std::endl;
    }
}

inline std::string Viewer::uint32ToHex(uint32_t num) {
    std::stringstream ss;
    ss << std::hex << num;
    return ss.str();
}

inline std::array<std::string, MAXPLAYER> Viewer::vecToHex(
    const std::array<uint32_t, MAXPLAYER>& source) {
    std::array<std::string, MAXPLAYER> ret{};

    for (int i = 0; i < source.size(); i++) {
        ret[i] = uint32ToHex(source[i]);
    }

    return ret;
}

inline Viewer::~Viewer() {}

}  // namespace Ra2ob

#endif  // RA2OB_SRC_VIEWER_HPP_
