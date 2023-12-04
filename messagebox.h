#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QApplication>
#include <QMessageBox>

class MessageBox {
public:
    static void warning(const QString& title, const QString& text) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    static int question(const QString& title, const QString& text) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msgBox.exec();
        return ret;
    }

    static void error(const QString& title, const QString& text) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    static void errorQuit(const QString& title, const QString& text) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        qApp->quit();
    }
};

#endif  // MESSAGEBOX_H
