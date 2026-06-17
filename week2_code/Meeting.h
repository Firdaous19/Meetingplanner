#ifndef PROJECTTITLE_MEETING_H
#define PROJECTTITLE_MEETING_H

#include <string>
#include <vector>
#include <cstddef>

/**
 * Stelt een meeting voor in het systeem.
 * Een meeting heeft een label, identifier, gekoppelde room,
 * datum, een lijst van deelnemers, en houdt ook CO2-uitstoot
 * en cateringkost bij.
 *
 * Vanaf use case 3.7 wordt per deelnemer ook bijgehouden
 * of die intern of extern is.
 */
class Meeting {
public:
    float calculateBaseCO2() const;
    float calculateCateringCost() const;
    /**
     * Constructor van een meeting.
     * @param label De naam of titel van de meeting.
     * @param identifier De unieke identifier van de meeting.
     * @param roomIdentifier De identifier van de gekoppelde room.
     *        Mag leeg zijn voor online meetings.
     * @param date De datum van de meeting.
     *
     * REQUIRE(!label.empty(), "Meeting label mag niet leeg zijn");
     * REQUIRE(!identifier.empty(), "Meeting identifier mag niet leeg zijn");
     * REQUIRE(!date.empty(), "Meeting date mag niet leeg zijn");
     * REQUIRE(isValidDateFormat(date), "Meeting date moet formaat YYYY-MM-DD hebben");
     * ENSURE(this->label == label, "Meeting label correct opgeslagen");
     * ENSURE(this->identifier == identifier, "Meeting identifier correct opgeslagen");
     * ENSURE(this->roomIdentifier == roomIdentifier, "Meeting room identifier correct opgeslagen");
     * ENSURE(this->date == date, "Meeting date correct opgeslagen");
     * ENSURE(!online, "Nieuwe meeting is standaard niet online");
     * ENSURE(!catering, "Nieuwe meeting heeft standaard geen catering");
     * ENSURE(!(online && catering),
     *        "Nieuwe meeting mag niet tegelijk online en catering hebben");
     * ENSURE(this->co2Emission == 0.0f,
     *        "Nieuwe meeting heeft standaard 0 CO2-uitstoot");
     * ENSURE(this->occupancyPercentage == 0,
     *        "Nieuwe meeting heeft standaard 0% occupancy");
     * ENSURE(this->cateringCost == 0.0f,
     *        "Nieuwe meeting heeft standaard 0 cateringkost");
     * ENSURE(this->participants.empty(), "Nieuwe meeting heeft standaard geen deelnemers");
     * ENSURE(this->externalParticipants.empty(),
     *        "Nieuwe meeting heeft standaard geen external participant flags");
     */
    Meeting(const std::string& label,
            const std::string& identifier,
            const std::string& roomIdentifier,
            const std::string& date);

    /** @return Het label van de meeting. */
    std::string getLabel() const { return label; }

    /** @return De unieke identifier van de meeting. */
    std::string getIdentifier() const { return identifier; }

    /** @return De identifier van de room waarin de meeting plaatsvindt. */
    std::string getRoomIdentifier() const { return roomIdentifier; }

    /** @return De datum van de meeting in formaat YYYY-MM-DD. */
    std::string getDate() const { return date; }

    /**
     * Voeg een interne deelnemer toe aan deze meeting.
     * Deze functie blijft bestaan zodat oude code en oude testen blijven werken.
     * @param user Naam van de deelnemer.
     *
     * REQUIRE(!user.empty(), "Participant name mag niet leeg zijn");
     * ENSURE(participants.size() == oldSize + 1, "Participant moet toegevoegd zijn");
     * ENSURE(externalParticipants.size() == participants.size(),
     *        "Voor elke participant moet een external flag bestaan");
     * ENSURE(!externalParticipants.back(),
     *        "addParticipant zonder external parameter voegt interne participant toe");
     */
    void addParticipant(const std::string& user);

    /**
     * Voeg een deelnemer toe aan deze meeting.
     * @param user Naam van de deelnemer.
     * @param external true als de deelnemer extern is.
     *
     * REQUIRE(!user.empty(), "Participant name mag niet leeg zijn");
     * ENSURE(participants.size() == oldSize + 1, "Participant moet toegevoegd zijn");
     * ENSURE(externalParticipants.size() == participants.size(),
     *        "Voor elke participant moet een external flag bestaan");
     * ENSURE(externalParticipants.back() == external,
     *        "External participant flag moet correct opgeslagen zijn");
     */
    void addParticipant(const std::string& user, bool external);

