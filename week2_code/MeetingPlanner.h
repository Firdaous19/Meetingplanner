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

class MeetingPlanner {
public:
    void addRoom(const Room& room);
    void addMeeting(const Meeting& meeting);
    bool addParticipation(const std::string& meetingId, const std::string& user);

    bool checkConsistency();
    void processMeetings();
    void addCampus(const Campus& campus);
    void addBuilding(const Building& building);
    void addRenovation(const Renovation& renovation);
    void setLoggingEnabled(bool enabled);
    void addCateringProvider(const CateringProvider& provider);
    const std::vector<CateringProvider>& getCateringProviders() const;

    const std::vector<Renovation>& getRenovations() const;
    const std::vector<Campus>& getCampuses() const;
    const std::vector<Building>& getBuildings() const;
    const std::vector<Room>& getRooms() const;
    const std::vector<Meeting>& getMeetings() const;
    const std::vector<Meeting>& getSuccessfulMeetings() const;
    const std::vector<std::string>& getConflicts() const;

private:
    bool loggingEnabled = true;

    bool processSingleMeeting(const Meeting& meeting);
    bool isRoomUnderRenovation(const std::string& roomIdentifier, const std::string& date) const;
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