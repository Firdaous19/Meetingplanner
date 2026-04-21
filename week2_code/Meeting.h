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

    /**
     * Geef het label van de meeting terug.
     */
    std::string getLabel() const { return label; }

    /**
     * Geef de identifier van de meeting terug.
     */
    std::string getIdentifier() const { return identifier; }

    /**
     * Geef de room identifier van de meeting terug.
     */
    std::string getRoomIdentifier() const { return roomIdentifier; }

    /**
     * Geef de datum van de meeting terug.
     */
    std::string getDate() const { return date; }

    /**
     * Voeg een participant toe aan de meeting.
     * @param user De naam van de participant.
     */
    void addParticipant(const std::string& user);

    /**
     * Geef alle deelnemers van de meeting terug.
     */
    const std::vector<std::string>& getParticipants() const { return participants; }

private:
    std::string label;
    std::string identifier;
    std::string roomIdentifier;
    std::string date;
    std::vector<std::string> participants;
};

#endif