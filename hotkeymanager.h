#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include <QAbstractNativeEventFilter>
#include <QVector>

#include "./globalsetting.h"

class NEFilter : public QAbstractNativeEventFilter {
public:
    NEFilter();

    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
    void addOpacity();
    void minusOpacity();
    void hideOb();
    void hideBottomPanel();

    Globalsetting* gls;
};

class HotkeyManager {
public:
    HotkeyManager();

    void registerHotkey(const NEFilter& filter);
    void releaseHotkey();
    bool registerSingle(const quint32& mod, const quint32& kc);
    bool releaseSingle(const quint32& mod, const quint32& kc);
};

#endif  // HOTKEYMANAGER_H
