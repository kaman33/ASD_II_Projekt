#ifndef KRASNAL_H
#define KRASNAL_H

#include <string>
#include <vector>

class Krasnal {
private:
    int id;
    int xDom;
    int yDom;
    std::vector<std::string> kompetencje;
    std::string ulubionySurowiec;
    bool czyUlubionySurowiec;

public:
    Krasnal();

    Krasnal(int id, int xDom, int yDom,
            const std::vector<std::string>& kompetencje,
            const std::string& ulubionySurowiec, bool czyUlubionySurowiec);

    int getId() const;
    int getXDom() const;
    int getYDom() const;
    const std::vector<std::string>& getKompetencje() const;
    const std::string& getUlubionySurowiec() const;
    bool isUlubionySurowiec() const;

    void setId(int noweId);
    void setXDom(int noweX);
    void setYDom(int noweY);
    void setKompetencje(const std::vector<std::string>& noweKompetencje);
    void setUlubionySurowiec(const std::string& nowySurowiec);
    void setCzyUlubionySurowiec(bool czyUlubionySurowiec);
};

#endif
