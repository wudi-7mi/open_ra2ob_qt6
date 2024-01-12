#include "./hotkeymanager.h"

#include <windows.h>

#include <iostream>

NEFilter::NEFilter() { gls = &Globalsetting::getInstance(); }

bool NEFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        const quint32 keycode   = HIWORD(msg->lParam);
        const quint32 modifiers = LOWORD(msg->lParam);

        switch (modifiers) {
            case MOD_CONTROL | MOD_ALT:
                switch (keycode) {
                    case 'J':
                        std::cerr << "ctrl+alt+j" << std::endl;
                        hideBottomPanel();
                        break;
                    case 'H':
                        std::cerr << "ctrl+alt+h" << std::endl;
                        hideOb();
                        break;
                    case VK_PRIOR:
                        std::cerr << "ctrl+alt+PgUp" << std::endl;
                        addOpacity();
                        break;
                    case VK_NEXT:
                        std::cerr << "ctrl+alt+PgDn" << std::endl;
                        minusOpacity();
                        break;
                    default:
                        break;
                }
        }
    }
    return false;
}

void NEFilter::addOpacity() {
    if (gls->c.top_panel_opacity != 1) {
        gls->c.top_panel_opacity += 0.1;
    }
}

void NEFilter::minusOpacity() {
    if (gls->c.top_panel_opacity != 0) {
        gls->c.top_panel_opacity -= 0.1;
    }
}

void NEFilter::hideOb() { gls->s.show_all_panel = !gls->s.show_all_panel; }

void NEFilter::hideBottomPanel() { gls->s.show_bottom_panel = !gls->s.show_bottom_panel; }

HotkeyManager::HotkeyManager() {}

void HotkeyManager::registerHotkey(const NEFilter& filter) {
    registerSingle(MOD_CONTROL | MOD_ALT, 'J');
    registerSingle(MOD_CONTROL | MOD_ALT, 'H');
    registerSingle(MOD_CONTROL | MOD_ALT, VK_PRIOR);
    registerSingle(MOD_CONTROL | MOD_ALT, VK_NEXT);
}

void HotkeyManager::registerSingle(const quint32& mod, const quint32& kc) {
    BOOL ok = RegisterHotKey(0, kc ^ mod, mod, kc);
    if (!ok) {
        std::cerr << "register shortcut failed!" << std::endl;
    }
}
