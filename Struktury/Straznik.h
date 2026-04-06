#ifndef STRAZNIK_H
#define STRAZNIK_H

#include "Krasnal.h"

class Straznik : public Krasnal {
private:
    int glosnosc;

public:
    Straznik();

    Straznik(int id, const Point& dom,
             const std::vector<std::string>& kompetencje,
             const std::string& ulubionySurowiec,
             bool czyUlubionySurowiec, int glosnosc);

    int getGlosnosc() const;

    void setGlosnosc(int nowaGlosnosc);
};

#endif
