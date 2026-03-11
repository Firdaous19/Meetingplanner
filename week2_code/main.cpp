#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "tinyxml/tinyxml.h"
#include "Room.h"
#include "Meeting.h"
#include "DesignByContract.h"

std::string formatDate(const std::string& isoDate) {
    // Verwacht formaat YYYY-MM-DD
    if (isoDate.size() != 10) {
        return isoDate;
    }

    std::string year = isoDate.substr(0, 4);
    std::string month = isoDate.substr(5, 2);
    std::string day = isoDate.substr(8, 2);

    return day + "/" + month + "/" + year;
}

int main() {
    TiXmlDocument doc("../week2_code/test.xml");

    if (!doc.LoadFile()) {
        std::cerr << "Fout bij openen van XML-bestand: " << doc.ErrorDesc() << std::endl;
        return 1;
    }

    TiXmlElement* root = doc.FirstChildElement("SYSTEM");
    if (root == nullptr) {
        std::cerr << "Fout: geen SYSTEM root element gevonden." << std::endl;
        return 1;
    }

    std::vector<Room> rooms;
    std::vector<Meeting> meetings;
    std::vector<std::string> conflicts;

    // -----------------------------
    // XML inlezen
    // -----------------------------
    for (TiXmlElement* elem = root->FirstChildElement();
         elem != nullptr;
         elem = elem->NextSiblingElement()) {

        std::string elementName = elem->Value();

        if (elementName == "ROOM") {
            TiXmlElement* nameElem = elem->FirstChildElement("NAME");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");
            TiXmlElement* capElem = elem->FirstChildElement("CAPACITY");

            if (nameElem == nullptr || idElem == nullptr || capElem == nullptr) {
                std::cerr << "Fout in ROOM: ontbrekende velden." << std::endl;
                continue;
            }

            std::string name = nameElem->GetText() ? nameElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";
            std::string capacityText = capElem->GetText() ? capElem->GetText() : "";

            if (name.empty() || identifier.empty() || capacityText.empty()) {
                std::cerr << "Fout in ROOM: lege velden." << std::endl;
                continue;
            }

            int capacity = 0;
            try {
                capacity = std::stoi(capacityText);
            } catch (...) {
                std::cerr << "Fout in ROOM: capacity is geen geldig getal." << std::endl;
                continue;
            }

            try {
                Room room(name, identifier, capacity);
                rooms.push_back(room);

                std::cout << "Room geladen: " << name
                          << " (" << identifier << "), capaciteit = "
                          << capacity << std::endl;
            } catch (...) {
                std::cerr << "Fout bij maken van room: " << name << std::endl;
            }
        }

        else if (elementName == "MEETING") {
            TiXmlElement* labelElem = elem->FirstChildElement("LABEL");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");
            TiXmlElement* roomElem = elem->FirstChildElement("ROOM");
            TiXmlElement* dateElem = elem->FirstChildElement("DATE");

            if (labelElem == nullptr || idElem == nullptr || roomElem == nullptr || dateElem == nullptr) {
                std::cerr << "Fout in MEETING: ontbrekende velden." << std::endl;
                continue;
            }

            std::string label = labelElem->GetText() ? labelElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";
            std::string roomIdentifier = roomElem->GetText() ? roomElem->GetText() : "";
            std::string date = dateElem->GetText() ? dateElem->GetText() : "";

            if (label.empty() || identifier.empty() || roomIdentifier.empty() || date.empty()) {
                std::cerr << "Fout in MEETING: lege velden." << std::endl;
                continue;
            }

            try {
                Meeting meeting(label, identifier, roomIdentifier, date);
                meetings.push_back(meeting);

                std::cout << "Meeting geladen: " << label
                          << " (" << identifier << "), room = "
                          << roomIdentifier << ", date = " << date << std::endl;
            } catch (...) {
                std::cerr << "Fout bij maken van meeting: " << label << std::endl;
            }
        }

        else if (elementName == "PARTICIPATION") {
            TiXmlElement* meetingElem = elem->FirstChildElement("MEETING");
            TiXmlElement* userElem = elem->FirstChildElement("USER");

            if (meetingElem == nullptr || userElem == nullptr) {
                std::cerr << "Fout in PARTICIPATION: ontbrekende velden." << std::endl;
                continue;
            }

            std::string meetingId = meetingElem->GetText() ? meetingElem->GetText() : "";
            std::string user = userElem->GetText() ? userElem->GetText() : "";

            if (meetingId.empty() || user.empty()) {
                std::cerr << "Fout in PARTICIPATION: lege velden." << std::endl;
                continue;
            }

            bool foundMeeting = false;
            for (auto& meeting : meetings) {
                if (meeting.getIdentifier() == meetingId) {
                    meeting.addParticipant(user);
                    foundMeeting = true;
                    break;
                }
            }

            if (!foundMeeting) {
                std::cerr << "Fout: participation verwijst naar onbekende meeting: "
                          << meetingId << std::endl;
            }
        }
    }

    // -----------------------------
    // Consistentiechecks
    // -----------------------------
    bool consistent = true;

    // Dubbele room identifiers
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

    // Dubbele meeting identifiers
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

    // Check of room van meeting bestaat + capaciteit genoeg is
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

    // -----------------------------
    // Simpele meeting processing
    // -----------------------------
    for (const auto& meeting : meetings) {
        std::cout << "Meeting " << meeting.getIdentifier()
                  << " vindt plaats in room "
                  << meeting.getRoomIdentifier() << std::endl;
    }

    // -----------------------------
    // Outputbestand maken
    // -----------------------------
    std::ofstream out("output.txt");
    if (!out.is_open()) {
        std::cerr << "Fout: outputbestand kon niet gemaakt worden." << std::endl;
        return 1;
    }

    out << "Past meetings:\n";
    // voorlopig leeg, tenzij je later datums vergelijkt

    out << "\nFuture meetings:\n";
    for (const auto& meeting : meetings) {
        out << "- " << meeting.getRoomIdentifier()
            << ", " << formatDate(meeting.getDate()) << "\n";
        out << meeting.getLabel() << "\n";

        const auto& participants = meeting.getParticipants();
        for (size_t i = 0; i < participants.size(); i++) {
            out << participants[i];
            if (i < participants.size() - 1) {
                out << ", ";
            }
        }
        out << "\n";
        out << "Meeting ID: " << meeting.getIdentifier() << "\n";
    }

    out << "\nConflicts:\n";
    for (const auto& conflict : conflicts) {
        out << "- " << conflict << "\n";
    }

    out << "\nRooms:\n";
    for (const auto& room : rooms) {
        out << "- " << room.getIdentifier() << "\n";
        out << "Capacity: " << room.getCapacity() << " people\n";
    }

    out.close();

    // -----------------------------
    // Samenvatting op scherm
    // -----------------------------
    std::cout << "Aantal rooms geladen: " << rooms.size() << std::endl;
    std::cout << "Aantal meetings geladen: " << meetings.size() << std::endl;

    for (const auto& meeting : meetings) {
        std::cout << "Meeting " << meeting.getIdentifier()
                  << " heeft " << meeting.getParticipants().size()
                  << " deelnemers." << std::endl;
    }

    std::cout << "Outputbestand gemaakt: output.txt" << std::endl;

    if (!consistent) {
        std::cerr << "Systeem is inconsistent." << std::endl;
        return 1;
    }

    return 0;
}