#ifndef STRAZNIK_H
#define STRAZNIK_H

class Straznik {
private:
    int id;
    int glosnosc;

public:
    Straznik();

    Straznik(int id, int glosnosc);
    int getId() const;
    int getGlosnosc() const;

    void setGlosnosc(int nowaGlosnosc);
    void setId(int noweId);
};

#endif
