//
// Created by firdi on 11/03/2026.
//

#ifndef PROJECTTITLE_MEETING_H
#define PROJECTTITLE_MEETING_H
#include <string>
#include <vector>

class Meeting {
public:
    Meeting(const std::string& label,
            const std::string& identifier,
            const std::string& roomIdentifier,
            const std::string& date);

    std::string getLabel() const { return label; }
    std::string getIdentifier() const { return identifier; }
    std::string getRoomIdentifier() const { return roomIdentifier; }
    std::string getDate() const { return date; }

    void addParticipant(const std::string& user);
    const std::vector<std::string>& getParticipants() const { return participants; }

private:
    std::string label;
    std::string identifier;
    std::string roomIdentifier;
    std::string date;
    std::vector<std::string> participants;
};

#endif
