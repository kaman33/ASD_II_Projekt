#include "Straznik.h"

Straznik::Straznik() : id(0), glosnosc(0) {}

Straznik::Straznik(int id, int glosnosc) : id(id), glosnosc(glosnosc) {}

int Straznik::getId() const {
    return id;
}

int Straznik::getGlosnosc() const {
    return glosnosc;
}

void Straznik::setGlosnosc(int nowaGlosnosc) {
    this->glosnosc = nowaGlosnosc;
}

void Straznik::setId(int noweId) {
    this->id = noweId;
}