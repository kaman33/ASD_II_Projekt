#ifndef STRAZNIK_H
#define STRAZNIK_H

class Straznik {
private:
    int id;
    int loudness;

public:
    Straznik();

    Straznik(int id, int loudness);
    int getId() const;
    int getLoudness() const;

    void setLoudness(int newLoudness);
    void setId(int newId);
};

#endif
