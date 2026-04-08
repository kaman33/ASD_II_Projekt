#include "Parser/DataLoader.h"
#include <exception>
#include <iostream>

using namespace std;

int main() {
    try {
        DataLoader loader;
        InputData data = loader.loadFromFile("../Dane/dane.txt");

        cout << "Krasnale: " << data.krasnale.size() << '\n';
        cout << "Kopalnie: " << data.kopalnie.size() << '\n';
        cout << "Straznicy: " << data.straznicy.size() << '\n';
    }
    catch (const std::exception& e) {
        cerr << "Blad: " << e.what() << '\n';
        return 1;
    }

    return 0;
}