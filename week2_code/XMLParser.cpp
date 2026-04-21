//
// Created by firdi on 21/04/2026.
//
#include "XMLParser.h"
#include <iostream>
#include "tinyxml/tinyxml.h"

bool XMLParser::parse(const std::string& filename, MeetingPlanner& planner) const {
    TiXmlDocument doc(filename.c_str());

    if (!doc.LoadFile()) {
        std::cerr << "Fout bij openen van XML-bestand: " << doc.ErrorDesc() << std::endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("SYSTEM");
    if (root == nullptr) {
        std::cerr << "Fout: geen SYSTEM root element gevonden." << std::endl;
        return false;
    }

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
                planner.addRoom(room);
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
                planner.addMeeting(meeting);
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

            bool found = planner.addParticipation(meetingId, user);
            if (!found) {
                std::cerr << "Fout: participation verwijst naar onbekende meeting: "
                          << meetingId << std::endl;
            }
        }
    }

    return true;
}