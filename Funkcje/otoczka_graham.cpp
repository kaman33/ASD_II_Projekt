#include <algorithm>
#include <iostream>
#include "det.cpp"

float dystans(const Punkt a, const Punkt b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int main() {

    std::vector<Punkt> punkty;

    /// PRZYKŁADOWE PUNKTY Z WYKŁADU
    punkty.push_back(Punkt(0, 0));
    punkty.push_back(Punkt(1, 0));
    punkty.push_back(Punkt(2, 0));
    punkty.push_back(Punkt(2, 1));
    punkty.push_back(Punkt(4, 2));
    punkty.push_back(Punkt(3, 4));
    punkty.push_back(Punkt(1, 3));
    punkty.push_back(Punkt(5, 1));
    punkty.push_back(Punkt(3, 0));
    punkty.push_back(Punkt(2, 2));

    /// ALGORYTM

    /// KROK 1: SORTUJEMY I DOSTOSOWUJEMY LISTĘ PUNKTÓW ZE WZGLĘDU NA WSPÓŁRZĘDNE KĄTOWE

    /// NAJPIERW SORTUJEMY LISTĘ PUNKTÓW
    std::sort(punkty.begin() + 1, punkty.end(), [&](const Punkt& pi, const Punkt& pj) {
        int d = det(punkty[0], pi, pj);
        if (d == 0) return dystans(punkty[0], pi) < dystans(punkty[0], pj);
        return d > 0;
    });

    /// USUWAMY Z LISTY BLIŻSZE PUNKTY Z TĄ SAMĄ WSPÓŁRZĘDNĄ KĄTOWĄ CO DALSZY PUNKT
    std::vector<Punkt> nowePunkty;
    nowePunkty.push_back(punkty[0]);

    for(int i = 1; i < punkty.size(); i++){
        while(i < punkty.size() - 1 && det(punkty[0], punkty[i], punkty[i+1]) == 0)
            i++;
        nowePunkty.push_back(punkty[i]);
    }

    punkty = nowePunkty;

    /// WYPISANIE PUNKTÓW
    /*
    for (int i = 0; i < punkty.size(); i++) {
        std::cout << "(" << punkty[i].x << ", " << punkty[i].y << ")\n";
    }
    */

    /// KROK 2: UTWORZENIE STOSU I DODANIE 3 PIERWSZYCH PUNKTÓW

    std::vector<Punkt> stos;
    stos.push_back(punkty[0]);
    stos.push_back(punkty[1]);
    stos.push_back(punkty[2]);

    /// KROK 3: OSTATECZNE DOBRANIE PUNKTÓW OTOCZKI

    for (int i = 3; i < punkty.size(); i++) {
        while (stos.size() >= 2 && det(stos[stos.size() - 2], stos.back(), punkty[i]) < 0) {
            stos.pop_back();
        }
        stos.push_back(punkty[i]);
    }

    /// WYPISANIE WYNIKU

    for (int i = 0; i < stos.size(); i++) {
        std::cout << "(" << stos[i].x << ", " << stos[i].y << ")\n";
    }

}