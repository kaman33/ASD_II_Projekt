#include "Krasnal.h"

Krasnal::Krasnal() : id(0), xDom(0), yDom(0), kompetencje({}), ulubionySurowiec(""), czyUlubionySurowiec(false) {}

Krasnal::Krasnal(int id, int xDom, int yDom,
                 const std::vector<std::string>& kompetencje,
                 const std::string& ulubionySurowiec,
                 bool czyUlubionySurowiec)
    : id(id),
      xDom(xDom),
      yDom(yDom),
      kompetencje(kompetencje),
      ulubionySurowiec(ulubionySurowiec),
      czyUlubionySurowiec(czyUlubionySurowiec) {}

int Krasnal::getId() const{
  return id;
}

int Krasnal::getYDom() const{
  return yDom;
}

int Krasnal::getXDom() const{
    return xDom;
}

const std::vector<std::string>& Krasnal::getKompetencje() const{
  return kompetencje;
}

const std::string& Krasnal::getUlubionySurowiec() const{
  return ulubionySurowiec;
}

bool Krasnal::isUlubionySurowiec() const{
  return czyUlubionySurowiec;
}

void Krasnal::setId(int noweId){
  this->id = noweId;
}

void Krasnal::setXDom(int noweX){
  this->xDom = noweX;
}
void Krasnal::setYDom(int noweY){
  this->yDom = noweY;
}
void Krasnal::setKompetencje(const std::vector<std::string>& noweKompetencje){
  this->kompetencje = noweKompetencje;
}
void Krasnal::setUlubionySurowiec(const std::string& nowySurowiec){
  this->ulubionySurowiec = nowySurowiec;
}
void Krasnal::setCzyUlubionySurowiec(bool czyUlubionySurowiec){
  this->czyUlubionySurowiec = czyUlubionySurowiec;
}
