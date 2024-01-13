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
                        hideBottomPanel();
                        break;
                    case 'H':
                        hideOb();
                        break;
                    case VK_PRIOR:
                        addOpacity();
                        break;
                    case VK_NEXT:
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
    Globalsetting& gls = Globalsetting::getInstance();

    gls.s.sc_ctrl_alt_j        = registerSingle(MOD_CONTROL | MOD_ALT, 'J');
    gls.s.sc_ctrl_alt_h        = registerSingle(MOD_CONTROL | MOD_ALT, 'H');
    gls.s.sc_ctrl_alt_pageup   = registerSingle(MOD_CONTROL | MOD_ALT, VK_PRIOR);
    gls.s.sc_ctrl_alt_pagedown = registerSingle(MOD_CONTROL | MOD_ALT, VK_NEXT);
}

void HotkeyManager::releaseHotkey() {
    releaseSingle(MOD_CONTROL | MOD_ALT, 'J');
    releaseSingle(MOD_CONTROL | MOD_ALT, 'H');
    releaseSingle(MOD_CONTROL | MOD_ALT, VK_PRIOR);
    releaseSingle(MOD_CONTROL | MOD_ALT, VK_NEXT);
}

bool HotkeyManager::registerSingle(const quint32& mod, const quint32& kc) {
    BOOL ok = RegisterHotKey(0, kc ^ mod, mod, kc);
    if (!ok) {
        return false;
    }
    return true;
}

bool HotkeyManager::releaseSingle(const quint32& mod, const quint32& kc) {
    BOOL ok = UnregisterHotKey(0, kc ^ mod);
    if (!ok) {
        return false;
    }
    return true;
}
