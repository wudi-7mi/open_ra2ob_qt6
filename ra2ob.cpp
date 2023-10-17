#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <memory>
#include <Windows.h>
#include <TlHelp32.h>   //for PROCESSENTRY32, needs to be included after windows.h
#include <locale>
#include <codecvt>

#include "ra2ob.h"

using json = nlohmann::json;

Ra2ob::Ra2ob() {
    _pHandle = nullptr;
    _strName = StrName();
    _strCountry = StrCountry();
    _view = View();

    _players        = std::vector<bool>(MAXPLAYER, false);
    _playerBases    = std::vector<uint32_t>(MAXPLAYER, 0);
    _buildings      = std::vector<uint32_t>(MAXPLAYER, 0);
    _infantrys      = std::vector<uint32_t>(MAXPLAYER, 0);
    _tanks          = std::vector<uint32_t>(MAXPLAYER, 0);
    _aircrafts      = std::vector<uint32_t>(MAXPLAYER, 0);
    _houseTypes     = std::vector<uint32_t>(MAXPLAYER, 0);
    _factionTypes   = std::vector<FactionType>(MAXPLAYER, FactionType::Unknown);
}

Ra2ob::~Ra2ob() {
    if (_pHandle != nullptr) {
        CloseHandle(_pHandle);
    }
}

Ra2ob::View::View(std::string jsonFile) {
    loadFromJson(jsonFile);
}

Ra2ob::View::~View() {}

void Ra2ob::View::loadFromJson(std::string jsonFile) {
    std::vector<std::string> ret;
    std::ifstream f(jsonFile);
    json data = json::parse(f);

    m_viewType = ViewType(data["ViewType"]);

    for (std::string it : data["NumericItems"]) {
        json jsonArray = json::array();

        for (int i = 0; i < MAXPLAYER; i++) {
            jsonArray.push_back("0");
        }
        m_numericView[it] = jsonArray;
    }

    for (auto& it : data["UnitItems"]["DefaultView"]) {
        std::string key = it["Name"];
        //int index = it["Index"];
        json jsonArray = json::array();

        for (int i = 0; i < MAXPLAYER; i++) {
            jsonArray.push_back("0");
        }

        m_unitView[key] = jsonArray;
        //m_order[key] = index;
    }
}

void Ra2ob::View::refreshView(std::string key, std::string value, int index) {
    if (m_numericView.contains(key)) {
        m_numericView[key][index] = value;
    }
    else if (m_unitView.contains(key)) {
        m_unitView[key][index] = value;
    }
}

void Ra2ob::View::sortView() {}

std::string Ra2ob::View::viewToString() {
    std::stringstream ss;

    sortView();

    for (int i = 0; i < MAXPLAYER; i++) {

        ss << std::endl;

        for (auto& it : m_numericView.items()) {
            auto v = it.value();
            if (v[i] != "0" && v[i] != "") {
                ss << it.key() << ": " << v[i] << std::endl;
            }
        }

        for (auto& it : m_unitView.items()) {
            auto v = it.value();
            if (m_viewType == ViewType::Auto || m_viewType == ViewType::ManualNoZero) {

                if (v[i] != "0" && v[i] != "") {
                    ss << it.key() << ": " << v[i] << std::endl;
                }
            }
            else {
                ss << it.key() << ": " << v[i] << std::endl;
            }
        }
    }

    return ss.str();
}

Ra2ob::DataBase::DataBase(std::string name, uint32_t offset) {
    m_name = name;
    m_offset = offset;
    m_value = std::vector<uint32_t>(MAXPLAYER, 0);
    m_size = NUMSIZE;
}

Ra2ob::DataBase::~DataBase() {}

void Ra2ob::DataBase::showInfo() {
    std::cout << "Data name: " << m_name << std::endl;
    std::cout << "Offset: 0x" << std::hex << m_offset << std::endl;
    std::cout << "Size: " << m_size << std::endl;
}

std::string Ra2ob::DataBase::getName() {
    return m_name;
}

uint32_t Ra2ob::DataBase::getValueByIndex(int index) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
        return -1;
    }
    return m_value[index];
}

void Ra2ob::DataBase::setValueByIndex(int index, uint32_t value) {
    if (index >= MAXPLAYER) {
        std::cerr << "Error: Index cannot be larger than MAXPLAYER." << std::endl;
    }
    m_value[index] = value;
}

void Ra2ob::DataBase::fetchData(HANDLE pHandle, std::vector<uint32_t> baseOffsets) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        uint32_t buf = 0;

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);
        m_value[i] = buf;
    }
}

Ra2ob::Numeric::Numeric(std::string name, uint32_t offset)
    : DataBase(name, offset) {}

