#ifndef RA2OB_SRC_GAME_HPP_
#define RA2OB_SRC_GAME_HPP_

#include <algorithm>
#include <array>
#include <sstream>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "./Viewer.hpp"
#include "./third_party/inicpp.hpp"
// clang-format off
#include <psapi.h> // NOLINT
#include <TlHelp32.h>
// clang-format on

namespace Ra2ob {

class Game {
public:
    static Game& getInstance();

    Game(const Game&)           = delete;
    void operator=(const Game&) = delete;

    void getHandle();
    void initAddrs();

    void loadNumericsFromJson(std::string filePath = F_PANELOFFSETS);
    void loadUnitsFromJson(std::string filePath = F_UNITOFFSETS);
    void initStrTypes();
    void initArrays();
    void initGameInfo();

    int hasPlayer();

    void refreshInfo();
    void getBuildingInfo(tagBuildingInfo* bi, int addr, int offset_0, int offset_1, UnitType utype);
    void refreshBuildingInfos();
    void refreshColors();
    void refreshStatusInfos();
    void refreshGameVersion();
    void refreshGameFrame();

    void structBuild();

    void restart(bool valid);

    void detectTask(int interval = 500);
    void fetchTask(int inferval = 500);
    void startLoop();

    tagNumerics _numerics;
    tagUnits _units;
    tagGameInfo _gameInfo;

    StrName _strName;
    StrCountry _strCountry;

    std::array<tagBuildingInfo, MAXPLAYER> _buildingInfos;
    std::array<tagStatusInfo, MAXPLAYER> _statusInfos;

    std::array<std::string, MAXPLAYER> _colors;

    std::array<bool, MAXPLAYER> _players;
    std::array<uint32_t, MAXPLAYER> _playerBases;

    std::array<uint32_t, MAXPLAYER> _buildings;
    std::array<uint32_t, MAXPLAYER> _infantrys;
    std::array<uint32_t, MAXPLAYER> _tanks;
    std::array<uint32_t, MAXPLAYER> _aircrafts;

    std::array<uint32_t, MAXPLAYER> _buildings_valid;
    std::array<uint32_t, MAXPLAYER> _infantrys_valid;
    std::array<uint32_t, MAXPLAYER> _tanks_valid;
    std::array<uint32_t, MAXPLAYER> _aircrafts_valid;

    std::array<uint32_t, MAXPLAYER> _houseTypes;

    Reader r;
    Viewer viewer;
    Version version = Version::Yr;

private:
    Game();
    ~Game();
};

inline Game& Game::getInstance() {
    static Game instance;
    return instance;
}

inline Game::Game() {
    loadNumericsFromJson();
    loadUnitsFromJson();
    initStrTypes();
    initArrays();
    initGameInfo();
}

inline Game::~Game() {
    if (r.getHandle() != nullptr) {
        CloseHandle(r.getHandle());
    }
}

/**
 * Get game handle, set Reader.
 */
inline void Game::getHandle() {
    DWORD pid = 0;

    std::wstring w_name = L"gamemd-spawn.exe";
    std::string name    = "gamemd-spawn.exe";

    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    hProcessSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    hThreadSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot\n";
        return;
    }

    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create thread snapshot\n";
        return;
    }

    PROCESSENTRY32 processInfo{};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    for (BOOL success = Process32First(hProcessSnap, &processInfo); success;
         success      = Process32Next(hProcessSnap, &processInfo)) {
        BOOL processMatch = false;

#ifdef UNICODE
        if (wcscmp(processInfo.szExeFile, w_name.c_str()) == 0) {
            processMatch = true;
        }
#else
        if (name == processInfo.szExeFile) {
            processMatch = true;
        }
#endif

        if (processMatch) {
            THREADENTRY32 threadInfo{};
            threadInfo.dwSize = sizeof(THREADENTRY32);

            pid = processInfo.th32ProcessID;

            int thread_nums = 0;

            for (BOOL success = Thread32First(hThreadSnap, &threadInfo); success;
                 success      = Thread32Next(hThreadSnap, &threadInfo)) {
                if (threadInfo.th32OwnerProcessID == pid) {
                    thread_nums++;
                }
            }

            if (thread_nums != 0) {
                break;
            }

            pid = 0;
        }
    }

    if (pid == 0) {
        std::cerr << "No Valid PID. Finding \"gamemd-spawn.exe\".\n";
        r = Reader(nullptr);
        return;
    }

    HANDLE pHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ,
        FALSE, pid);

