#include "./hotkeymanager.h"

#include <windows.h>

#include <iostream>

NEFilter::NEFilter(const unsigned int mod, const unsigned int key) {
    this->mod = mod;
    this->key = key;
}

bool NEFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        const quint32 keycode   = HIWORD(msg->lParam);
        const quint32 modifiers = LOWORD(msg->lParam);
        if (keycode == key && mod == modifiers) std::cout << "shortcut trigger!" << std::endl;
    }
    return false;
}

HotkeyManager::HotkeyManager() { defineHotkey(); }

void HotkeyManager::defineHotkey() {}

void HotkeyManager::registerHotkey(const NEFilter& filter) {
    int id  = filter.key ^ filter.mod;
    BOOL ok = RegisterHotKey(0, id, filter.mod, filter.key);
    if (!ok) {
        std::cerr << "register shortcut failed!" << std::endl;
    }
}

void HotkeyManager::batchRegister() {}
