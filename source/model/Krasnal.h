#ifndef KRASNAL_H
#define KRASNAL_H

#include <string>
#include <vector>

class Krasnal {
private:
    int id;
    std::string imie;
    int xDom;
    int yDom;
    std::vector<std::string> kompetencje;
    std::string ulubionySurowiec;
    double mnoznikPremii;

public:
    Krasnal();

    Krasnal(int id,
            const std::string& imie, int xDom, int yDom,
            const std::vector<std::string>& kompetencje,
            const std::string& ulubionySurowiec,
            double mnoznikPremii);

    int getId() const;
    const std::string& getImie() const;
    int getXDom() const;
    int getYDom() const;
    const std::vector<std::string>& getKompetencje() const;
    const std::string& getUlubionySurowiec() const;
    double getMnoznikPremii() const;

    void setId(int noweId);
    void setImie(const std::string& imie);
    void setXDom(int noweX);
    void setYDom(int noweY);
    void setKompetencje(const std::vector<std::string>& noweKompetencje);
    void setUlubionySurowiec(const std::string& nowySurowiec);
    void setMnoznikPremii(double nowyMnoznikPremii);
};

#endif