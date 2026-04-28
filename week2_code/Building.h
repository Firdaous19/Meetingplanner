//
// Created by ali_s on 26/04/2026.
//

#ifndef PROJECTTITLE_BUILDINGS_H
#define PROJECTTITLE_BUILDINGS_H
#include <string>

/**
 * Stelt een gebouw voor binnen een campus.
 */
class Building {
public:
    Building(const std::string& name,
             const std::string& identifier,
             const std::string& campusIdentifier);

    std::string getName() const { return name; }
    std::string getIdentifier() const { return identifier; }
    std::string getCampusIdentifier() const { return campusIdentifier; }

private:
    std::string name;
    std::string identifier;
    std::string campusIdentifier;
};


#endif //PROJECTTITLE_BUILDINGS_H