#ifdef UNICODE
    wchar_t exePath[256];
    GetModuleFileNameEx(pHandle, NULL, exePath, sizeof(exePath));
    std::string filePath = utf16ToGbk(exePath);
#else
    char exePath[256];
    GetModuleFileNameEx(pHandle, NULL, exePath, sizeof(exePath));
    std::string filePath = exePath;
#endif

    std::string destPart = "gamemd-spawn.exe";
    std::string iniPart  = "spawn.ini";

    filePath = filePath.replace(filePath.find(destPart), destPart.length(), iniPart);

    inicpp::IniManager iniData(filePath);

    if (iniData["Settings"].isKeyExist("GameVersion")) {
        std::string gameVersion = iniData["Settings"]["GameVersion"];
        if (gameVersion == "0") {
            version = Version::Yr;
        } else {
            version = Version::Ra2;
        }
    } else if (iniData["Settings"].isKeyExist("Ra2Mode")) {
        std::string ra2Mode = iniData["Settings"]["Ra2Mode"];
        if (ra2Mode == "False") {
            version = Version::Yr;
        } else {
            version = Version::Ra2;
        }
    }

    if (pHandle == nullptr) {
        std::cerr << "Could not open process\n";
        r = Reader(nullptr);
        return;
    }

    r = Reader(pHandle);
}

/**
 * Initialize all the addresses.
 */
inline void Game::initAddrs() {
    if (nullptr == r.getHandle()) {
        std::cerr << "No valid process handle, call Game::getHandle() first.\n";
    }

    uint32_t fixed              = r.getAddr(FIXEDOFFSET);
    uint32_t classBaseArray     = r.getAddr(CLASSBASEARRAYOFFSET);
    uint32_t playerBaseArrayPtr = fixed + PLAYERBASEARRAYPTROFFSET;

    bool isObserverFlag   = true;
    bool isAllControlable = true;
    bool isThisGameOver   = false;

    for (int i = 0; i < MAXPLAYER; i++, playerBaseArrayPtr += 4) {
        uint32_t playerBase = r.getAddr(playerBaseArrayPtr);

        _players[i] = false;

        if (playerBase != INVALIDCLASS) {
            uint32_t realPlayerBase = r.getAddr(playerBase * 4 + classBaseArray);

            bool cur          = r.getBool(realPlayerBase + CURRENTPLAYEROFFSET);
            std::string cur_s = r.getString(realPlayerBase + STRNAMEOFFSET);
            if (cur) {
                isObserverFlag = false;
            } else {
                isAllControlable = false;
            }

            bool isDefeated = r.getBool(realPlayerBase + ISDEFEATEDOFFSET);
            bool isGameOver = r.getBool(realPlayerBase + ISGAMEOVEROFFSET);
            bool isWinner   = r.getBool(realPlayerBase + ISWINNEROFFSET);

            if (isDefeated || isGameOver || isWinner) {
                isThisGameOver = true;
            }

            _players[i]     = true;
            _playerBases[i] = realPlayerBase;

            _buildings[i] = r.getAddr(realPlayerBase + BUILDINGOFFSET);
            _tanks[i]     = r.getAddr(realPlayerBase + TANKOFFSET);
            _infantrys[i] = r.getAddr(realPlayerBase + INFANTRYOFFSET);
            _aircrafts[i] = r.getAddr(realPlayerBase + AIRCRAFTOFFSET);

            _buildings_valid[i] = r.getAddr(realPlayerBase + BUILDINGOFFSET + 4);
            _tanks_valid[i]     = r.getAddr(realPlayerBase + TANKOFFSET + 4);
            _infantrys_valid[i] = r.getAddr(realPlayerBase + INFANTRYOFFSET + 4);
            _aircrafts_valid[i] = r.getAddr(realPlayerBase + AIRCRAFTOFFSET + 4);

            _houseTypes[i] = r.getAddr(realPlayerBase + HOUSETYPEOFFSET);
        }
    }

    _gameInfo.isObserver = isObserverFlag || isAllControlable;
    _gameInfo.isGameOver = isThisGameOver;
}

