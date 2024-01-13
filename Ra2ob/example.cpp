#include "Ra2ob"

int main(int argc, char* argv[]) {
    int runMode = 0;

    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            if (std::strcmp(argv[i], "debug") == 0) {
                runMode = 2;
            }
        }
    }

    Ra2ob::Game& g = Ra2ob::Game::getInstance();

    g.startLoop();

    while (true) {
        Sleep(Ra2ob::T_PRINTTIME);

        if (g._gameInfo.valid) {
            system("cls");
            g.viewer.print(g._gameInfo, runMode);
        }
    }

    return 0;
}
