#ifndef KRASNAL_H
#define KRASNAL_H

#include "Point.h"
#include <string>
#include <vector>

class Krasnal {
private:
    int id;
    Point dom;
    std::vector<std::string> kompetencje;
    std::string ulubionySurowiec;
    bool czyUlubionySurowiec;

public:
    Krasnal();

    Krasnal(int id, const Point& dom,
            const std::vector<std::string>& kompetencje,
            const std::string& ulubionySurowiec, bool czyUlubionySurowiec);

    int getId() const;
    const Point& getDom() const;
    float getXDom() const;
    float getYDom() const;
    const std::vector<std::string>& getKompetencje() const;
    const std::string& getUlubionySurowiec() const;
    bool isUlubionySurowiec() const;

    void setId(int noweId);
    void setDom(const Point& nowyDom);
    void setXDom(float noweX);
    void setYDom(float noweY);
    void setKompetencje(const std::vector<std::string>& noweKompetencje);
    void setUlubionySurowiec(const std::string& nowySurowiec);
    void setCzyUlubionySurowiec(bool czyUlubionySurowiec);
};

#endif
