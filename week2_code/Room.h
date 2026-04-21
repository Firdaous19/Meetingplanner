#ifndef PROJECTTITLE_ROOM_H
#define PROJECTTITLE_ROOM_H

#include <string>
#include <vector>

/**
 * Stelt een vergaderzaal voor in het systeem.
 * Een room heeft een naam, een identifier, een capaciteit
 * en kan bezet worden tijdens het verwerken van meetings.
 */
class Room {
public:
    /**
     * Constructor van een room.
     * @param name De leesbare naam van de room.
     * @param identifier De unieke identifier van de room.
     * @param capacity Het maximum aantal personen in de room.
     */
    Room(const std::string& name, const std::string& identifier, int capacity);

    /**
     * Geef de naam van de room terug.
     */
    std::string getName() const { return name; }

    /**
     * Geef de identifier van de room terug.
     */
    std::string getIdentifier() const { return identifier; }

    /**
     * Geef de capaciteit van de room terug.
     */
    int getCapacity() const { return capacity; }

    /**
     * Voeg een persoon toe aan de room.
     * @param personName De naam van de persoon.
     */
    void addPerson(const std::string& personName);

    /**
     * Geef het aantal personen in de room terug.
     */
    int getNumberOfPersons() const { return persons.size(); }

    /**
     * Controleer of de room bezet is.
     */
    bool isOccupied() const { return occupied; }

    /**
     * Zet de room op bezet.
     */
    void occupy();

private:
    std::string name;
    std::string identifier;
    int capacity;
    std::vector<std::string> persons;
    bool occupied = false;
};

#endif