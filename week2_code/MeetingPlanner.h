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
#include "Logger.h"

/**
 * Centrale klasse die alle rooms, meetings, campussen,
 * gebouwen, renovaties en cateringproviders beheert.
 */
class MeetingPlanner {
public:
    /**
     * Voeg een room toe aan het systeem.
     * @param room De toe te voegen room.
     *
     * ENSURE(rooms.size() == oldSize + 1, "Room moet toegevoegd zijn");
     */
    void addRoom(const Room& room);

    /**
     * Voeg een meeting toe aan het systeem.
     * @param meeting De toe te voegen meeting.
     *
     * REQUIRE(!meeting.getIdentifier().empty(), "Meeting identifier mag niet leeg zijn");
     * REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
     * REQUIRE(!(meeting.isOnline() && meeting.hasCatering()),
     *         "Online meeting mag geen catering hebben");
     * REQUIRE(meeting.isOnline() || !meeting.getRoomIdentifier().empty(),
     *         "Fysieke meeting moet een room identifier hebben");
     * ENSURE(meetings.size() == oldSize + 1, "Meeting moet toegevoegd zijn");
     */
    void addMeeting(const Meeting& meeting);

    /**
     * Voeg een participant toe aan een bestaande meeting.
     * @param meetingId Identifier van de meeting.
     * @param user Naam van de gebruiker.
     * @return true indien succesvol toegevoegd.
     *
     * REQUIRE(!meetingId.empty(), "Meeting ID mag niet leeg zijn");
     * REQUIRE(!user.empty(), "User mag niet leeg zijn");
     * ENSURE(meeting.getParticipants().size() == oldSize + 1,
     *        "Participant moet toegevoegd zijn aan meeting");
     */
    bool addParticipation(const std::string& meetingId, const std::string& user);
    bool addParticipation(const std::string& meetingId,
                          const std::string& user,
                          bool external);

    /**
     * Controleer de algemene consistentie van het systeem.
     * @return true indien systeem consistent is.
     *
     * ENSURE(conflicts.empty(), "Bij consistent systeem mogen er geen conflicts zijn");
     */
    bool checkConsistency();

    /**
     * Verwerk automatisch alle meetings.
     *
     * ENSURE(successfulMeetings.size() <= meetings.size(),
     *        "Aantal succesvolle meetings mag niet groter zijn dan totaal aantal meetings");
     */
    void processMeetings();

    /**
     * Voeg een campus toe.
     * @param campus De toe te voegen campus.
     *
     * ENSURE(campuses.size() == oldSize + 1, "Campus moet toegevoegd zijn");
     */
    void addCampus(const Campus& campus);

    /**
     * Voeg een building toe.
     * @param building Het toe te voegen gebouw.
     *
     * ENSURE(buildings.size() == oldSize + 1, "Building moet toegevoegd zijn");
     */
    void addBuilding(const Building& building);

    /**
     * Voeg een renovatieperiode toe.
     * @param renovation De renovatie.
     *
     * REQUIRE(!renovation.getRoomIdentifier().empty(), "Renovation room identifier mag niet leeg zijn");
     * REQUIRE(!renovation.getStartDate().empty(), "Renovation start date mag niet leeg zijn");
     * REQUIRE(!renovation.getEndDate().empty(), "Renovation end date mag niet leeg zijn");
     * REQUIRE(roomExists(renovation.getRoomIdentifier()),
     *         "Renovation moet verwijzen naar een bestaande room");
     * REQUIRE(renovation.getStartDate() <= renovation.getEndDate(),
     *         "Renovation start date moet voor of gelijk aan end date liggen");
     * ENSURE(renovations.size() == oldSize + 1, "Renovation moet toegevoegd zijn");
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
     *
     * ENSURE(cateringProviders.size() == oldSize + 1, "Catering provider moet toegevoegd zijn");
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

    /**
     * Geef de totale cateringkost van alle succesvol verwerkte meetings terug.
     * @return Totale cateringkost in euro.
     */
    float getTotalCateringCost() const;

private:
    bool validatePhysicalMeeting(Meeting& meeting, Room*& room);
    void processSuccessfulPhysicalMeeting(Meeting& meeting, Room& room);
    Logger logger;
    bool loggingEnabled = true;

