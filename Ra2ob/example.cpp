#include "Ra2ob"

int main() {
    Ra2ob::Game& g = Ra2ob::Game::getInstance();

    g.startLoop();

    while (true) {
        Sleep(1000);

        if (g._gameInfo.valid) {
            system("cls");
            g.viewer.print(g._gameInfo, 0);
        }
    }

    return 0;
}
