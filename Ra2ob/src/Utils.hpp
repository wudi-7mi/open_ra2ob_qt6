#ifndef RA2OB_SRC_UTILS_HPP_
#define RA2OB_SRC_UTILS_HPP_

#include <Windows.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./json.hpp"

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

}  // end of namespace Ra2ob

#endif  // RA2OB_SRC_UTILS_HPP_
