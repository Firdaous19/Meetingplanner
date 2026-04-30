//
// Created by firdi on 21/04/2026.
//

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

private:
    bool loggingEnabled = true;

    /**
     * Verwerk één individuele meeting.
     * @param meeting De meeting.
     * @return true indien succesvol verwerkt.
     */
    bool processSingleMeeting(const Meeting& meeting);

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

    std::vector<Room> rooms;
    std::vector<Meeting> meetings;
    std::vector<Meeting> successfulMeetings;
    std::vector<std::string> conflicts;
    std::vector<Campus> campuses;
    std::vector<Building> buildings;
    std::vector<Renovation> renovations;
    std::vector<CateringProvider> cateringProviders;
};

#endif // PROJECTTITLE_MEETINGPLANNER_H