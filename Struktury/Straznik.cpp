#include "Straznik.h"

Straznik::Straznik() : Krasnal(), glosnosc(0) {}

Straznik::Straznik(int id, const Point& dom,
                   const std::vector<std::string>& kompetencje,
                   const std::string& ulubionySurowiec,
                   bool czyUlubionySurowiec, int glosnosc)
    : Krasnal(id, dom, kompetencje, ulubionySurowiec, czyUlubionySurowiec),
      glosnosc(glosnosc) {}

int Straznik::getGlosnosc() const {
    return glosnosc;
}

void Straznik::setGlosnosc(int nowaGlosnosc) {
    this->glosnosc = nowaGlosnosc;
}
