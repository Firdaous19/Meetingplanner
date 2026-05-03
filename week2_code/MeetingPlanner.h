
#ifndef PROJECTTITLE_MEETINGPLANNER_H
#define PROJECTTITLE_MEETINGPLANNER_H

#include <vector>
#include <string>
#include "Room.h"
#include "Meeting.h"
#include "Campus.h"
#include "Building.h"
#include "Renovation.h"
#include "CateringProvider.h"

/**
 * Centrale klasse die alle rooms, meetings, campussen,
 * gebouwen, renovaties en cateringproviders beheert.
 */
class MeetingPlanner {
public:
    /**
     * Voeg een room toe aan het systeem.
     * @param room De toe te voegen room.
     */
    void addRoom(const Room& room);

    /**
     * Voeg een meeting toe aan het systeem.
     * @param meeting De toe te voegen meeting.
     */
    void addMeeting(const Meeting& meeting);

    /**
     * Voeg een participant toe aan een bestaande meeting.
     * @param meetingId Identifier van de meeting.
     * @param user Naam van de gebruiker.
     * @return true indien succesvol toegevoegd.
     */
    bool addParticipation(const std::string& meetingId, const std::string& user);

    /**
     * Controleer de algemene consistentie van het systeem.
     * @return true indien systeem consistent is.
     */
    bool checkConsistency();

    /**
     * Verwerk automatisch alle meetings.
     */
    void processMeetings();

    /**
     * Voeg een campus toe.
     * @param campus De toe te voegen campus.
     */
    void addCampus(const Campus& campus);

    /**
     * Voeg een building toe.
     * @param building Het toe te voegen gebouw.
     */
    void addBuilding(const Building& building);

    /**
     * Voeg een renovatieperiode toe.
     * @param renovation De renovatie.
     */
    void addRenovation(const Renovation& renovation);

    /**
     * Activeer of deactiveer console logging.
     * @param enabled true om logging aan te zetten.
     */
    void setLoggingEnabled(bool enabled);

    /**
     * Voeg een cateringprovider toe.
     * @param provider De provider.
     */
    void addCateringProvider(const CateringProvider& provider);

    /** @return Alle cateringproviders. */
    const std::vector<CateringProvider>& getCateringProviders() const;

    /** @return Alle renovaties. */
    const std::vector<Renovation>& getRenovations() const;

    /** @return Alle campussen. */
    const std::vector<Campus>& getCampuses() const;

    /** @return Alle buildings. */
    const std::vector<Building>& getBuildings() const;

    /** @return Alle rooms. */
    const std::vector<Room>& getRooms() const;

    /** @return Alle meetings. */
    const std::vector<Meeting>& getMeetings() const;

    /** @return Alle succesvol verwerkte meetings. */
    const std::vector<Meeting>& getSuccessfulMeetings() const;

    /** @return Alle conflicten. */
    const std::vector<std::string>& getConflicts() const;

    /**
     * Geef de totale CO2-uitstoot van alle succesvol verwerkte meetings terug.
     * @return Totale CO2-uitstoot in gram.
     */
    float getTotalCO2Emission() const;

private:
    bool loggingEnabled = true;

    /**
     * Verwerk één individuele meeting.
     *
     * PRE:
     * - meeting identifier mag niet leeg zijn
     * - meeting date mag niet leeg zijn
     * - online meeting mag geen catering hebben
     * - fysieke meeting moet een room identifier hebben
     * - als catering gevraagd wordt, moet er een provider bestaan
     *   voor de campus van de room
     *
     * POST bij succes:
     * - de meeting heeft een berekende CO2-uitstoot
     * - de totale CO2-uitstoot van het systeem is verhoogd
     *
     * @param meeting De meeting.
     * @return true indien succesvol verwerkt.
     */
    bool processSingleMeeting(Meeting& meeting);

    /**
     * Controleer of een room in renovatie is op een bepaalde datum.
     * @param roomIdentifier Identifier van de room.
     * @param date Datum.
     * @return true indien room in renovatie is.
     */
    bool isRoomUnderRenovation(const std::string& roomIdentifier,
                               const std::string& date) const;

    /**
     * Controleer of een room bestaat.
     * @param roomIdentifier Identifier van de room.
     * @return true indien room bestaat.
     */
    bool roomExists(const std::string& roomIdentifier) const;

    /**
     * Zoek een room op basis van identifier.
     * @param roomIdentifier Identifier van de room.
     * @return Pointer naar de room of nullptr indien niet gevonden.
     */
    Room* findRoomByIdentifier(const std::string& roomIdentifier);

    /**
     * Zoek een room op basis van identifier.
     * @param roomIdentifier Identifier van de room.
     * @return Pointer naar de room of nullptr indien niet gevonden.
     */
    const Room* findRoomByIdentifier(const std::string& roomIdentifier) const;

    /**
     * Zoek een cateringprovider voor een campus.
     * @param campusIdentifier Identifier van de campus.
     * @return Pointer naar de provider of nullptr indien niet gevonden.
     */
    const CateringProvider* findCateringProviderByCampus(
            const std::string& campusIdentifier) const;

    /**
     * Bereken de CO2-uitstoot van een meeting zelf
     * (zonder catering-CO2).
     * @param meeting De meeting.
     * @return CO2-uitstoot in gram.
     */
    float calculateMeetingCO2(const Meeting& meeting) const;

    /**
     * Bereken de catering-CO2 van een meeting.
     * @param meeting De meeting.
     * @param room De room waarin de meeting plaatsvindt.
     * @return Catering-CO2 in gram.
     */
    float calculateCateringCO2(const Meeting& meeting, const Room& room) const;

    std::vector<Room> rooms;
    std::vector<Meeting> meetings;
    std::vector<Meeting> successfulMeetings;
    std::vector<std::string> conflicts;
    std::vector<Campus> campuses;
    std::vector<Building> buildings;
    std::vector<Renovation> renovations;
    std::vector<CateringProvider> cateringProviders;
    float totalCO2Emission = 0.0f;
};

#endif // PROJECTTITLE_MEETINGPLANNER_H