inline void Game::loadNumericsFromJson(std::string filePath) {
    json data = readJsonFromFile(filePath);

    _numerics.items.clear();

    for (auto& it : data) {
        std::string offset = it["Offset"];
        uint32_t s_offset  = std::stoul(offset, nullptr, 16);
        Numeric n(it["Name"], s_offset);
        _numerics.items.push_back(n);
    }
}

inline void Game::loadUnitsFromJson(std::string filePath) {
    json data = readJsonFromFile(filePath);

    _units.items.clear();

    for (auto& ut : data.items()) {
        UnitType s_ut;

        if (ut.key() == "Building") {
            s_ut = UnitType::Building;
        } else if (ut.key() == "Tank") {
            s_ut = UnitType::Tank;
        } else if (ut.key() == "Infantry") {
            s_ut = UnitType::Infantry;
        } else if (ut.key() == "Aircraft") {
            s_ut = UnitType::Aircraft;
        } else {
            s_ut = UnitType::Unknown;
        }

        for (auto& u : data[ut.key()]) {
            if (u.empty()) {
                continue;
            }

            bool s_show = true;
            if (u.contains("Show") && u["Show"] == 0) {
                s_show = false;
            }

            std::string offset = u["Offset"];
            uint32_t s_offset  = std::stoul(offset, nullptr, 16);

            int s_index = 99;
            if (u.contains("Index")) {
                s_index = u["Index"];
            }

            Unit ub(u["Name"], s_offset, s_ut, s_index, s_show);

            std::string s_invalid;
            if (u.contains("Invalid")) {
                ub.setInvalid(u["Invalid"]);
            }

            _units.items.push_back(ub);
        }
    }
}

inline void Game::initStrTypes() {
    _strName    = StrName();
    _strCountry = StrCountry();
}

inline void Game::initArrays() {
    _players     = std::array<bool, MAXPLAYER>{};
    _playerBases = std::array<uint32_t, MAXPLAYER>{};

    _buildings = std::array<uint32_t, MAXPLAYER>{};
    _infantrys = std::array<uint32_t, MAXPLAYER>{};
    _tanks     = std::array<uint32_t, MAXPLAYER>{};
    _aircrafts = std::array<uint32_t, MAXPLAYER>{};

    _buildings_valid = std::array<uint32_t, MAXPLAYER>{};
    _infantrys_valid = std::array<uint32_t, MAXPLAYER>{};
    _tanks_valid     = std::array<uint32_t, MAXPLAYER>{};
    _aircrafts_valid = std::array<uint32_t, MAXPLAYER>{};

    _houseTypes    = std::array<uint32_t, MAXPLAYER>{};
    _buildingInfos = std::array<tagBuildingInfo, MAXPLAYER>{};
    _statusInfos   = std::array<tagStatusInfo, MAXPLAYER>{};
    _colors        = std::array<std::string, MAXPLAYER>{};
    _colors.fill("0x000000");
}

inline void Game::initGameInfo() {
    _gameInfo.valid              = false;
    _gameInfo.isObserver         = false;
    _gameInfo.isGameOver         = false;
    _gameInfo.gameVersion        = "Yr";
    _gameInfo.currentFrame       = 0;
    _gameInfo.players            = std::array<tagPlayer, MAXPLAYER>{};
    _gameInfo.debug.playerBase   = std::array<uint32_t, MAXPLAYER>{};
    _gameInfo.debug.buildingBase = std::array<uint32_t, MAXPLAYER>{};
    _gameInfo.debug.infantryBase = std::array<uint32_t, MAXPLAYER>{};
    _gameInfo.debug.tankBase     = std::array<uint32_t, MAXPLAYER>{};
    _gameInfo.debug.aircraftBase = std::array<uint32_t, MAXPLAYER>{};
    _gameInfo.debug.houseType    = std::array<uint32_t, MAXPLAYER>{};
}

/**
 * Return valid player number.
 */
