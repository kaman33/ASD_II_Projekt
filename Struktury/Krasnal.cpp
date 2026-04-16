#include "Krasnal.h"

Krasnal::Krasnal() : id(0), dom(0.0, 0.0), kompetencje({}), ulubionySurowiec(""), czyUlubionySurowiec(false) {}

Krasnal::Krasnal(int id, const Point& dom,
                 const std::vector<std::string>& kompetencje,
                 const std::string& ulubionySurowiec,
                 bool czyUlubionySurowiec)
    : id(id),
      dom(dom),
      kompetencje(kompetencje),
      ulubionySurowiec(ulubionySurowiec),
      czyUlubionySurowiec(czyUlubionySurowiec) {}

int Krasnal::getId() const{
  return id;
}

const Point& Krasnal::getDom() const{
  return dom;
}

double Krasnal::getYDom() const{
  return dom.y;
}

double Krasnal::getXDom() const{
    return dom.x;
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

void Krasnal::setDom(const Point& nowyDom){
  this->dom = nowyDom;
}

void Krasnal::setXDom(double noweX){
  this->dom.x = noweX;
}

void Krasnal::setYDom(double noweY){
  this->dom.y = noweY;
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
