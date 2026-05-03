#ifndef PROJECTTITLE_MEETING_H
#define PROJECTTITLE_MEETING_H

#include <string>
#include <vector>
#include "DesignByContract.h"

/**
 * Stelt een meeting voor in het systeem.
 * Een meeting heeft een label, identifier, gekoppelde room,
 * datum, een lijst van deelnemers, en houdt ook CO2-uitstoot bij.
 */
class Meeting {
public:
    /**
     * Constructor van een meeting.
     *
     * PRE:
     * - label mag niet leeg zijn
     * - identifier mag niet leeg zijn
     * - date mag niet leeg zijn
     * - date moet formaat YYYY-MM-DD hebben
     * - roomIdentifier mag leeg zijn voor online meetings
     *
     * POST:
     * - alle velden zijn correct opgeslagen
     * - nieuwe meeting heeft standaard geen catering
     * - nieuwe meeting is standaard niet online
     * - nieuwe meeting heeft standaard 0 CO2-uitstoot
     * - nieuwe meeting heeft standaard 0% occupancy
     *
     * @param label De naam of titel van de meeting.
     * @param identifier De unieke identifier van de meeting.
     * @param roomIdentifier De identifier van de gekoppelde room.
     *        Mag leeg zijn voor online meetings.
     * @param date De datum van de meeting.
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
     * Voeg een deelnemer toe aan deze meeting.
     * @param user Naam van de deelnemer.
     */
    void addParticipant(const std::string& user);

    /** @return Alle deelnemers van deze meeting. */
    const std::vector<std::string>& getParticipants() const { return participants; }

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

    /**
     * Stel in of externen toegelaten zijn.
     * @param value true als externen toegelaten zijn.
     */
    void setExternalsAllowed(bool value) {
        externalsAllowed = value;
        ENSURE(externalsAllowed == value, "Externals allowed flag correct opgeslagen");
    }

    /**
     * Stel in of de meeting catering nodig heeft.
     *
     * PRE:
     * - een online meeting mag geen catering hebben
     *
     * POST:
     * - catering flag is correct opgeslagen
     * - meeting is niet tegelijk online én met catering
     *
     * @param value true als catering nodig is.
     */
    void setCatering(bool value) {
        REQUIRE(!(online && value),
                "Online meeting mag geen catering hebben");

        catering = value;

        ENSURE(catering == value, "Catering flag correct opgeslagen");
        ENSURE(!(online && catering),
               "Meeting mag niet tegelijk online en catering hebben");
    }

    /**
     * Stel in of de meeting online plaatsvindt.
     *
     * PRE:
     * - een online meeting mag geen catering hebben
     *
     * POST:
     * - online flag is correct opgeslagen
     * - meeting is niet tegelijk online én met catering
     *
     * @param value true als de meeting online is.
     */
    void setOnline(bool value) {
        REQUIRE(!(value && catering),
                "Online meeting mag geen catering hebben");

        online = value;

        ENSURE(online == value, "Online flag correct opgeslagen");
        ENSURE(!(online && catering),
               "Meeting mag niet tegelijk online en catering hebben");
    }

    /**
     * Stel de CO2-uitstoot van de meeting in.
     *
     * PRE:
     * - value mag niet negatief zijn
     *
     * POST:
     * - co2Emission is correct opgeslagen
     *
     * @param value CO2-uitstoot in gram.
     */
    void setCO2Emission(float value) {
        REQUIRE(value >= 0, "CO2 emission mag niet negatief zijn");

        co2Emission = value;

        ENSURE(co2Emission == value, "CO2 emission correct opgeslagen");
    }

    /**
     * Stel het bezettingspercentage van de room in voor deze meeting.
     *
     * PRE:
     * - percentage moet tussen 0 en 100 liggen
     *
     * POST:
     * - occupancyPercentage is correct opgeslagen
     *
     * @param percentage Percentage tussen 0 en 100.
     */
    void setOccupancyPercentage(int percentage) {
        REQUIRE(percentage >= 0 && percentage <= 100,
                "Occupancy percentage moet tussen 0 en 100 liggen");

        occupancyPercentage = percentage;

        ENSURE(occupancyPercentage == percentage,
               "Occupancy percentage correct opgeslagen");
    }

private:
    std::string label;
    std::string identifier;
    std::string roomIdentifier;
    std::string date;
    std::vector<std::string> participants;

    bool externalsAllowed = false;
    bool catering = false;
    bool online = false;
    float co2Emission = 0.0f;
    int occupancyPercentage = 0;
};

#endif