//
// Created by firdi on 27/04/2026.
//

#ifndef PROJECTTITLE_CAMPUS_H
#define PROJECTTITLE_CAMPUS_H

#include <string>

/**
 * Stelt een campus voor binnen het systeem.
 * Een campus heeft een naam en een unieke identifier.
 */
class Campus {
public:
    /**
     * Constructor van een campus.
     * @param name De naam van de campus.
     * @param identifier De unieke identifier van de campus.
     *
     * REQUIRE(!name.empty(), "Campus name mag niet leeg zijn");
     * REQUIRE(!identifier.empty(), "Campus identifier mag niet leeg zijn");
     * ENSURE(this->name == name, "Campus name correct opgeslagen");
     * ENSURE(this->identifier == identifier, "Campus identifier correct opgeslagen");
     */
    Campus(const std::string& name,
           const std::string& identifier);

    /**
     * Geef de naam van de campus terug.
     * @return Naam van de campus.
     */
    std::string getName() const { return name; }

    /**
     * Geef de unieke identifier van de campus terug.
     * @return Identifier van de campus.
     */
    std::string getIdentifier() const { return identifier; }

private:
    std::string name;
    std::string identifier;
};

#endif // PROJECTTITLE_CAMPUS_H
