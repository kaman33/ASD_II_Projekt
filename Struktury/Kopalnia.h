#ifndef KOPALNIA_H
#define KOPALNIA_H

#include "Point.h"
#include <string>
#include <vector>
#include <algorithm>

class Kopalnia {
private:
    int id;
    Point lokalizacja;
    std::string typSurowca;
    int pojemnosc;
    std::vector<int> przypisaneKrasnale;

public:
    Kopalnia();

    Kopalnia(int id, const Point& lokalizacja, const std::string& typSurowca,
             int pojemnosc, const std::vector<int>& przypisaneKrasnale);

    int getId() const;
    const Point& getLokalizacja() const;
    double getX() const;
    double getY() const;
    const std::string& getTypSurowca() const;
    int getPojemnosc() const;
    const std::vector<int>& getPrzypisaneKrasnale() const;

    void setId(int noweId);
    void setLokalizacja(const Point& nowaLokalizacja);
    void setX(double noweX);
    void setY(double noweY);
    void setTypSurowca(const std::string& nowyTypSurowca);
    void setPojemnosc(int nowaPojemnosc);
    void setPrzypisaneKrasnale(const std::vector<int>& nowePrzypisaneKrasnale);

    int getDostepnaPojemnosc() const;
    bool maWolneMiejsce() const;
    bool czyUzywana() const;
    void dodajKrasnala(int idKrasnala);
    void usunKrasnala(int idKrasnala);
};

#endif
