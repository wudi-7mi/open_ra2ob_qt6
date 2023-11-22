#ifndef RA2OB_SRC_GAME_HPP_
#define RA2OB_SRC_GAME_HPP_

#include <sstream>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "./Viewer.hpp"

// clang-format off
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
    void initVectors();
    void initGameInfo();

    int hasPlayer();

    void refreshInfo();
    void refreshBuildingInfos();
    void refreshColors();

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

    std::vector<tagBuildingInfo> _buildingInfos;
    std::vector<std::string> _colors;

    std::vector<bool> _players;
    std::vector<uint32_t> _playerBases;

    std::vector<uint32_t> _buildings;
    std::vector<uint32_t> _infantrys;
    std::vector<uint32_t> _tanks;
    std::vector<uint32_t> _aircrafts;

    std::vector<uint32_t> _buildings_valid;
    std::vector<uint32_t> _infantrys_valid;
    std::vector<uint32_t> _tanks_valid;
    std::vector<uint32_t> _aircrafts_valid;

    std::vector<uint32_t> _houseTypes;

    Reader r;
    Viewer viewer;

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
    initVectors();
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

    for (int i = 0; i < MAXPLAYER; i++, playerBaseArrayPtr += 4) {
        uint32_t playerBase = r.getAddr(playerBaseArrayPtr);

        _players[i] = false;

        if (playerBase != INVALIDCLASS) {
            uint32_t realPlayerBase = r.getAddr(playerBase * 4 + classBaseArray);

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

            std::string offset = u["Offset"];
            uint32_t s_offset  = std::stoul(offset, nullptr, 16);

            Unit ub(u["Name"], s_offset, s_ut);
            _units.items.push_back(ub);
        }
    }
}

inline void Game::initStrTypes() {
    _strName    = StrName();
    _strCountry = StrCountry();
}

inline void Game::initVectors() {
    _players     = std::vector<bool>(MAXPLAYER, false);
    _playerBases = std::vector<uint32_t>(MAXPLAYER, 0);

    _buildings = std::vector<uint32_t>(MAXPLAYER, 0);
    _infantrys = std::vector<uint32_t>(MAXPLAYER, 0);
    _tanks     = std::vector<uint32_t>(MAXPLAYER, 0);
    _aircrafts = std::vector<uint32_t>(MAXPLAYER, 0);

    _buildings_valid = std::vector<uint32_t>(MAXPLAYER, 0);
    _infantrys_valid = std::vector<uint32_t>(MAXPLAYER, 0);
    _tanks_valid     = std::vector<uint32_t>(MAXPLAYER, 0);
    _aircrafts_valid = std::vector<uint32_t>(MAXPLAYER, 0);

    _houseTypes    = std::vector<uint32_t>(MAXPLAYER, 0);
    _buildingInfos = std::vector<tagBuildingInfo>(MAXPLAYER, tagBuildingInfo());
    _colors        = std::vector<std::string>(MAXPLAYER, "0x000000");
}

inline void Game::initGameInfo() {
    _gameInfo.players            = std::vector<tagPlayer>(MAXPLAYER, tagPlayer());
    _gameInfo.debug.playerBase   = std::vector<uint32_t>(MAXPLAYER, 0);
    _gameInfo.debug.buildingBase = std::vector<uint32_t>(MAXPLAYER, 0);
    _gameInfo.debug.infantryBase = std::vector<uint32_t>(MAXPLAYER, 0);
    _gameInfo.debug.tankBase     = std::vector<uint32_t>(MAXPLAYER, 0);
    _gameInfo.debug.aircraftBase = std::vector<uint32_t>(MAXPLAYER, 0);
    _gameInfo.debug.houseType    = std::vector<uint32_t>(MAXPLAYER, 0);
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
}

inline void Game::refreshBuildingInfos() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        uint32_t addr = _playerBases[i];
        uint32_t base = r.getAddr(addr + P_INFANTRYOFFSET);

        int currentCD      = r.getInt(base + P_TIMEOFFSET);
        int status         = r.getInt(base + P_STATUSOFFSET);
        uint32_t queueAddr = r.getAddr(base + P_QUEUEPTROFFSET);
        int queueLen       = r.getInt(base + P_QUEUELENGTHOFFSET);

        tagBuildingInfo bi;

        for (int j = 0; j < queueLen; j++) {
            int nodeAddr = r.getAddr(queueAddr + j * 4);

            std::string name   = r.getString(nodeAddr + P_NAMEOFFSET);
            tagBuildingNode bn = tagBuildingNode(name);

            bi.list.push_back(bn);
        }

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
            us.num      = it.getValueByIndex(i);
            ui.units.push_back(us);
        }

        // Players info
        p.valid    = true;
        p.panel    = pi;
        p.units    = ui;
        p.building = _buildingInfos[i];

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
    initVectors();
    initGameInfo();
}

inline void Game::startLoop() {
    std::thread d_thread(std::bind(&Game::detectTask, this, 1000));

    std::thread f_thread(std::bind(&Game::fetchTask, this, 500));

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
