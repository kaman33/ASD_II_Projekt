#include "Parser/DataLoader.h"
#include <exception>
#include <iostream>

using namespace std;

int main() {
    try {
        DataLoader loader;
        InputData data = loader.loadFromFile("../Dane/dane.txt");

        cout << "Krasnale: " << data.dwarves.size() << '\n';
        cout << "Kopalnie: " << data.mines.size() << '\n';
        cout << "Straznicy: " << data.guards.size() << '\n';
    }
    catch (const std::exception& e) {
        cerr << "Blad: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
