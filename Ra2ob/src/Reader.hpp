#ifndef RA2OB_SRC_READER_HPP_
#define RA2OB_SRC_READER_HPP_

#include <Windows.h>

#include <memory>
#include <string>

#include "./Constants.hpp"

namespace Ra2ob {

class Reader {
public:
    explicit Reader(HANDLE handle = nullptr);

    HANDLE getHandle();
    bool readMemory(uint32_t addr, void* value, uint32_t size);
    uint32_t getAddr(uint32_t offset);
    int getInt(uint32_t offset);
    bool getBool(uint32_t offset);
    std::string getString(uint32_t offset);
    uint32_t getColor(uint32_t offset);

protected:
    HANDLE m_handle;
};

inline Reader::Reader(HANDLE handle) { m_handle = handle; }

inline HANDLE Reader::getHandle() { return m_handle; }

inline bool Reader::readMemory(uint32_t addr, void* value, uint32_t size) {
    return ReadProcessMemory(m_handle, (const void*)addr, value, size, nullptr);
}

inline uint32_t Reader::getAddr(uint32_t offset) {
    uint32_t buf = 0;

    if (!readMemory(offset, &buf, 4)) {
        return 1;
    }

    return buf;
}

inline int Reader::getInt(uint32_t offset) {
    int buf = 0;

    if (!readMemory(offset, &buf, 4)) {
        return -1;
    }

    return buf;
}

inline bool Reader::getBool(uint32_t offset) {
    bool buf = false;

    if (!readMemory(offset, &buf, 1)) {
        return false;
    }

    return buf;
}

inline std::string Reader::getString(uint32_t offset) {
    char buf[STRUNITNAMESIZE] = "\0";

    readMemory(offset, &buf, STRUNITNAMESIZE);

    std::string ret = buf;

    return ret;
}

inline uint32_t Reader::getColor(uint32_t offset) {
    uint32_t buf = 0;

    readMemory(offset, &buf, 3);

    return buf;
}

}  // end of namespace Ra2ob

#endif  // RA2OB_SRC_READER_HPP_
