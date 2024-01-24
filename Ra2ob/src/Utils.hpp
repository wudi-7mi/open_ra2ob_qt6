#ifndef RA2OB_SRC_UTILS_HPP_
#define RA2OB_SRC_UTILS_HPP_

#include <Windows.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./third_party/inicpp.hpp"
#include "./third_party/json.hpp"

using json = nlohmann::json;

namespace Ra2ob {

inline json readJsonFromFile(std::string filePath) {
    std::ifstream f(filePath);
    json j;

    try {
        j = json::parse(f);
    } catch (json::parse_error err) {
        std::cerr << filePath << " parse error.\n";
        std::exit(1);
    }

    return j;
}

class IniFile {
public:
    IniFile(std::string filePath, std::string seg);

    bool isItemExist(std::string item);
    std::string getItem(std::string item);

private:
    inicpp::IniManager* im;
    std::string m_filePath;
    std::string m_seg;
};

inline IniFile::IniFile(std::string filePath, std::string seg) {
    m_filePath = filePath;
    m_seg      = seg;
    im         = new inicpp::IniManager(m_filePath);
}

inline bool IniFile::isItemExist(std::string item) { return ((*im)[m_seg].isKeyExist(item)); }

inline std::string IniFile::getItem(std::string item) { return ((*im)[m_seg][item]); }

inline std::string utf16ToGbk(const wchar_t* src_wstr) {
    int len = WideCharToMultiByte(CP_ACP, 0, src_wstr, -1, nullptr, 0, nullptr, nullptr);

    std::vector<char> str(len);

    WideCharToMultiByte(CP_ACP, 0, src_wstr, -1, &str[0], len, nullptr, nullptr);

    return std::string(str.begin(), str.end() - 1);
}

inline std::string utf16ToUtf8(const wchar_t* src_wstr) {
    int len = WideCharToMultiByte(CP_UTF8, 0, src_wstr, -1, nullptr, 0, nullptr, nullptr);

    std::vector<char> str(len);

    WideCharToMultiByte(CP_UTF8, 0, src_wstr, -1, &str[0], len, nullptr, nullptr);

    return std::string(str.begin(), str.end() - 1);
}

inline std::wstring gbkToUtf16(const char* src_str) {
    int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, nullptr, 0);

    std::vector<wchar_t> wstr(len);

    MultiByteToWideChar(CP_ACP, 0, src_str, -1, &wstr[0], len);

    return std::wstring(wstr.begin(), wstr.end() - 1);
}

inline std::string convertFrameToTimeString(int frame, int framePerSecond) {
    int totalSeconds = frame / framePerSecond;

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;

    return ss.str();
}

}  // end of namespace Ra2ob

#endif  // RA2OB_SRC_UTILS_HPP_
