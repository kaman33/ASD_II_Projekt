#include <algorithm>
#include <iostream>
#include <vector>
#include <float.h>

#include "det.h"

int nearestIdx(std::vector<Punkt> &points) {
    int best = 0;

    for (int i = 1; i < points.size(); i++) {
        if (points[i].y < points[best].y || 
           (points[i].y == points[best].y && points[i].x < points[best].x)) {
            best = i;
        }
    }
    return best;
}

float distance(Punkt a, Punkt b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int main() {

    std::vector<Punkt> points;

    /// PRZYKŁADOWE PUNKTY Z WYKŁADU
    points.push_back(Punkt(0, 0));
    points.push_back(Punkt(1, 0));
    points.push_back(Punkt(2, 0));
    points.push_back(Punkt(2, 1));
    points.push_back(Punkt(4, 2));
    points.push_back(Punkt(3, 4));
    points.push_back(Punkt(1, 3));
    points.push_back(Punkt(5, 1));
    points.push_back(Punkt(3, 0));
    points.push_back(Punkt(2, 2));

    if (points.size() < 4) {
        std::cout << "[ERROR] W zbiorze muszą być więcej niż 3 punkty\n";
        return 1;
    }

    /// USTAWIENIE NAJBLIŻSZEGO PUNKTU JAKO PIERWSZY (NAJPIERW NA Y, W RAZIE REMISU NA X)
    std::swap(points[nearestIdx(points)], points[0]);

    /// ALGORYTM

    /// KROK 1: SORTUJEMY I DOSTOSOWUJEMY LISTĘ PUNKTÓW ZE WZGLĘDU NA WSPÓŁRZĘDNE KĄTOWE

    /// NAJPIERW SORTUJEMY LISTĘ PUNKTÓW
    std::sort(points.begin() + 1, points.end(), [&](const Punkt& pi, const Punkt& pj) {
        float d = det(points[0], pi, pj);
        if (d == 0) return distance(points[0], pi) < distance(points[0], pj);
        return d > 0;
    });

    /// USUWAMY Z LISTY BLIŻSZE PUNKTY Z TĄ SAMĄ WSPÓŁRZĘDNĄ KĄTOWĄ CO DALSZY PUNKT
    std::vector<Punkt> furtherPoints;
    furtherPoints.push_back(points[0]);

    for(int i = 1; i < points.size(); i++){
        while(i < points.size() - 1 && det(points[0], points[i], points[i+1]) == 0)
            i++;
        furtherPoints.push_back(points[i]);
    }

    points = furtherPoints;

    /// KROK 2: UTWORZENIE STOSU I DODANIE 3 PIERWSZYCH PUNKTÓW

    std::vector<Punkt> stack;
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    stack.push_back(points[2]);

    /// KROK 3: OSTATECZNE DOBRANIE PUNKTÓW OTOCZKI

    for (int i = 3; i < points.size(); i++) {
        while (stack.size() >= 2 && det(stack[stack.size() - 2], stack.back(), points[i]) < 0) {
            stack.pop_back();
        }
        stack.push_back(points[i]);
    }

    /// WYPISANIE WYNIKU - PUNKTY TWORZĄCE OTOCZKĘ ZE ZBIORU PUNKTÓW

    for (auto& point : stack) {
        std::cout << point << "\n";
    }

}
