#ifndef KRASNAL_H
#define KRASNAL_H

#include "Point.h"
#include <string>
#include <vector>

class Krasnal {
private:
    int id;
    Point home;
    std::vector<std::string> skills;
    std::string preferredResource;
    bool assignedToPreferredResource;

public:
    Krasnal();

    Krasnal(int id, const Point& home,
            const std::vector<std::string>& skills,
            const std::string& preferredResource, bool assignedToPreferredResource);

    int getId() const;
    const Point& getHome() const;
    double getHomeX() const;
    double getHomeY() const;
    const std::vector<std::string>& getSkills() const;
    const std::string& getPreferredResource() const;
    bool isAssignedToPreferredResource() const;

    void setId(int newId);
    void setHome(const Point& newHome);
    void setHomeX(double newX);
    void setHomeY(double newY);
    void setSkills(const std::vector<std::string>& newSkills);
    void setPreferredResource(const std::string& newPreferredResource);
    void setAssignedToPreferredResource(bool assignedToPreferredResource);
};

#endif
