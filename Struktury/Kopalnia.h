#ifndef KOPALNIA_H
#define KOPALNIA_H

#include "Point.h"
#include <string>
#include <vector>
#include <algorithm>

class Kopalnia {
private:
    int id;
    Point location;
    std::string resourceType;
    int capacity;
    std::vector<int> assignedDwarves;

public:
    Kopalnia();

    Kopalnia(int id, const Point& location, const std::string& resourceType,
             int capacity, const std::vector<int>& assignedDwarves);

    int getId() const;
    const Point& getLocation() const;
    double getX() const;
    double getY() const;
    const std::string& getResourceType() const;
    int getCapacity() const;
    const std::vector<int>& getAssignedDwarves() const;

    void setId(int newId);
    void setLocation(const Point& newLocation);
    void setX(double newX);
    void setY(double newY);
    void setResourceType(const std::string& newResourceType);
    void setCapacity(int newCapacity);
    void setAssignedDwarves(const std::vector<int>& newAssignedDwarves);

    int getAvailableCapacity() const;
    bool hasAvailableSpace() const;
    bool isInUse() const;
    void addDwarf(int dwarfId);
    void removeDwarf(int dwarfId);
};

#endif
