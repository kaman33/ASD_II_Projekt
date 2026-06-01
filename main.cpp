#include "Struktury/Manager/KingdomManager.h"

int main() {
    try {
        KingdomManager manager;
        manager.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << '\n';
        return 1;
    }

    return 0;
}