    /** @return Alle deelnemers van deze meeting. */
    const std::vector<std::string>& getParticipants() const { return participants; }

    /**
     * Geef terug of een participant extern is.
     * @param index Index van de participant.
     * @return true als deze participant extern is.
     *
     * REQUIRE(index < externalParticipants.size(),
     *         "Participant index moet geldig zijn");
     */
    bool isParticipantExternal(std::size_t index) const;

    /** @return Aantal interne deelnemers. */
    int getInternalParticipantCount() const;

    /** @return Aantal externe deelnemers. */
    int getExternalParticipantCount() const;

    /** @return true als er minstens één interne deelnemer is. */
    bool hasInternalParticipants() const;

    /** @return true als er minstens één externe deelnemer is. */
    bool hasExternalParticipants() const;

    /** @return true als externen toegelaten zijn. */
    bool areExternalsAllowed() const { return externalsAllowed; }

    /** @return true als catering nodig is. */
    bool hasCatering() const { return catering; }

    /** @return true als de meeting online plaatsvindt. */
    bool isOnline() const { return online; }

    /** @return CO2-uitstoot van deze meeting in gram. */
    float getCO2Emission() const { return co2Emission; }

    /** @return Bezettingspercentage van de room voor deze meeting. */
    int getOccupancyPercentage() const { return occupancyPercentage; }

    /** @return Cateringkost van deze meeting in euro. */
    float getCateringCost() const { return cateringCost; }

    /**
     * Stel in of externen toegelaten zijn.
     * @param value true als externen toegelaten zijn.
     *
     * ENSURE(externalsAllowed == value, "Externals allowed flag correct opgeslagen");
     */
    void setExternalsAllowed(bool value);

    /**
     * Stel in of de meeting catering nodig heeft.
     * @param value true als catering nodig is.
     *
     * REQUIRE(!(online && value),
     *         "Online meeting mag geen catering hebben");
     * ENSURE(catering == value, "Catering flag correct opgeslagen");
     * ENSURE(!(online && catering),
     *        "Meeting mag niet tegelijk online en catering hebben");
     */
    void setCatering(bool value);

    /**
     * Stel in of de meeting online plaatsvindt.
     * @param value true als de meeting online is.
     *
     * REQUIRE(!(value && catering),
     *         "Online meeting mag geen catering hebben");
     * ENSURE(online == value, "Online flag correct opgeslagen");
     * ENSURE(!(online && catering),
     *        "Meeting mag niet tegelijk online en catering hebben");
     */
    void setOnline(bool value);

    /**
     * Stel de CO2-uitstoot van de meeting in.
     * @param value CO2-uitstoot in gram.
     *
     * REQUIRE(value >= 0, "CO2 emission mag niet negatief zijn");
     * ENSURE(co2Emission == value, "CO2 emission correct opgeslagen");
     */
    void setCO2Emission(float value);

    /**
     * Stel het bezettingspercentage van de room in voor deze meeting.
     * @param percentage Percentage tussen 0 en 100.
     *
     * REQUIRE(percentage >= 0 && percentage <= 100,
     *         "Occupancy percentage moet tussen 0 en 100 liggen");
     * ENSURE(occupancyPercentage == percentage,
     *        "Occupancy percentage correct opgeslagen");
     */
    void setOccupancyPercentage(int percentage);

    /**
     * Stel de cateringkost van de meeting in.
     * @param value Cateringkost in euro.
     *
     * REQUIRE(value >= 0, "Catering cost mag niet negatief zijn");
     * ENSURE(cateringCost == value, "Catering cost correct opgeslagen");
     */
    void setCateringCost(float value);

private:
    std::string label;
    std::string identifier;
    std::string roomIdentifier;
    std::string date;

    std::vector<std::string> participants;
    std::vector<bool> externalParticipants;

    bool externalsAllowed = false;
    bool catering = false;
    bool online = false;
    float co2Emission = 0.0f;
    int occupancyPercentage = 0;
    float cateringCost = 0.0f;
};

#endif