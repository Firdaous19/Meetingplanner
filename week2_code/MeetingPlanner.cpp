#include "MeetingPlanner.h"
#include <iostream>


void MeetingPlanner::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
}

void MeetingPlanner::addRoom(const Room& room) {
    size_t oldSize = rooms.size();
    rooms.push_back(room);
    ENSURE(rooms.size() == oldSize + 1, "Room moet toegevoegd zijn");
}

void MeetingPlanner::addMeeting(const Meeting& meeting) {
    size_t oldSize = meetings.size();
    meetings.push_back(meeting);
    ENSURE(meetings.size() == oldSize + 1, "Meeting moet toegevoegd zijn");
}

bool MeetingPlanner::addParticipation(const std::string& meetingId, const std::string& user) {
    REQUIRE(!meetingId.empty(), "Meeting ID mag niet leeg zijn");
    REQUIRE(!user.empty(), "User mag niet leeg zijn");

    for (auto& meeting : meetings) {
        if (meeting.getIdentifier() == meetingId) {
            size_t oldSize = meeting.getParticipants().size();
            meeting.addParticipant(user);
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

bool MeetingPlanner::isRoomUnderRenovation(const std::string& roomIdentifier, const std::string& date) const {
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

bool MeetingPlanner::checkConsistency() {
    bool consistent = true;
    conflicts.clear();

    for (size_t i = 0; i < rooms.size(); i++) {
        for (size_t j = i + 1; j < rooms.size(); j++) {
            if (rooms[i].getIdentifier() == rooms[j].getIdentifier()) {
                std::string msg = "Dubbele room identifier: " + rooms[i].getIdentifier();

                if (loggingEnabled) {
                    std::cerr << "Fout: " << msg << std::endl;
                }

                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (size_t i = 0; i < meetings.size(); i++) {
        for (size_t j = i + 1; j < meetings.size(); j++) {
            if (meetings[i].getIdentifier() == meetings[j].getIdentifier()) {
                std::string msg = "Dubbele meeting identifier: " + meetings[i].getIdentifier();

                if (loggingEnabled) {
                    std::cerr << "Fout: " << msg << std::endl;
                }

                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (const auto& meeting : meetings) {
        bool roomFound = false;
        int roomCapacity = 0;

        for (const auto& room : rooms) {
            if (room.getIdentifier() == meeting.getRoomIdentifier()) {
                roomFound = true;
                roomCapacity = room.getCapacity();
                break;
            }
        }

        if (!roomFound) {
            std::string msg = "Meeting " + meeting.getIdentifier() +
                              " verwijst naar onbekende room " +
                              meeting.getRoomIdentifier();

            if (loggingEnabled) {
                std::cerr << "Fout: " << msg << std::endl;
            }

            conflicts.push_back(msg);
            consistent = false;
        } else if ((int)meeting.getParticipants().size() > roomCapacity) {
            std::string msg = "Room " + meeting.getRoomIdentifier() +
                              " heeft onvoldoende capaciteit voor meeting " +
                              meeting.getIdentifier();

            if (loggingEnabled) {
                std::cerr << "Fout: " << msg << std::endl;
            }

            conflicts.push_back(msg);
            consistent = false;
        }
    }

    if (consistent) {
        ENSURE(conflicts.empty(), "Bij consistent systeem mogen er geen conflicts zijn");
    }

    return consistent;
}

bool MeetingPlanner::processSingleMeeting(const Meeting& meeting) {
    REQUIRE(!meeting.getIdentifier().empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!meeting.getRoomIdentifier().empty(), "Meeting room identifier mag niet leeg zijn");
    REQUIRE(!meeting.getDate().empty(), "Meeting date mag niet leeg zijn");

    if (isRoomUnderRenovation(meeting.getRoomIdentifier(), meeting.getDate())) {
        std::string msg = "Meeting " + meeting.getIdentifier() +
                          " geannuleerd: room " + meeting.getRoomIdentifier() +
                          " is in renovatie op " + meeting.getDate() + ".";

        if (loggingEnabled) {
            std::cerr << msg << std::endl;
        }

        size_t oldConflictsSize = conflicts.size();
        conflicts.push_back(msg);

        ENSURE(conflicts.size() == oldConflictsSize + 1,
               "Bij renovatieconflict moet conflict toegevoegd zijn");

        return false;
    }

    for (auto& room : rooms) {
        if (room.getIdentifier() == meeting.getRoomIdentifier()) {
            if (room.isOccupied()) {
                std::string msg = "Meeting " + meeting.getIdentifier() +
                                  " geannuleerd: room " + room.getIdentifier() +
                                  " is al bezet.";

                if (loggingEnabled) {
                    std::cerr << msg << std::endl;
                }

                size_t oldConflictsSize = conflicts.size();
                conflicts.push_back(msg);

                ENSURE(conflicts.size() == oldConflictsSize + 1,
                       "Bij bezettingsconflict moet conflict toegevoegd zijn");

                return false;
            }

            room.occupy();
            int occupancy = (meeting.getParticipants().size() * 100) / room.getCapacity();
            const_cast<Meeting&>(meeting).setOccupancyPercentage(occupancy);

            if (loggingEnabled) {
                std::cout << "Meeting " << meeting.getIdentifier()
                          << " vindt plaats in room "
                          << room.getIdentifier() << std::endl;
            }

            ENSURE(room.isOccupied(), "Room moet bezet zijn na processing");
            return true;
        }
    }

    std::string msg = "Meeting " + meeting.getIdentifier() +
                      " geannuleerd: onbekende room " +
                      meeting.getRoomIdentifier();

    if (loggingEnabled) {
        std::cerr << msg << std::endl;
    }

    size_t oldConflictsSize = conflicts.size();
    conflicts.push_back(msg);

    ENSURE(conflicts.size() == oldConflictsSize + 1,
           "Bij onbekende room moet conflict toegevoegd zijn");

    return false;
}

void MeetingPlanner::processMeetings() {
    successfulMeetings.clear();

    for (const auto& meeting : meetings) {
        bool success = processSingleMeeting(meeting);

        if (success) {
            Meeting processedMeeting = meeting;

            for (const auto& room : rooms) {
                if (room.getIdentifier() == meeting.getRoomIdentifier()) {
                    int occupancy =
                            (meeting.getParticipants().size() * 100) / room.getCapacity();

                    processedMeeting.setOccupancyPercentage(occupancy);
                    break;
                }
            }

            successfulMeetings.push_back(processedMeeting);
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