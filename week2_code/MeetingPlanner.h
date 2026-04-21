//
// Created by firdi on 21/04/2026.
//

#ifndef PROJECTTITLE_MEETINGPLANNER_H
#define PROJECTTITLE_MEETINGPLANNER_H
#include <vector>
#include <string>
#include "Room.h"
#include "Meeting.h"

class MeetingPlanner {
public:
    void addRoom(const Room& room);
    void addMeeting(const Meeting& meeting);
    bool addParticipation(const std::string& meetingId, const std::string& user);

    bool checkConsistency();

    const std::vector<Room>& getRooms() const;
    const std::vector<Meeting>& getMeetings() const;
    const std::vector<std::string>& getConflicts() const;

private:
    std::vector<Room> rooms;
    std::vector<Meeting> meetings;
    std::vector<std::string> conflicts;
};
#endif //PROJECTTITLE_MEETINGPLANNER_H
