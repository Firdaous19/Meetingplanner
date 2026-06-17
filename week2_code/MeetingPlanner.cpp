#include "MeetingPlanner.h"
#include "DesignByContract.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace {
    const float kInternalCateringCostPerPerson = 10.59f;
}

void MeetingPlanner::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
    logger.setEnabled(enabled);
}

void MeetingPlanner::addRoom(const Room& room) {
    size_t oldSize = rooms.size();
    rooms.push_back(room);
    ENSURE(rooms.size() == oldSize + 1, "Room moet toegevoegd zijn");
}

void MeetingPlanner::addMeeting(const Meeting& meeting) {
    REQUIRE(!meeting.getIdentifier().empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
    REQUIRE(!(meeting.isOnline() && meeting.hasCatering()),
            "Online meeting mag geen catering hebben");
    REQUIRE(meeting.isOnline() || !meeting.getRoomIdentifier().empty(),
            "Fysieke meeting moet een room identifier hebben");

    size_t oldSize = meetings.size();
    meetings.push_back(meeting);

    ENSURE(meetings.size() == oldSize + 1, "Meeting moet toegevoegd zijn");
}
bool MeetingPlanner::addParticipation(const std::string& meetingId,
                                      const std::string& user) {
    return addParticipation(meetingId, user, false);
}
bool MeetingPlanner::addParticipation(const std::string& meetingId,
                                      const std::string& user,
                                      bool external) {
    REQUIRE(!meetingId.empty(), "Meeting ID mag niet leeg zijn");
    REQUIRE(!user.empty(), "User mag niet leeg zijn");

    for (auto& meeting : meetings) {
        if (meeting.getIdentifier() == meetingId) {
            if (external && !meeting.areExternalsAllowed()) {
                return false;
            }

            size_t oldSize = meeting.getParticipants().size();
            meeting.addParticipant(user, external);

            ENSURE(meeting.getParticipants().size() == oldSize + 1,
                   "Participant moet toegevoegd zijn aan meeting");

            return true;
        }
    }

    return false;
}

bool MeetingPlanner::roomExists(const std::string& roomIdentifier) const {
    REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");

    for (const auto& room : rooms) {
        if (room.getIdentifier() == roomIdentifier) {
            return true;
        }
    }
    return false;
}

Room* MeetingPlanner::findRoomByIdentifier(const std::string& roomIdentifier) {
    REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");

    for (auto& room : rooms) {
        if (room.getIdentifier() == roomIdentifier) {
            return &room;
        }
    }
    return nullptr;
}

const Room* MeetingPlanner::findRoomByIdentifier(const std::string& roomIdentifier) const {
    REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");

    for (const auto& room : rooms) {
        if (room.getIdentifier() == roomIdentifier) {
            return &room;
        }
    }
    return nullptr;
}

const CateringProvider* MeetingPlanner::findCateringProviderByCampus(
        const std::string& campusIdentifier) const {
    REQUIRE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");

    for (const auto& provider : cateringProviders) {
        if (provider.getCampusIdentifier() == campusIdentifier) {
            return &provider;
        }
    }
    return nullptr;
}

bool MeetingPlanner::isRoomUnderRenovation(const std::string& roomIdentifier,
                                           const std::string& date) const {
    REQUIRE(!roomIdentifier.empty(), "Room identifier mag niet leeg zijn");
    REQUIRE(!date.empty(), "Date mag niet leeg zijn");

    for (const auto& renovation : renovations) {
        if (renovation.getRoomIdentifier() == roomIdentifier &&
            renovation.isActiveOnDate(date)) {
            return true;
        }
    }

    return false;
}

float MeetingPlanner::calculateMeetingCO2(const Meeting& meeting) const {
    REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");

    const float participantCount = static_cast<float>(meeting.getParticipants().size());

    if (meeting.isOnline()) {
        return participantCount * 30.0f;
    }

    return participantCount * 120.0f;
}

float MeetingPlanner::calculateCateringCO2(const Meeting& meeting, const Room& room) const {
    REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");
    REQUIRE(meeting.hasCatering(), "Catering CO2 berekenen vereist catering");
    REQUIRE(!room.getCampusIdentifier().empty(), "Room campus identifier mag niet leeg zijn");

    const CateringProvider* provider =
            findCateringProviderByCampus(room.getCampusIdentifier());

    REQUIRE(provider != nullptr,
            "Catering provider moet bestaan voor campus van room");

    const float participantCount = static_cast<float>(meeting.getParticipants().size());
    return provider->getCO2() * participantCount;
}

float MeetingPlanner::calculateCateringCost(const Meeting& meeting) const {
    REQUIRE(meeting.hasCatering(), "Catering cost berekenen vereist catering");
    REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");

    const float participantCount = static_cast<float>(meeting.getParticipants().size());

    return participantCount * kInternalCateringCostPerPerson;
}

void MeetingPlanner::appendCateringDeliveryToFile(const Meeting& meeting,
                                                  const Room& room) const {
    REQUIRE(meeting.hasCatering(), "Alleen meetings met catering mogen weggeschreven worden");
    REQUIRE(!meeting.isOnline(), "Online meeting mag geen catering hebben");
    REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
    REQUIRE(!room.getIdentifier().empty(), "Room identifier mag niet leeg zijn");

    std::ofstream output("catering_deliveries.txt", std::ios::app);
    REQUIRE(output.is_open(), "Catering deliveries file kon niet geopend worden");

    output << "Meeting: " << meeting.getIdentifier() << '\n';
    output << "Date: " << meeting.getDate() << '\n';
    output << "Location: " << room.getIdentifier() << '\n';
    output << "Participants: " << meeting.getParticipants().size() << '\n';
    output << std::fixed << std::setprecision(2);
    output << "Catering cost: EUR " << meeting.getCateringCost() << '\n';
    output << "----------------------------------------" << '\n';

    ENSURE(output.good(), "Catering delivery info moet correct weggeschreven zijn");

    output.close();
}

bool MeetingPlanner::checkConsistency() {
    bool consistent = true;
    conflicts.clear();

    for (size_t i = 0; i < rooms.size(); i++) {
        for (size_t j = i + 1; j < rooms.size(); j++) {
            if (rooms[i].getIdentifier() == rooms[j].getIdentifier()) {
                std::string msg = "Dubbele room identifier: " + rooms[i].getIdentifier();

                logger.error(msg);
                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (size_t i = 0; i < meetings.size(); i++) {
        for (size_t j = i + 1; j < meetings.size(); j++) {
            if (meetings[i].getIdentifier() == meetings[j].getIdentifier()) {
                std::string msg = "Dubbele meeting identifier: " + meetings[i].getIdentifier();

                logger.error("Fout: " + msg);

                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (size_t i = 0; i < cateringProviders.size(); i++) {
        for (size_t j = i + 1; j < cateringProviders.size(); j++) {
            if (cateringProviders[i].getCampusIdentifier() ==
                cateringProviders[j].getCampusIdentifier()) {
                std::string msg = "Meerdere catering providers voor campus: " +
                                  cateringProviders[i].getCampusIdentifier();

                logger.error("Fout: " + msg);
                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (const auto& meeting : meetings) {
        if (meeting.isOnline() && meeting.hasCatering()) {
            std::string msg = "Meeting " + meeting.getIdentifier() +
                              " is ongeldig: online meetings mogen geen catering hebben";

            logger.error("Fout: " + msg);

            conflicts.push_back(msg);
            consistent = false;
            continue;
        }

        if (meeting.isOnline()) {
            continue;
        }

        if (meeting.getRoomIdentifier().empty()) {
            std::string msg = "Meeting " + meeting.getIdentifier() +
                              " heeft geen room identifier";

            logger.error("Fout: " + msg);

            conflicts.push_back(msg);
            consistent = false;
            continue;
        }

        const Room* room = findRoomByIdentifier(meeting.getRoomIdentifier());

        if (room == nullptr) {
            std::string msg = "Meeting " + meeting.getIdentifier() +
                              " verwijst naar onbekende room " +
                              meeting.getRoomIdentifier();

            logger.error("Fout: " + msg);

            conflicts.push_back(msg);
            consistent = false;
            continue;
        }

        if (static_cast<int>(meeting.getParticipants().size()) > room->getCapacity()) {
            std::string msg = "Room " + meeting.getRoomIdentifier() +
                              " heeft onvoldoende capaciteit voor meeting " +
                              meeting.getIdentifier();

            logger.error("Fout: " + msg);

            conflicts.push_back(msg);
            consistent = false;
        }

        if (meeting.hasCatering()) {
            const CateringProvider* provider =
                    findCateringProviderByCampus(room->getCampusIdentifier());

            if (provider == nullptr) {
                std::string msg = "Geen catering provider gevonden voor campus " +
                                  room->getCampusIdentifier() +
                                  " van meeting " + meeting.getIdentifier();

                logger.error("Fout: " + msg);

                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    if (consistent) {
        ENSURE(conflicts.empty(), "Bij consistent systeem mogen er geen conflicts zijn");
    }

    return consistent;
}

bool MeetingPlanner::processSingleMeeting(Meeting& meeting) {
    REQUIRE(!meeting.getIdentifier().empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");
    REQUIRE(!(meeting.isOnline() && meeting.hasCatering()),
            "Online meeting mag geen catering hebben");
    REQUIRE(meeting.isOnline() || !meeting.getRoomIdentifier().empty(),
            "Fysieke meeting moet een room identifier hebben");

    if (meeting.isOnline()) {
        meeting.setOccupancyPercentage(0);
        meeting.setCateringCost(0.0f);

        float meetingCO2 = calculateMeetingCO2(meeting);
        meeting.setCO2Emission(static_cast<float>(std::lround(meetingCO2)));
        totalCO2Emission += meetingCO2;

        logger.info("Meeting " + meeting.getIdentifier() + " vindt online plaats");

        ENSURE(meeting.getOccupancyPercentage() == 0,
               "Online meeting moet occupancy 0 hebben");
        ENSURE(meeting.getCateringCost() == 0.0f,
               "Online meeting moet 0 cateringkost hebben");
        ENSURE(meeting.getCO2Emission() >= 0,
               "Online meeting moet niet-negatieve CO2 hebben");

        return true;
    }

    Room* room = findRoomByIdentifier(meeting.getRoomIdentifier());

    if (room == nullptr) {
        std::string msg = "Meeting " + meeting.getIdentifier() +
                          " geannuleerd: onbekende room " +
                          meeting.getRoomIdentifier();

        logger.error(msg);

        size_t oldConflictsSize = conflicts.size();
        conflicts.push_back(msg);

        ENSURE(conflicts.size() == oldConflictsSize + 1,
               "Bij onbekende room moet conflict toegevoegd zijn");

        return false;
    }

    if (room->isBeingRenovated(meeting.getDate())) {
        std::string msg = "Meeting " + meeting.getIdentifier() +
                          " geannuleerd: room " + meeting.getRoomIdentifier() +
                          " is in renovatie op " + meeting.getDate() + ".";

        logger.error(msg);

        size_t oldConflictsSize = conflicts.size();
        conflicts.push_back(msg);

        ENSURE(conflicts.size() == oldConflictsSize + 1,
               "Bij renovatieconflict moet conflict toegevoegd zijn");

        return false;
    }





    if (meeting.hasCatering()) {
        const CateringProvider* provider =
                findCateringProviderByCampus(room->getCampusIdentifier());

        if (provider == nullptr) {
            std::string msg = "Meeting " + meeting.getIdentifier() +
                              " geannuleerd: geen catering provider voor campus " +
                              room->getCampusIdentifier();

            logger.error(msg);

            size_t oldConflictsSize = conflicts.size();
            conflicts.push_back(msg);

            ENSURE(conflicts.size() == oldConflictsSize + 1,
                   "Bij ontbrekende catering provider moet conflict toegevoegd zijn");

            return false;
        }
    }

    if (room->isOccupied()) {
        std::string msg = "Meeting " + meeting.getIdentifier() +
                          " geannuleerd: room " + room->getIdentifier() +
                          " is al bezet.";

        logger.error(msg);

        size_t oldConflictsSize = conflicts.size();
        conflicts.push_back(msg);

        ENSURE(conflicts.size() == oldConflictsSize + 1,
               "Bij bezettingsconflict moet conflict toegevoegd zijn");

        return false;
    }

    room->occupy();

    int occupancy = static_cast<int>(
            (meeting.getParticipants().size() * 100) / room->getCapacity());
    meeting.setOccupancyPercentage(occupancy);

    float meetingCO2 = calculateMeetingCO2(meeting);
    float cateringCO2 = 0.0f;
    float cateringCost = 0.0f;

    if (meeting.hasCatering()) {
        cateringCO2 = calculateCateringCO2(meeting, *room);
        cateringCost = calculateCateringCost(meeting);

        meeting.setCateringCost(cateringCost);
        totalCateringCost += cateringCost;

        appendCateringDeliveryToFile(meeting, *room);
    } else {
        meeting.setCateringCost(0.0f);
    }

    float totalMeetingCO2 = meetingCO2 + cateringCO2;
    meeting.setCO2Emission(static_cast<float>(std::lround(totalMeetingCO2)));
    totalCO2Emission += totalMeetingCO2;

    logger.info("Meeting " + meeting.getIdentifier() +
                " vindt plaats in room " +
                room->getIdentifier());

    ENSURE(room->isOccupied(), "Room moet bezet zijn na processing");
    ENSURE(meeting.getCateringCost() >= 0.0f,
           "Meeting moet niet-negatieve cateringkost hebben");
    ENSURE(meeting.getCO2Emission() >= 0,
           "Meeting moet niet-negatieve CO2 hebben");

    return true;
}

void MeetingPlanner::processMeetings() {
    successfulMeetings.clear();
    conflicts.clear();
    totalCO2Emission = 0.0f;
    totalCateringCost = 0.0f;

    std::ofstream cateringFile("catering_deliveries.txt");
    if (cateringFile.is_open()) {
        cateringFile << "CATERING DELIVERIES" << '\n';
        cateringFile << "========================================" << '\n';
        cateringFile.close();
    }

    for (auto& meeting : meetings) {
        bool success = processSingleMeeting(meeting);

        if (success) {
            successfulMeetings.push_back(meeting);
        }
    }

    ENSURE(successfulMeetings.size() <= meetings.size(),
           "Aantal succesvolle meetings mag niet groter zijn dan totaal aantal meetings");
}

void MeetingPlanner::addCampus(const Campus& campus) {
    size_t oldSize = campuses.size();
    campuses.push_back(campus);
    ENSURE(campuses.size() == oldSize + 1, "Campus moet toegevoegd zijn");
}

void MeetingPlanner::addBuilding(const Building& building) {
    size_t oldSize = buildings.size();
    buildings.push_back(building);
    ENSURE(buildings.size() == oldSize + 1, "Building moet toegevoegd zijn");
}

void MeetingPlanner::addRenovation(const Renovation& renovation) {
    REQUIRE(!renovation.getRoomIdentifier().empty(), "Renovation room identifier mag niet leeg zijn");
    REQUIRE(!renovation.getStartDate().empty(), "Renovation start date mag niet leeg zijn");
    REQUIRE(!renovation.getEndDate().empty(), "Renovation end date mag niet leeg zijn");
    REQUIRE(roomExists(renovation.getRoomIdentifier()),
            "Renovation moet verwijzen naar een bestaande room");
    REQUIRE(renovation.getStartDate() <= renovation.getEndDate(),
            "Renovation start date moet voor of gelijk aan end date liggen");

    size_t oldSize = renovations.size();
    renovations.push_back(renovation);
    Room* room = findRoomByIdentifier(renovation.getRoomIdentifier());

    if (room != nullptr) {
        room->addRenovation(renovation);
    }

    ENSURE(renovations.size() == oldSize + 1, "Renovation moet toegevoegd zijn");
}

void MeetingPlanner::addCateringProvider(const CateringProvider& provider) {
    size_t oldSize = cateringProviders.size();
    cateringProviders.push_back(provider);
    ENSURE(cateringProviders.size() == oldSize + 1, "Catering provider moet toegevoegd zijn");
}

const std::vector<CateringProvider>& MeetingPlanner::getCateringProviders() const {
    return cateringProviders;
}

const std::vector<Renovation>& MeetingPlanner::getRenovations() const {
    return renovations;
}

const std::vector<Campus>& MeetingPlanner::getCampuses() const {
    return campuses;
}

const std::vector<Building>& MeetingPlanner::getBuildings() const {
    return buildings;
}

const std::vector<Room>& MeetingPlanner::getRooms() const {
    return rooms;
}

const std::vector<Meeting>& MeetingPlanner::getMeetings() const {
    return meetings;
}

const std::vector<Meeting>& MeetingPlanner::getSuccessfulMeetings() const {
    return successfulMeetings;
}

const std::vector<std::string>& MeetingPlanner::getConflicts() const {
    return conflicts;
}

float MeetingPlanner::getTotalCO2Emission() const {
    return totalCO2Emission;
}

float MeetingPlanner::getTotalCateringCost() const {
    return totalCateringCost;
}