#include "Kopalnia.h"

Kopalnia::Kopalnia()
    : id(0), lokalizacja(0.0f, 0.0f), typSurowca(""), pojemnosc(0), przypisaneKrasnale({}) {}

Kopalnia::Kopalnia(int id, const Point& lokalizacja, const std::string& typSurowca,
                   int pojemnosc, const std::vector<int>& przypisaneKrasnale)
    : id(id),
      lokalizacja(lokalizacja),
      typSurowca(typSurowca),
      pojemnosc(pojemnosc),
      przypisaneKrasnale(przypisaneKrasnale) {}

int Kopalnia::getId() const {
    return id;
}

const Point& Kopalnia::getLokalizacja() const {
    return lokalizacja;
}

double Kopalnia::getX() const {
    return lokalizacja.x;
}

double Kopalnia::getY() const {
    return lokalizacja.y;
}

const std::string& Kopalnia::getTypSurowca() const {
    return typSurowca;
}

int Kopalnia::getPojemnosc() const {
    return pojemnosc;
}

const std::vector<int>& Kopalnia::getPrzypisaneKrasnale() const {
    return przypisaneKrasnale;
}

void Kopalnia::setId(int noweId) {
    this->id = noweId;
}

void Kopalnia::setLokalizacja(const Point& nowaLokalizacja) {
    this->lokalizacja = nowaLokalizacja;
}

void Kopalnia::setX(double noweX) {
    this->lokalizacja.x = noweX;
}

void Kopalnia::setY(double noweY) {
    this->lokalizacja.y = noweY;
}

void Kopalnia::setTypSurowca(const std::string& nowyTypSurowca) {
    this->typSurowca = nowyTypSurowca;
}

void Kopalnia::setPojemnosc(int nowaPojemnosc) {
    this->pojemnosc = nowaPojemnosc;
}

void Kopalnia::setPrzypisaneKrasnale(const std::vector<int>& nowePrzypisaneKrasnale) {
    this->przypisaneKrasnale = nowePrzypisaneKrasnale;
}

int Kopalnia::getDostepnaPojemnosc() const {
    return pojemnosc - static_cast<int>(przypisaneKrasnale.size());
}

bool Kopalnia::maWolneMiejsce() const {
    return getDostepnaPojemnosc() > 0;
}

bool Kopalnia::czyUzywana() const {
    return !przypisaneKrasnale.empty();
}

void Kopalnia::dodajKrasnala(int idKrasnala) {
    if (!maWolneMiejsce()) {
        return;
    }

    if (std::find(przypisaneKrasnale.begin(), przypisaneKrasnale.end(), idKrasnala) != przypisaneKrasnale.end()) {
        return;
    }

    przypisaneKrasnale.push_back(idKrasnala);
}

void Kopalnia::usunKrasnala(int idKrasnala) {
    auto it = std::find(przypisaneKrasnale.begin(), przypisaneKrasnale.end(), idKrasnala);

    if (it != przypisaneKrasnale.end()) {
        przypisaneKrasnale.erase(it);
    }
}