inline int Game::hasPlayer() {
    int count = 0;

    for (bool it : _players) {
        if (it) {
            count++;
        }
    }
    if (count == 0) {
        std::cerr << "No valid player." << std::endl;
    }

    return count;
}

/**
 * Fetch the data in all the base class.
 */
inline void Game::refreshInfo() {
    if (!hasPlayer()) {
        std::cerr << "No valid player to show info.\n";
        _gameInfo.valid = false;
        return;
    }

    for (auto& it : _numerics.items) {
        it.fetchData(r, _playerBases);
    }

    for (auto& it : _units.items) {
        if (it.getUnitType() == UnitType::Building) {
            it.fetchData(r, _buildings, _buildings_valid);
        } else if (it.getUnitType() == UnitType::Infantry) {
            it.fetchData(r, _infantrys, _infantrys_valid);
        } else if (it.getUnitType() == UnitType::Tank) {
            it.fetchData(r, _tanks, _tanks_valid);
        } else {
            it.fetchData(r, _aircrafts, _aircrafts_valid);
        }
    }

    _strName.fetchData(r, _playerBases);
    _strCountry.fetchData(r, _houseTypes);

    refreshBuildingInfos();
    refreshColors();
    refreshStatusInfos();
    refreshGameVersion();
    refreshGameFrame();
}

inline void Game::getBuildingInfo(tagBuildingInfo* bi, int addr, int offset_0, int offset_1,
                                  UnitType utype) {
    uint32_t base = r.getAddr(addr + offset_0);

    int currentCD = r.getInt(base + P_TIMEOFFSET);
    bool status   = r.getBool(base + P_STATUSOFFSET);

    uint32_t current = r.getAddr(base + P_CURRENTOFFSET);
    uint32_t type    = r.getAddr(current + offset_1);
    int offset       = r.getInt(type + P_ARRAYINDEXOFFSET);

    // Count same production item.
    int queueLength    = r.getInt(base + P_QUEUELENGTHOFFSET);
    uint32_t queueBase = r.getAddr(base + P_QUEUEPTROFFSET);
    int count          = 1;
    for (int i = 0; i < queueLength; i++) {
        uint32_t curAddr = r.getAddr(queueBase + i * 4);
        int curOffset    = r.getInt(curAddr + P_ARRAYINDEXOFFSET);
        if (curOffset != offset) {
            break;
        }
        count++;
    }

    Version v = version;
    auto it   = std::find_if(
        _units.items.begin(), _units.items.end(),
        [offset, utype, v](const Unit& u) { return u.checkOffset(offset * 4, utype, v); });

    std::string name;

    if (it != _units.items.end()) {
        name = it->getName();

        tagBuildingNode bn = tagBuildingNode(name);

        bn.progress = currentCD;
        bn.status   = status;
        bn.number   = count;

        bi->list.push_back(bn);
    }
}

inline void Game::refreshBuildingInfos() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        tagBuildingInfo bi;

        uint32_t addr = _playerBases[i];

        getBuildingInfo(&bi, addr, P_AIRCRAFTOFFSET, P_UNITTYPEOFFSET, UnitType::Aircraft);
        getBuildingInfo(&bi, addr, P_BUILDINGFIRSTOFFSET, P_BUILDINGTYPEOFFSET, UnitType::Building);
        getBuildingInfo(&bi, addr, P_BUILDINGSECONDOFFSET, P_BUILDINGTYPEOFFSET,
                        UnitType::Building);
        getBuildingInfo(&bi, addr, P_INFANTRYOFFSET, P_INFANTRYTYPEOFFSET, UnitType::Infantry);
        getBuildingInfo(&bi, addr, P_TANKOFFSET, P_UNITTYPEOFFSET, UnitType::Tank);
        getBuildingInfo(&bi, addr, P_SHIPOFFSET, P_UNITTYPEOFFSET, UnitType::Tank);

        _buildingInfos[i] = bi;
    }
}

inline void Game::refreshColors() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        uint32_t addr  = _playerBases[i];
        uint32_t color = r.getColor(addr + COLOROFFSET);

        color = (color & 0x00FF00) | (color << 16 & 0xFF0000) | (color >> 16 & 0x0000FF);

        std::stringstream ss;
        ss << std::hex << color;
        _colors[i] = ss.str();
    }
}

