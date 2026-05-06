#include "Straznik.h"

Straznik::Straznik() : id(0), loudness(0) {}

Straznik::Straznik(int id, int loudness) : id(id), loudness(loudness) {}

int Straznik::getId() const {
    return id;
}

int Straznik::getLoudness() const {
    return loudness;
}

void Straznik::setLoudness(int newLoudness) {
    this->loudness = newLoudness;
}

void Straznik::setId(int newId) {
    this->id = newId;
}
