/*
    Copyright (C) 2023  wudi-7mi  wudi7mi@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./ra2ob.hpp"

#include <TlHelp32.h>  // for PROCESSENTRY32, needs to be included after windows.h
#include <Windows.h>

#include <codecvt>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>
#include <sstream>
#include <string>
#include <thread>  // NOLINT
#include <vector>


using json = nlohmann::json;

Ra2ob& Ra2ob::getInstance() {
    static Ra2ob instance;
    return instance;
}

Ra2ob::Ra2ob() {


    _pHandle    = nullptr;
    _strName    = StrName();
    _strCountry = StrCountry();
    _view       = View();

    initDatas();

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

    _houseTypes   = std::vector<uint32_t>(MAXPLAYER, 0);
    _buildingList = std::vector<BuildingList>(MAXPLAYER, BuildingList());
    _colors       = std::vector<uint32_t>(MAXPLAYER, 0x000000);
}

Ra2ob::~Ra2ob() {
    if (_pHandle != nullptr) {
        CloseHandle(_pHandle);
    }
}

Ra2ob::View::View(std::string jsonFile) {
    loadFromJson(jsonFile);
    m_gameValid = false;
}

Ra2ob::View::~View() {}

void Ra2ob::View::loadFromJson(std::string jsonFile) {
    std::vector<std::string> ret;
    std::ifstream f(jsonFile);

    json data;

    try {
        data = json::parse(f);
    } catch (json::parse_error err) {
        std::cerr << "view.json parse error." << std::endl;
        std::exit(1);
    }

    m_viewType = ViewType(data["ViewType"]);

    for (std::string it : data["PanelItems"]) {
        json jsonArray = json::array();

        for (int i = 0; i < MAXPLAYER; i++) {
            jsonArray.push_back("0");
        }
        m_numericView[it] = jsonArray;
    }

    for (auto& it : data["UnitItems"]["DefaultView"]) {
        std::string key = it["Name"];
        // int index = it["Index"];
        json jsonArray = json::array();

        for (int i = 0; i < MAXPLAYER; i++) {
            jsonArray.push_back("0");
        }

        m_unitView[key] = jsonArray;
        // m_order[key] = index;
    }

    json jsonValidPlayer = json::array();
    for (int i = 0; i < MAXPLAYER; i++) {
        jsonValidPlayer.push_back(false);
    }
    m_validPlayer = jsonValidPlayer;

    json jsonPlayerbase = json::array();
    for (int i = 0; i < MAXPLAYER; i++) {
        jsonPlayerbase.push_back("");
    }
    m_debugInfo["Playerbase"] = jsonPlayerbase;
}

void Ra2ob::View::refreshView(std::string key, std::string value, int index) {
    if (m_numericView.contains(key)) {
        m_numericView[key][index] = value;
    } else if (m_unitView.contains(key)) {
        m_unitView[key][index] = value;
    } else {
        m_debugInfo[key][index] = value;
    }
}

void Ra2ob::View::sortView() {}

void Ra2ob::View::viewPrint() {
    sortView();

    for (int i = 0; i < MAXPLAYER; i++) {
        if (!m_validPlayer[i]) {
            continue;
        }

        std::cout << std::endl;

        for (auto& it : m_numericView.items()) {
            auto v = it.value();
            if (v[i] != "0" && v[i] != "") {
                if (it.key() == "Player Name") {
                    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                    std::wstring wstr = converter.from_bytes(v[i]);

                    std::cout << it.key() << ": ";
                    std::wcout.imbue(std::locale("zh_CN"));
                    std::wcout << wstr << std::endl;
                } else {
                    std::cout << it.key() << ": " << v[i] << std::endl;
                }
            }
        }

        for (auto& it : m_unitView.items()) {
            auto v = it.value();
            if (m_viewType == ViewType::Auto || m_viewType == ViewType::ManualNoZero) {
                if (v[i] != "0" && v[i] != "") {
                    std::cout << it.key() << ": " << v[i] << std::endl;
                }
            } else {
                std::cout << it.key() << ": " << v[i] << std::endl;
            }
        }
    }
}

json Ra2ob::View::viewToJson() {
    json j;

    sortView();

    j["player_info"] = json::array();

    for (int i = 0; i < MAXPLAYER; i++) {
        json jp;

        if (!m_validPlayer[i]) {
            continue;
        }

        for (auto& it : m_numericView.items()) {
            auto v = it.value();
            if (v[i] != "0" && v[i] != "") {
                jp[it.key()] = v[i];
            }
        }

        for (auto& it : m_unitView.items()) {
            auto v = it.value();
            if (m_viewType == ViewType::Auto || m_viewType == ViewType::ManualNoZero) {
                if (v[i] != "0" && v[i] != "") {
                    jp[it.key()] = v[i];
                }
            } else {
                jp[it.key()] = v[i];
            }
        }

        j["player_info"].emplace_back(jp);
    }

    j["game_running"] = m_gameValid;

    return j;
}

json Ra2ob::View::viewToJsonFull() {
    json j;

    j["player_info"]   = json::array();
    j["valid_players"] = json::array();

    for (int i = 0; i < MAXPLAYER; i++) {
        json jp;

        jp["player_index"] = i;

        if (!m_validPlayer[i]) {
            continue;
        }

        j["valid_players"].emplace_back(i);

        for (auto& it : m_numericView.items()) {
            auto v       = it.value();
            jp[it.key()] = v[i];
        }

        json ui;

        for (auto& it : m_unitView.items()) {
            auto v       = it.value();
            ui[it.key()] = v[i];
        }

        jp["unit_info"] = ui;

        j["player_info"].emplace_back(jp);
    }

    j["game_running"] = m_gameValid;
    j["debug_info"]   = m_debugInfo;

    return j;
}

json Ra2ob::View::getPlayerPanelInfo(int index) {
    if (!m_validPlayer[index]) {
        return json({});
    }

    json j;

    for (auto& it : m_numericView.items()) {
        auto v      = it.value();
        j[it.key()] = v[index];
    }

    return j;
}

json Ra2ob::View::getPlayerUnitInfo(int index) {
    if (!m_validPlayer[index]) {
        return json({});
    }

    json j;

    for (auto& it : m_unitView.items()) {
        auto v      = it.value();
        j[it.key()] = v[index];
    }

    return j;
}

Ra2ob::Base::Base(std::string name, uint32_t offset) {
    m_name   = name;
    m_offset = offset;
    m_value  = std::vector<uint32_t>(MAXPLAYER, 0);
    m_size   = NUMSIZE;
}

Ra2ob::Base::~Base() {}

void Ra2ob::Base::showInfo() {
    std::cout << "Data name: " << m_name << std::endl;
    std::cout << "Offset: 0x" << std::hex << m_offset << std::endl;
    std::cout << "Size: " << m_size << std::endl;
}

std::string Ra2ob::Base::getName() { return m_name; }

uint32_t Ra2ob::Base::getValueByIndex(int index) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
        return -1;
    }
    return m_value[index];
}

void Ra2ob::Base::setValueByIndex(int index, uint32_t value) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
    }
    m_value[index] = value;
}

void Ra2ob::Base::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        uint32_t buf = 0;

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);
        m_value[i] = buf;
    }
}

Ra2ob::Numeric::Numeric(std::string name, uint32_t offset) : Base(name, offset) {}

Ra2ob::Numeric::~Numeric() {}

Ra2ob::Unit::Unit(std::string name, uint32_t offset, UnitType ut) : Ra2ob::Base(name, offset) {
    m_unitType = ut;
}

Ra2ob::Unit::~Unit() {}

void Ra2ob::Unit::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets,
                            std::vector<uint32_t> valids) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        if (valids.size() == 0 || m_offset >= valids[i] * 4) {
            m_value[i] = 0;
            continue;
        }

        uint32_t buf = 0;

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);
        m_value[i] = buf;
    }
}

Ra2ob::UnitType Ra2ob::Unit::getUnitType() { return m_unitType; }

Ra2ob::StrName::StrName(std::string name, uint32_t offset) : Ra2ob::Base(name, offset) {
    m_value = std::vector<std::string>(MAXPLAYER, "");
    m_size  = STRNAMESIZE;
}

Ra2ob::StrName::~StrName() {}

void Ra2ob::StrName::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        wchar_t buf[STRNAMESIZE] = L"";

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        m_value[i] = convert.to_bytes(buf);
    }
}

std::string Ra2ob::StrName::getValueByIndex(int index) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
        return "";
    }
    return m_value[index];
}

void Ra2ob::StrName::setValueByIndex(int index, std::string value) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
    }
    m_value[index] = value;
}

Ra2ob::StrCountry::StrCountry(std::string name, uint32_t offset) : StrName(name, offset) {
    m_size = STRCOUNTRYSIZE;
}

Ra2ob::StrCountry::~StrCountry() {}

void Ra2ob::StrCountry::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        char buf[STRCOUNTRYSIZE] = "\0";

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);

        if (m_countryMap.find(buf) == m_countryMap.end()) {
            m_value[i] = "";
        } else {
            m_value[i] = m_countryMap[buf];
        }
    }
}

Ra2ob::WinOrLose::WinOrLose(std::string name, uint32_t offset) : Ra2ob::Base(name, offset) {
    m_value = std::vector<bool>(MAXPLAYER, false);
    m_size  = BOOLSIZE;
}

Ra2ob::WinOrLose::~WinOrLose() {}

void Ra2ob::WinOrLose::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        bool buf = false;

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);
        m_value[i] = buf;
    }
}

bool Ra2ob::WinOrLose::getValueByIndex(int index) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be bigger than MAXPLAYER." << std::endl;
        return false;
    }
    return m_value[index];
}

void Ra2ob::WinOrLose::setValueByIndex(int index, bool value) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be bigger than MAXPLAYER." << std::endl;
    }
    m_value[index] = value;
}

Ra2ob::Numerics Ra2ob::loadNumericsFromJson(std::string filePath) {
    Numerics numerics;

    std::ifstream f(filePath);
    json data;

    try {
        data = json::parse(f);
    } catch (json::parse_error err) {
        std::cerr << "panel_offsets.json parse error." << std::endl;
        std::exit(1);
    }

    for (auto& it : data) {
        std::string offset = it["Offset"];
        uint32_t s_offset  = std::stoul(offset, nullptr, 16);
        Numeric n(it["Name"], s_offset);
        numerics.push_back(n);
    }

    return numerics;
}

Ra2ob::Units Ra2ob::loadUnitsFromJson(std::string filePath) {
    Units units;

    std::ifstream f(filePath);
    json data;

    try {
        data = json::parse(f);
    } catch (json::parse_error err) {
        std::cerr << "unit_offsets.json parse error." << std::endl;
        std::exit(1);
    }

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
            units.push_back(ub);
        }
    }

    return units;
}

Ra2ob::WinOrLoses Ra2ob::initWinOrLose() {
    WinOrLoses ret;

    WinOrLose w("Win", WINOFFSET);
    WinOrLose l("Lose", LOSEOFFSET);

    ret.push_back(w);
    ret.push_back(l);

    return ret;
}

void Ra2ob::initDatas() {
    _numerics   = loadNumericsFromJson();
    _units      = loadUnitsFromJson();
    _winOrLoses = initWinOrLose();
}

bool Ra2ob::initAddrs() {
    if (NULL == _pHandle) {
        std::cerr << "No valid process handle, call getHandle() first." << std::endl;
        return false;
    }

    uint32_t fixed              = getAddr(FIXEDOFFSET);
    uint32_t classBaseArray     = getAddr(CLASSBASEARRAYOFFSET);
    uint32_t playerBaseArrayPtr = fixed + PLAYERBASEARRAYPTROFFSET;

    if (playerBaseArrayPtr == 1) {
        return false;
    }

    for (int i = 0; i < MAXPLAYER; i++, playerBaseArrayPtr += 4) {
        uint32_t playerBase = getAddr(playerBaseArrayPtr);

        _players[i] = false;

        if (playerBase != INVALIDCLASS) {
            uint32_t realPlayerBase = getAddr(playerBase * 4 + classBaseArray);

            _players[i]     = true;
            _playerBases[i] = realPlayerBase;

            _buildings[i] = getAddr(realPlayerBase + BUILDINGOFFSET);
            _tanks[i]     = getAddr(realPlayerBase + TANKOFFSET);
            _infantrys[i] = getAddr(realPlayerBase + INFANTRYOFFSET);
            _aircrafts[i] = getAddr(realPlayerBase + AIRCRAFTOFFSET);

            _buildings_valid[i] = getAddr(realPlayerBase + BUILDINGOFFSET + 4);
            _tanks_valid[i]     = getAddr(realPlayerBase + TANKOFFSET + 4);
            _infantrys_valid[i] = getAddr(realPlayerBase + INFANTRYOFFSET + 4);
            _aircrafts_valid[i] = getAddr(realPlayerBase + AIRCRAFTOFFSET + 4);

            _houseTypes[i] = getAddr(realPlayerBase + HOUSETYPEOFFSET);

            if (_buildings[i] == 1 && _tanks[i] == 1 && _infantrys[i] == 1 && _aircrafts[i] == 1 &&
                _houseTypes[i] == 1) {
                return false;
            }
        }
    }

    return true;
}

int Ra2ob::hasPlayer() {
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

bool Ra2ob::refreshInfo() {
    if (!hasPlayer()) {
        std::cerr << "No valid player to show info." << std::endl;
        return false;
    }

    for (auto& it : _numerics) {
        it.fetchData(_pHandle, _playerBases);
    }

    for (auto& it : _units) {
        if (it.getUnitType() == UnitType::Building) {
            it.fetchData(_pHandle, _buildings, _buildings_valid);
        } else if (it.getUnitType() == UnitType::Infantry) {
            it.fetchData(_pHandle, _infantrys, _infantrys_valid);
        } else if (it.getUnitType() == UnitType::Tank) {
            it.fetchData(_pHandle, _tanks, _tanks_valid);
        } else {
            it.fetchData(_pHandle, _aircrafts, _aircrafts_valid);
        }
    }

    for (auto& it : _winOrLoses) {
        it.fetchData(_pHandle, _playerBases);
    }

    _strName.fetchData(_pHandle, _playerBases);
    _strCountry.fetchData(_pHandle, _houseTypes);

    refreshBuildingList();
    refreshColors();

    return true;
}

void Ra2ob::updateView(bool show) {
    for (int i = 0; i < MAXPLAYER; i++) {
        // Filter invalid players
        if (!_players[i]) {
            continue;
        }
        if (_strCountry.getValueByIndex(i) == "") {
            continue;
        }

        _view.m_validPlayer[i] = true;

        // Refresh Name & Country
        _view.refreshView(_strName.getName(), _strName.getValueByIndex(i), i);
        _view.refreshView(_strCountry.getName(), _strCountry.getValueByIndex(i), i);

        // Refresh Color
        _view.refreshView("Color", std::to_string(_colors[i]), i);

        // Refresh numeric values
        for (auto& it : _numerics) {
            if (_view.m_numericView.find(it.getName()) != _view.m_numericView.end()) {
                _view.refreshView(it.getName(), std::to_string(it.getValueByIndex(i)), i);
            }
        }

        // Refresh units
        for (auto& it : _units) {
            if (_view.m_unitView.find(it.getName()) != _view.m_unitView.end()) {
                int unitNum = it.getValueByIndex(i);

                if (unitNum < UNITSAFE) {
                    _view.refreshView(it.getName(), std::to_string(unitNum), i);
                }
            }
        }

        // Refresh win or lose
        _view.refreshView("Win Or Lose", "unknown", i);
        if (_winOrLoses[0].getValueByIndex(i) == true) {
            _view.refreshView("Win Or Lose", "win", i);
        }
        if (_winOrLoses[1].getValueByIndex(i) == true) {
            _view.refreshView("Win Or Lose", "lose", i);
        }

        // Refresh playerbase
        _view.refreshView("Playerbase", std::to_string(_playerBases[i]), i);
    }

    if (show) {
        _view.viewPrint();
    }
}

int Ra2ob::getHandle(bool show) {
    DWORD pid = 0;

    std::wstring w_name = L"gamemd-spawn.exe";
    std::string name    = "gamemd-spawn.exe";

    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    hProcessSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    hThreadSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot" << std::endl;
        return 1;
    }

    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create thread snapshot" << std::endl;
        return 1;
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
                    // std::cout << "THREAD ID: " << te32.th32ThreadID;
                    // std::cout << ", base priority: " << te32.tpBasePri << std::endl;
                    thread_nums++;
                }
            }

            if (thread_nums != 0) {
                if (show) {
                    std::cout << "PID Found: " << pid << std::endl;
                }
                break;
            }

            pid = 0;
        }
    }

    if (pid == 0) {
        if (show) {
            std::cerr << "No Valid PID. Finding \"gamemd-spawn.exe\"." << std::endl;
        }
        return 1;
    }

    HANDLE pHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ,
        FALSE, pid);

    if (pHandle == NULL) {
        std::cerr << "Could not open process" << std::endl;
        return 1;
    }

    _pHandle = pHandle;

    return 0;
}

uint32_t Ra2ob::getAddr(uint32_t offset) {
    uint32_t buf = 0;

    if (!readMemory(_pHandle, offset, &buf, 4)) {
        return 1;
    }

    return buf;
}

int Ra2ob::getInt(uint32_t offset) {
    int buf = 0;

    if (!readMemory(_pHandle, offset, &buf, 4)) {
        return -1;
    }

    return buf;
}

std::string Ra2ob::getString(uint32_t offset) {
    char buf[STRUNITNAMESIZE] = "\0";

    readMemory(_pHandle, offset, &buf, STRUNITNAMESIZE);

    std::string ret = buf;

    return ret;
}

uint32_t Ra2ob::getColor(uint32_t offset) {
    uint32_t buf = 0;

    readMemory(_pHandle, offset, &buf, 3);

    return buf;
}

bool Ra2ob::readMemory(HANDLE handle, uint32_t addr, void* value, uint32_t size) {
    return ReadProcessMemory(handle, (const void*)addr, value, size, nullptr);
}

std::string Ra2ob::getTime() {
    char timeBuffer[15];

    time_t now = time(nullptr);
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d%H%M%S", std::localtime(&now));
    std::string timeStr = timeBuffer;

    return timeStr;
}

void Ra2ob::refreshBuildingList() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        uint32_t addr = _playerBases[i];
        uint32_t base = getAddr(addr + P_INFANTRYOFFSET);

        int currentCD      = getInt(base + P_TIMEOFFSET);
        int status         = getInt(base + P_STATUSOFFSET);
        uint32_t queueAddr = getAddr(base + P_QUEUEPTROFFSET);
        int queueLen       = getInt(base + P_QUEUELENGTHOFFSET);

        BuildingList bl;

        for (int j = 0; j < queueLen; j++) {
            int nodeAddr = getAddr(queueAddr + j * 4);

            std::string name = getString(nodeAddr + P_NAMEOFFSET);
            BuildingNode bn  = BuildingNode(name);

            bl.list.push_back(bn);
        }

        _buildingList[i] = bl;
    }
}

void Ra2ob::refreshColors() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        uint32_t addr  = _playerBases[i];
        uint32_t color = getColor(addr + COLOROFFSET);

        color = (color & 0x00FF00) | (color << 16 & 0xFF0000) | (color >> 16 & 0x0000FF);

        _colors[i] = color;
    }
}

void Ra2ob::close() {
    if (_pHandle != nullptr) {
        CloseHandle(_pHandle);
    }
    _strName    = StrName();
    _strCountry = StrCountry();
    _view       = View();
    initDatas();
    std::cout << "Handle Closed." << std::endl;
}

void Ra2ob::detectTask(bool show, int interval) {
    while (true) {
        if (getHandle(show) == 0) {
            _gameValid        = true;
            _view.m_gameValid = true;
            initAddrs();
        }

        Sleep(interval);
    }
}

void Ra2ob::fetchTask(int interval) {
    while (true) {
        if (_gameValid && _view.m_gameValid) {
            if (!refreshInfo()) {
                _gameValid = false;
            }

            if (!initAddrs()) {
                _gameValid = false;
            }
        } else if (_view.m_gameValid) {
            _view.m_gameValid = false;
            close();
        }

        Sleep(interval);
    }
}

void Ra2ob::refreshViewTask(bool show, int interval) {
    while (true) {
        if (_gameValid && _view.m_gameValid) {
            if (show) {
                system("cls");
                std::cout << "Player numbers: " << hasPlayer() << std::endl;
            }

            updateView(show);

            if (show) {
                std::cout << std::endl;
            }
        }

        Sleep(interval);
    }
}

void Ra2ob::startLoop(bool show) {
    std::thread d_thread(std::bind(&Ra2ob::detectTask, this, false, 1000));

    std::thread f_thread(std::bind(&Ra2ob::fetchTask, this, 500));

    std::thread r_thread(std::bind(&Ra2ob::refreshViewTask, this, show, 500));

    d_thread.detach();
    f_thread.detach();
    r_thread.detach();
}