    /**
     * Verwerk één individuele meeting.
     * @param meeting De meeting.
     * @return true indien succesvol verwerkt.
     *
     * REQUIRE(!meeting.getIdentifier().empty(), "Meeting identifier mag niet leeg zijn");
     * REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
     * REQUIRE(!(meeting.isOnline() && meeting.hasCatering()),
     *         "Online meeting mag geen catering hebben");
     * REQUIRE(meeting.isOnline() || !meeting.getRoomIdentifier().empty(),
     *         "Fysieke meeting moet een room identifier hebben");
     * ENSURE(meeting.getCateringCost() >= 0.0f,
     *        "Meeting moet niet-negatieve cateringkost hebben");
     * ENSURE(meeting.getCO2Emission() >= 0,
     *        "Meeting moet niet-negatieve CO2 hebben");
     */
    bool processSingleMeeting(Meeting& meeting);

    /**
     * Controleer of een room bestaat.
     * @param roomIdentifier Identifier van de room.
     * @return true indien room bestaat.
     *
     * REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");
     */
    bool roomExists(const std::string& roomIdentifier) const;

    /**
     * Zoek een room op basis van identifier.
     * @param roomIdentifier Identifier van de room.
     * @return Pointer naar de room of nullptr indien niet gevonden.
     *
     * REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");
     */
    Room* findRoomByIdentifier(const std::string& roomIdentifier);

    /**
     * Zoek een room op basis van identifier.
     * @param roomIdentifier Identifier van de room.
     * @return Pointer naar de room of nullptr indien niet gevonden.
     *
     * REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");
     */
    const Room* findRoomByIdentifier(const std::string& roomIdentifier) const;

    /**
     * Zoek een cateringprovider voor een campus.
     * @param campusIdentifier Identifier van de campus.
     * @return Pointer naar de provider of nullptr indien niet gevonden.
     *
     * REQUIRE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");
     */
    const CateringProvider* findCateringProviderByCampus(
            const std::string& campusIdentifier) const;

    /**
     * Bereken de CO2-uitstoot van een meeting.
     * @param meeting De meeting.
     * @return CO2-uitstoot in gram.
     *
     * REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
     */
    float calculateMeetingCO2(const Meeting& meeting) const;

    /**
     * Bereken de catering-CO2 van een meeting.
     * @param meeting De meeting.
     * @param room De room waarin de meeting plaatsvindt.
     * @return Catering-CO2 in gram.
     *
     * REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");
     * REQUIRE(meeting.hasCatering(), "Catering CO2 berekenen vereist catering");
     * REQUIRE(!room.getCampusIdentifier().empty(), "Room campus identifier mag niet leeg zijn");
     * REQUIRE(provider != nullptr,
     *         "Catering provider moet bestaan voor campus van room");
     */
    float calculateCateringCO2(const Meeting& meeting, const Room& room) const;

    /**
     * Bereken de cateringkost van een meeting.
     * @param meeting De meeting.
     * @return Cateringkost in euro.
     *
     * REQUIRE(meeting.hasCatering(), "Catering cost berekenen vereist catering");
     * REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");
     */
    float calculateCateringCost(const Meeting& meeting) const;

    /**
     * Schrijf cateringinformatie van een meeting weg naar een bestand.
     * @param meeting De meeting.
     * @param room De room waarin de meeting plaatsvindt.
     *
     * REQUIRE(meeting.hasCatering(), "Alleen meetings met catering mogen weggeschreven worden");
     * REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");
     * REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
     * REQUIRE(!room.getIdentifier().empty(), "Room identifier mag niet leeg zijn");
     * REQUIRE(output.is_open(), "Catering deliveries file kon niet geopend worden");
     * ENSURE(output.good(), "Catering delivery info moet correct weggeschreven zijn");
     */
    void appendCateringDeliveryToFile(const Meeting& meeting, const Room& room) const;

    std::vector<Room> rooms;
    std::vector<Meeting> meetings;
    std::vector<Meeting> successfulMeetings;
    std::vector<std::string> conflicts;
    std::vector<Campus> campuses;
    std::vector<Building> buildings;
    std::vector<Renovation> renovations;
    std::vector<CateringProvider> cateringProviders;
    float totalCO2Emission = 0.0f;
    float totalCateringCost = 0.0f;
};

#endif // PROJECTTITLE_MEETINGPLANNER_H

