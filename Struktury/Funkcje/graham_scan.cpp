#include "graham_scan.h"

// https://en.wikipedia.org/wiki/Graham_scan
std::vector<Point> graham_scan(std::vector<Point> points){
    /// KROK 1: USUNIECIE DUPLIKATOW I USTAWIENIE NAJBLIZSZEGO
    ///         PUNKTU JAKO PIERWSZY (NAJPIERW NA Y, W RAZIE REMISU NA X)

    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        int cmpY = cmpDouble(a.y, b.y);
        if (cmpY != 0) return cmpY < 0;
        return cmpDouble(a.x, b.x) < 0;
    });

    points.erase(std::unique(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return cmpDouble(a.x, b.x) == 0 && cmpDouble(a.y, b.y) == 0;
    }), points.end());

    /// PRZYPADKI, GDY JEST PONIZEJ 3 PUNKTOW
    if (points.size() < 3) {
        if (points.size() == 0) return {};
        if(points.size() == 1 || points.size() == 2) return points;
    }

    /// KROK 2: SORTUJEMY I DOSTOSOWUJEMY LISTE PUNKTOW ZE WZGLEDU NA WSPOLRZEDNE KATOWE

    /// NAJPIERW SORTUJEMY LISTE PUNKTOW
    std::sort(points.begin() + 1, points.end(), [&](const Point& pi, const Point& pj){
        double d = det(points[0], pi, pj);
        if (sgn(d) == 0) {
            int cmpDist = cmpDouble(distance(points[0], pi), distance(points[0], pj));
            if (cmpDist != 0) return cmpDist < 0;
            int cmpX = cmpDouble(pi.x, pj.x);
            if (cmpX != 0) return cmpX < 0;
            return cmpDouble(pi.y, pj.y) < 0;
        }
        return sgn(d) > 0;
    });

    /// USUWAMY Z LISTY BLIZSZE PUNKTY Z TA SAMA WSPOLRZEDNA KATOWA CO DALSZY PUNKT
    std::vector<Point> furtherPoints;
    furtherPoints.push_back(points[0]);

    for(unsigned int i = 1; i < points.size(); i++){
        while(i + 1 < points.size() && sgn(det(points[0], points[i], points[i+1])) == 0)
            i++;
        furtherPoints.push_back(points[i]);
    }
    
    points = furtherPoints;

    /// KROK 3: UTWORZENIE STOSU I DODANIE 3 PIERWSZYCH PUNKTOW

    if (points.size() < 3) return points;

    std::vector<Point> stack;
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    stack.push_back(points[2]);

    /// KROK 4: OSTATECZNE DOBRANIE PUNKTOW OTOCZKI

    for (unsigned int i = 3; i < points.size(); i++) {
        while (stack.size() >= 2 && sgn(det(stack[stack.size() - 2], stack.back(), points[i])) <= 0) {
            stack.pop_back();
        }
        stack.push_back(points[i]);
    }

    return stack;
}
