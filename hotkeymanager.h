#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include <QAbstractNativeEventFilter>
#include <QVector>

class NEFilter : public QAbstractNativeEventFilter {
public:
    NEFilter(const unsigned int mod, const unsigned int key);

    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
    unsigned int mod, key;
};

class HotkeyManager {
public:
    HotkeyManager();

    void defineHotkey();
    void registerHotkey(const NEFilter& filter);
    void batchRegister();
};

#endif  // HOTKEYMANAGER_H
