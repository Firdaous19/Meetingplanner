#ifndef PROJECTTITLE_ROOM_H
#define PROJECTTITLE_ROOM_H

#include <string>
#include <vector>
#include "Renovation.h"

/**
 * Stelt een vergaderzaal voor in het systeem.
 * Een room heeft een naam, een unieke identifier, een capaciteit,
 * een campusidentifier, een buildingidentifier
 * en kan bezet worden tijdens het verwerken van meetings.
 */
class Room {
public:
    /**
     * Constructor van een room.
     * @param name De leesbare naam van de room.
     * @param identifier De unieke identifier van de room.
     * @param capacity Het maximum aantal personen dat in de room past.
     * @param campusIdentifier De identifier van de campus waartoe de room behoort.
     * @param buildingIdentifier De identifier van het gebouw waartoe de room behoort.
     *
     * REQUIRE(!name.empty(), "Room name mag niet leeg zijn");
     * REQUIRE(!identifier.empty(), "Room identifier mag niet leeg zijn");
     * REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
     * REQUIRE(!campusIdentifier.empty(), "Room campus identifier mag niet leeg zijn");
     * REQUIRE(!buildingIdentifier.empty(), "Room building identifier mag niet leeg zijn");
     * ENSURE(this->name == name, "Room name correct opgeslagen");
     * ENSURE(this->identifier == identifier, "Room identifier correct opgeslagen");
     * ENSURE(this->capacity == capacity, "Room capacity correct opgeslagen");
     * ENSURE(this->campusIdentifier == campusIdentifier,
     *        "Room campus identifier correct opgeslagen");
     * ENSURE(this->buildingIdentifier == buildingIdentifier,
     *        "Room building identifier correct opgeslagen");
     * ENSURE(!occupied, "Nieuwe room mag niet bezet zijn");
     */
    Room(const std::string& name,
         const std::string& identifier,
         int capacity,
         const std::string& campusIdentifier,
         const std::string& buildingIdentifier);

    /**
     * Geef de naam van de room terug.
     * @return De naam van de room.
     */
    std::string getName() const;

    /**
     * Geef de unieke identifier van de room terug.
     * @return De identifier van de room.
     */
    std::string getIdentifier() const;

    /**
     * Geef de capaciteit van de room terug.
     * @return Het maximum aantal personen dat in de room past.
     */
    int getCapacity() const;

    /**
     * Geef de campus identifier van de room terug.
     * @return De campus identifier.
     */
    std::string getCampusIdentifier() const;

    /**
     * Geef de building identifier van de room terug.
     * @return De building identifier.
     */
    std::string getBuildingIdentifier() const;

    /**
     * Voeg een persoon toe aan de room.
     * @param personName De naam van de persoon.
     *
     * REQUIRE(!personName.empty(), "Persoonsnaam mag niet leeg zijn");
     * REQUIRE(persons.size() < static_cast<size_t>(capacity), "Kamer is al vol!");
     * ENSURE(persons.size() == oldSize + 1, "Persoon moet toegevoegd zijn");
     */
    void addPerson(const std::string& personName);

    /**
     * Geef het aantal personen in de room terug.
     * @return Het aantal toegevoegde personen.
     */
    int getNumberOfPersons() const;

    /**
     * Controleer of de room bezet is.
     * @return true als de room bezet is, anders false.
     */
    bool isOccupied() const;

    /**
     * Zet de room op bezet.
     *
     * REQUIRE(!occupied, "Room mag niet al bezet zijn");
     * ENSURE(occupied, "Room moet bezet zijn na occupy()");
     */
    void occupy();
    void addRenovation(const Renovation& renovation);

    bool isBeingRenovated(const std::string& date) const;

private:
    std::string name;
    std::string identifier;
    int capacity;
    std::string campusIdentifier;
    std::string buildingIdentifier;
    std::vector<std::string> persons;
    std::vector<Renovation> renovations;
    bool occupied = false;
};

#endif