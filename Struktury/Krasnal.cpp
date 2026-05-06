#include "Krasnal.h"

Krasnal::Krasnal()
    : id(0), home(0.0, 0.0), skills({}), preferredResource(""), assignedToPreferredResource(false) {}

Krasnal::Krasnal(int id, const Point& home,
                 const std::vector<std::string>& skills,
                 const std::string& preferredResource,
                 bool assignedToPreferredResource)
    : id(id),
      home(home),
      skills(skills),
      preferredResource(preferredResource),
      assignedToPreferredResource(assignedToPreferredResource) {}

int Krasnal::getId() const{
  return id;
}

const Point& Krasnal::getHome() const{
  return home;
}

double Krasnal::getHomeY() const{
  return home.y;
}

double Krasnal::getHomeX() const{
    return home.x;
}

const std::vector<std::string>& Krasnal::getSkills() const{
  return skills;
}

const std::string& Krasnal::getPreferredResource() const{
  return preferredResource;
}

bool Krasnal::isAssignedToPreferredResource() const{
  return assignedToPreferredResource;
}

void Krasnal::setId(int newId){
  this->id = newId;
}

void Krasnal::setHome(const Point& newHome){
  this->home = newHome;
}

void Krasnal::setHomeX(double newX){
  this->home.x = newX;
}

void Krasnal::setHomeY(double newY){
  this->home.y = newY;
}

void Krasnal::setSkills(const std::vector<std::string>& newSkills){
  this->skills = newSkills;
}
void Krasnal::setPreferredResource(const std::string& newPreferredResource){
  this->preferredResource = newPreferredResource;
}
void Krasnal::setAssignedToPreferredResource(bool assignedToPreferredResource){
  this->assignedToPreferredResource = assignedToPreferredResource;
}
