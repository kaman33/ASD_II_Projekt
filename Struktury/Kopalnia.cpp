#include "Kopalnia.h"

Kopalnia::Kopalnia()
    : id(0), location(0.0, 0.0), resourceType(""), capacity(0), assignedDwarves({}) {}

Kopalnia::Kopalnia(int id, const Point& location, const std::string& resourceType,
                   int capacity, const std::vector<int>& assignedDwarves)
    : id(id),
      location(location),
      resourceType(resourceType),
      capacity(capacity),
      assignedDwarves(assignedDwarves) {}

int Kopalnia::getId() const {
    return id;
}

const Point& Kopalnia::getLocation() const {
    return location;
}

double Kopalnia::getX() const {
    return location.x;
}

double Kopalnia::getY() const {
    return location.y;
}

const std::string& Kopalnia::getResourceType() const {
    return resourceType;
}

int Kopalnia::getCapacity() const {
    return capacity;
}

const std::vector<int>& Kopalnia::getAssignedDwarves() const {
    return assignedDwarves;
}

void Kopalnia::setId(int newId) {
    this->id = newId;
}

void Kopalnia::setLocation(const Point& newLocation) {
    this->location = newLocation;
}

void Kopalnia::setX(double newX) {
    this->location.x = newX;
}

void Kopalnia::setY(double newY) {
    this->location.y = newY;
}

void Kopalnia::setResourceType(const std::string& newResourceType) {
    this->resourceType = newResourceType;
}

void Kopalnia::setCapacity(int newCapacity) {
    this->capacity = newCapacity;
}

void Kopalnia::setAssignedDwarves(const std::vector<int>& newAssignedDwarves) {
    this->assignedDwarves = newAssignedDwarves;
}

int Kopalnia::getAvailableCapacity() const {
    return capacity - static_cast<int>(assignedDwarves.size());
}

bool Kopalnia::hasAvailableSpace() const {
    return getAvailableCapacity() > 0;
}

bool Kopalnia::isInUse() const {
    return !assignedDwarves.empty();
}

void Kopalnia::addDwarf(int dwarfId) {
    if (!hasAvailableSpace()) {
        return;
    }

    if (std::find(assignedDwarves.begin(), assignedDwarves.end(), dwarfId) != assignedDwarves.end()) {
        return;
    }

    assignedDwarves.push_back(dwarfId);
}

void Kopalnia::removeDwarf(int dwarfId) {
    auto it = std::find(assignedDwarves.begin(), assignedDwarves.end(), dwarfId);

    if (it != assignedDwarves.end()) {
        assignedDwarves.erase(it);
    }
}
