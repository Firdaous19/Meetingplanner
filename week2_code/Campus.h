//
// Created by firdi on 27/04/2026.
//

#ifndef PROJECTTITLE_CAMPUS_H
#define PROJECTTITLE_CAMPUS_H
#include <string>

/**
 * Stelt een campus voor binnen het systeem.
 */
class Campus {
public:
    Campus(const std::string& name,
           const std::string& identifier);

    std::string getName() const { return name; }
    std::string getIdentifier() const { return identifier; }

private:
    std::string name;
    std::string identifier;
};
#endif //PROJECTTITLE_CAMPUS_H
