#include "Krasnal.h"
#include <iostream>

Krasnal::Krasnal()
    : id(0), imie(""), xDom(0), yDom(0), kompetencje({}), ulubionySurowiec(""), mnoznikPremii(1.0) {
}

//Do skończenia - ewentualnego poprawienia jeśli nie chcemy setterów