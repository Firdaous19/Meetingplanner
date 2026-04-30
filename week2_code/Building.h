//
// Created by ali_s on 26/04/2026.
//

#ifndef PROJECTTITLE_BUILDINGS_H
#define PROJECTTITLE_BUILDINGS_H

#include <string>

/**
 * Stelt een gebouw voor binnen een campus.
 * Een building heeft een naam, unieke identifier
 * en verwijst naar een specifieke campus.
 */
class Building {
public:
    /**
     * Constructor van een building.
     * @param name De naam van het gebouw.
     * @param identifier De unieke identifier van het gebouw.
     * @param campusIdentifier De identifier van de gekoppelde campus.
     */
    Building(const std::string& name,
             const std::string& identifier,
             const std::string& campusIdentifier);

    /**
     * Geef de naam van het gebouw terug.
     * @return Naam van het gebouw.
     */
    std::string getName() const { return name; }

    /**
     * Geef de unieke identifier van het gebouw terug.
     * @return Identifier van het gebouw.
     */
    std::string getIdentifier() const { return identifier; }

    /**
     * Geef de gekoppelde campus identifier terug.
     * @return Identifier van de campus.
     */
    std::string getCampusIdentifier() const { return campusIdentifier; }

private:
    std::string name;
    std::string identifier;
    std::string campusIdentifier;
};

#endif // PROJECTTITLE_BUILDINGS_H