Ra2ob::Numeric::~Numeric() {}

Ra2ob::Unit::Unit(
    std::string name,
    uint32_t offset,
    FactionType ft,
    UnitType ut
) : Ra2ob::DataBase(name, offset) {
    m_factionType = ft;
    m_unitType = ut;
}

Ra2ob::Unit::~Unit() {}

Ra2ob::FactionType Ra2ob::Unit::getFactionType() {
    return m_factionType;
}

Ra2ob::UnitType Ra2ob::Unit::getUnitType() {
    return m_unitType;
}

Ra2ob::StrName::StrName(std::string name, uint32_t offset)
    : Ra2ob::DataBase(name, offset) {
    m_value = std::vector<std::string>(MAXPLAYER, "");
    m_size = STRNAMESIZE;
}

Ra2ob::StrName::~StrName() {}

void Ra2ob::StrName::fetchData(
    HANDLE pHandle,
    std::vector<uint32_t> baseOffsets
) {
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

Ra2ob::StrCountry::StrCountry(std::string name, uint32_t offset)
    : StrName(name, offset) {
    m_size = STRCOUNTRYSIZE;
}

Ra2ob::StrCountry::~StrCountry() {}

void Ra2ob::StrCountry::fetchData(
    HANDLE pHandle,
    std::vector<uint32_t> baseOffsets
) {
    for (int i = 0; i < baseOffsets.size(); i++) {
        if (baseOffsets[i] == 0) {
            continue;
        }

        char buf[STRCOUNTRYSIZE] = "\0";

        readMemory(pHandle, baseOffsets[i] + m_offset, &buf, m_size);

        if (m_countryMap.find(buf) == m_countryMap.end()) {
            m_value[i] = "";
        }
        else {
            m_value[i] = m_countryMap[buf];
        }
    }
}

Ra2ob::WinOrLose::WinOrLose(std::string name, uint32_t offset)
    : Ra2ob::DataBase(name, offset) {
    m_value = std::vector<bool>(MAXPLAYER, false);
    m_size = BOOLSIZE;
}

Ra2ob::WinOrLose::~WinOrLose() {}

void Ra2ob::WinOrLose::fetchData(
    HANDLE pHandle,
    std::vector<uint32_t> baseOffsets
) {
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
    json data = json::parse(f);

    for (auto& it : data) {
        std::string offset = it["Offset"];
        uint32_t s_offset = std::stoul(offset, nullptr, 16);
        Numeric n(it["Name"], s_offset);
        numerics.push_back(n);
    }

    return numerics;
}

Ra2ob::Units Ra2ob::loadUnitsFromJson(std::string filePath) {
    Units units;

    std::ifstream f(filePath);
    json data = json::parse(f);

    for (auto& ft : data.items()) {
        FactionType s_ft;

        if (ft.key() == "Soviet") {
            s_ft = FactionType::Soviet;
        }
        else if (ft.key() == "Allied") {
            s_ft = FactionType::Allied;
        }
        else {
            s_ft = FactionType::Unknown;
        }

        for (auto& ut : data[ft.key()].items()) {
            UnitType s_ut;

            if (ut.key() == "Building") {
                s_ut = UnitType::Building;
            }
            else if (ut.key() == "Tank") {
                s_ut = UnitType::Tank;
            }
            else if (ut.key() == "Infantry") {
                s_ut = UnitType::Infantry;
            }
            else if (ut.key() == "Aircraft") {
                s_ut = UnitType::Aircraft;
            }
            else {
                s_ut = UnitType::Unknown;
            }

            for (auto& u : data[ft.key()][ut.key()]) {

                if (u.empty()) {
                    continue;
                }

                std::string offset = u["Offset"];
                uint32_t s_offset = std::stoul(offset, nullptr, 16);
                Unit ub(u["Name"], s_offset, s_ft, s_ut);
                units.push_back(ub);
            }
        }
    }

    return units;
}

std::vector<std::string> Ra2ob::loadViewsFromJson(std::string filePath) {
    std::vector<std::string> ret;
    std::ifstream f(filePath);
    json data = json::parse(f);

    for (auto& v : data) {
        ret.push_back(v);
    }

    return ret;
}

void Ra2ob::initDatas() {
    _numerics = loadNumericsFromJson();
    _units = loadUnitsFromJson();
    _views = loadViewsFromJson();
}

bool Ra2ob::initAddrs() {
    if (NULL == _pHandle) {
        std::cerr << "No valid process handle, call Game::getHandle() first." << std::endl;
        return false;
    }

    uint32_t fixed = getAddr(FIXEDOFFSET);
    uint32_t classBaseArray = getAddr(CLASSBASEARRAYOFFSET);
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
            _buildings[i]   = getAddr(realPlayerBase + BUILDINGOFFSET);
            _tanks[i]       = getAddr(realPlayerBase + TANKOFFSET);
            _infantrys[i]   = getAddr(realPlayerBase + INFANTRYOFFSET);
            _aircrafts[i]   = getAddr(realPlayerBase + AIRCRAFTOFFSET);
            _houseTypes[i]  = getAddr(realPlayerBase + HOUSETYPEOFFSET);

            if (
                _buildings[i]   == 1 &&
                _tanks[i]       == 1 &&
                _infantrys[i]   == 1 &&
                _aircrafts[i]   == 1 &&
                _houseTypes[i]  == 1
            ) {
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
            it.fetchData(_pHandle, _buildings);
        }
        else if (it.getUnitType() == UnitType::Infantry) {
            it.fetchData(_pHandle, _infantrys);
        }
        else if (it.getUnitType() == UnitType::Tank) {
            it.fetchData(_pHandle, _tanks);
        }
        else {
            it.fetchData(_pHandle, _aircrafts);
        }
    }

    _strName.fetchData(_pHandle, _playerBases);
    _strCountry.fetchData(_pHandle, _houseTypes);

    return true;
}