inline void Game::refreshStatusInfos() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        tagStatusInfo si;

        uint32_t addr = _playerBases[i];

        int infantrySelfHeal = r.getInt(addr + INFANTRYSELFHEALOFFSET);
        int unitSelfHeal     = r.getInt(addr + UNITSELFHEALOFFSET);

        si.infantrySelfHeal = infantrySelfHeal;
        si.unitSelfHeal     = unitSelfHeal;

        _statusInfos[i] = si;
    }
}

inline void Game::refreshGameVersion() {
    if (version == Version::Yr) {
        _gameInfo.gameVersion = "Yr";
    } else {
        _gameInfo.gameVersion = "Ra2";
    }
}

inline void Game::refreshGameFrame() { _gameInfo.currentFrame = r.getInt(GAMEFRAMEOFFSET); }

inline void Game::structBuild() {
    for (int i = 0; i < MAXPLAYER; i++) {
        tagPlayer p;

        // Filter invalid players
        if (!_players[i] || _strCountry.getValueByIndex(i) == "") {
            p.valid = false;
            continue;
        }

        // Panel info
        tagPanelInfo pi;
        pi.playerName    = _strName.getValueByIndex(i);
        pi.playerNameUtf = _strName.getValueByIndexUtf(i);
        pi.balance       = _numerics.getItem("Balance").getValueByIndex(i);
        pi.creditSpent   = _numerics.getItem("Credit Spent").getValueByIndex(i);
        pi.powerDrain    = _numerics.getItem("Power Drain").getValueByIndex(i);
        pi.powerOutput   = _numerics.getItem("Power Output").getValueByIndex(i);
        pi.color         = _colors[i];
        pi.country       = _strCountry.getValueByIndex(i);

        // Units info
        tagUnitsInfo ui;
        for (auto& it : _units.items) {
            tagUnitSingle us;
            us.unitName = it.getName();
            us.index    = it.getUnitIndex();
            us.num      = it.getValueByIndex(i);
            us.show     = it.checkShow();
            ui.units.push_back(us);
        }

        std::sort(ui.units.begin(), ui.units.end(),
                  [](const tagUnitSingle& a, const tagUnitSingle& b) { return a.index < b.index; });

        // Players info
        p.valid    = true;
        p.panel    = pi;
        p.units    = ui;
        p.building = _buildingInfos[i];
        p.status   = _statusInfos[i];

        // Game info
        _gameInfo.players[i]            = p;
        _gameInfo.debug.playerBase[i]   = _playerBases[i];
        _gameInfo.debug.buildingBase[i] = _buildings[i];
        _gameInfo.debug.infantryBase[i] = _infantrys[i];
        _gameInfo.debug.tankBase[i]     = _tanks[i];
        _gameInfo.debug.aircraftBase[i] = _aircrafts[i];
        _gameInfo.debug.houseType[i]    = _houseTypes[i];
    }
}

inline void Game::restart(bool valid) {
    if (!valid) {
        return;
    }

    if (r.getHandle() != nullptr) {
        CloseHandle(r.getHandle());
    }

    std::cout << "Handle Closed.\n";

    initStrTypes();
    initArrays();
    initGameInfo();
}

inline void Game::startLoop() {
    std::thread d_thread(std::bind(&Game::detectTask, this, T_DETECTTIME));

    std::thread f_thread(std::bind(&Game::fetchTask, this, T_FETCHTIME));

    d_thread.detach();
    f_thread.detach();
}

inline void Game::detectTask(int interval) {
    while (true) {
        getHandle();
        if (r.getHandle() != nullptr) {
            _gameInfo.valid = true;
            initAddrs();
        } else {
            restart(_gameInfo.valid);
            _gameInfo.valid = false;
        }

        Sleep(interval);
    }
}

inline void Game::fetchTask(int interval) {
    while (true) {
        if (_gameInfo.valid) {
            refreshInfo();
            structBuild();
            initAddrs();
        }

        Sleep(interval);
    }
}

}  // end of namespace Ra2ob

#endif  // RA2OB_SRC_GAME_HPP_
