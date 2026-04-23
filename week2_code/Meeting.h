#ifndef PROJECTTITLE_MEETING_H
#define PROJECTTITLE_MEETING_H

#include <string>
#include <vector>

/**
 * Stelt een meeting voor in het systeem.
 * Een meeting heeft een label, identifier, gekoppelde room,
 * datum en een lijst van deelnemers.
 */
class Meeting {
public:
    /**
     * Constructor van een meeting.
     * @param label De naam of titel van de meeting.
     * @param identifier De unieke identifier van de meeting.
     * @param roomIdentifier De identifier van de gekoppelde room.
     * @param date De datum van de meeting.
     */
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

    bool areExternalsAllowed() const { return externalsAllowed; }
    bool hasCatering() const { return catering; }
    bool isOnline() const { return online; }
    int getCO2Emission() const { return co2Emission; }

    void setExternalsAllowed(bool value) { externalsAllowed = value; }
    void setCatering(bool value) { catering = value; }
    void setOnline(bool value) { online = value; }
    void setCO2Emission(int value) { co2Emission = value; }

private:
    std::string label;
    std::string identifier;
    std::string roomIdentifier;
    std::string date;
    std::vector<std::string> participants;

    bool externalsAllowed = false;
    bool catering = false;
    bool online = false;
    int co2Emission = 0;
};

#endif