void Ra2ob::exportInfo() {
    for (int i = 0; i < MAXPLAYER; i++) {
        if (!_players[i]) {
            continue;
        }

        if (_strCountry.getValueByIndex(i) == "") {
            continue;
        }

        _view.refreshView(_strName.getName(), _strName.getValueByIndex(i), i);
        _view.refreshView(_strCountry.getName(), _strCountry.getValueByIndex(i), i);

        if (countryToFaction(_strCountry.getValueByIndex(i)) == FactionType::Allied) {
            _factionTypes[i] = FactionType::Allied;
        }
        else {
            _factionTypes[i] = FactionType::Soviet;
        }

        for (auto& it : _numerics) {
            if (_view.m_numericView.find(it.getName()) != _view.m_numericView.end()) {
                _view.refreshView(it.getName(), std::to_string(it.getValueByIndex(i)), i);
            }
        }
        for (auto& it : _units) {
            // Todo: Add support for opposite faction units.
            if (it.getFactionType() != _factionTypes[i]) {
                continue;
            }
            if (_view.m_unitView.find(it.getName()) != _view.m_unitView.end()) {
                int unitNum = it.getValueByIndex(i);

                if (unitNum < UNITSAFE) {
                    _view.refreshView(it.getName(), std::to_string(unitNum), i);
                }
            }
        }
    }

    std::cout << _view.viewToString() << std::endl;
}

int Ra2ob::getHandle() {
    DWORD pid = 0;
    // Use this if something goes wrong here.
    std::wstring name = L"gamemd-spawn.exe";
    //std::string name = "gamemd-spawn.exe";

    std::unique_ptr<void, decltype(&CloseHandle)> h(
        CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0),
        &CloseHandle
    );

    if (h.get() == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create snapshot" << std::endl;
        return 1;
    }

    PROCESSENTRY32 processInfo {};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    for (BOOL success = Process32First(h.get(), &processInfo); success; success = Process32Next(h.get(), &processInfo)) {
        // Use this if something goes wrong here.
        if (wcscmp(processInfo.szExeFile, name.c_str()) == 0) {
            pid = processInfo.th32ProcessID;
            std::cout << "PID Found: " << pid << std::endl;
        }
    }

    if (pid == 0) {
        std::cerr << "No Valid PID. Finding \"gamemd-spawn.exe\"." << std::endl;
        return 1;
    }

    HANDLE pHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION |     // Needed to get a process' token
        PROCESS_CREATE_THREAD   |     // For obvious reasons
        PROCESS_VM_OPERATION    |      // Required to perform operations on address space of process (like WriteProcessMemory)
        PROCESS_VM_READ,            // Required for read data
        FALSE,                          // Don't inherit pHandle
        pid
    );

    if (pHandle == NULL)
    {
        std::cerr << "Could not open process\n" << std::endl;
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

Ra2ob::FactionType Ra2ob::countryToFaction(std::string country) {
    if (
        country == "Americans"  ||
        country == "Korea"      ||
        country == "French"     ||
        country == "Germans"    ||
        country == "British"
    ) {
        return FactionType::Allied;
    }
    return FactionType::Soviet;
}

bool Ra2ob::readMemory(HANDLE handle, uint32_t addr, void* value, uint32_t size) {
    return ReadProcessMemory(handle, (const void*)addr, value, size, nullptr);
}
