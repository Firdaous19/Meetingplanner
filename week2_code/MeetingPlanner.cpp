//
// Created by firdi on 21/04/2026.
//
#include "MeetingPlanner.h"
#include <iostream>

void MeetingPlanner::addRoom(const Room& room) {
    rooms.push_back(room);
}

void MeetingPlanner::addMeeting(const Meeting& meeting) {
    meetings.push_back(meeting);
}

bool MeetingPlanner::addParticipation(const std::string& meetingId, const std::string& user) {
    for (auto& meeting : meetings) {
        if (meeting.getIdentifier() == meetingId) {
            meeting.addParticipant(user);
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
                std::cerr << "Fout: " << msg << std::endl;
                conflicts.push_back(msg);
                consistent = false;
            }
        }
    }

    for (size_t i = 0; i < meetings.size(); i++) {
        for (size_t j = i + 1; j < meetings.size(); j++) {
            if (meetings[i].getIdentifier() == meetings[j].getIdentifier()) {
                std::string msg = "Dubbele meeting identifier: " + meetings[i].getIdentifier();
                std::cerr << "Fout: " << msg << std::endl;
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
            std::cerr << "Fout: " << msg << std::endl;
            conflicts.push_back(msg);
            consistent = false;
        } else if ((int)meeting.getParticipants().size() > roomCapacity) {
            std::string msg = "Room " + meeting.getRoomIdentifier() +
                              " heeft onvoldoende capaciteit voor meeting " +
                              meeting.getIdentifier();
            std::cerr << "Fout: " << msg << std::endl;
            conflicts.push_back(msg);
            consistent = false;
        }
    }

    return consistent;
}

const std::vector<Room>& MeetingPlanner::getRooms() const {
    return rooms;
}

const std::vector<Meeting>& MeetingPlanner::getMeetings() const {
    return meetings;
}

const std::vector<std::string>& MeetingPlanner::getConflicts() const {
    return conflicts;
}