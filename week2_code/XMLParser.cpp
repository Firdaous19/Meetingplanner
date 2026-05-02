//
// Created by firdi on 21/04/2026.
//
#include "XMLParser.h"
#include <iostream>
#include <string>
#include "tinyxml/tinyxml.h"
#include "Campus.h"
#include "Building.h"
#include "Renovation.h"
#include "CateringProvider.h"

namespace {
    bool parseBooleanText(const std::string& text, bool& value) {
        if (text == "TRUE" || text == "true" || text == "1") {
            value = true;
            return true;
        }
        if (text == "FALSE" || text == "false" || text == "0") {
            value = false;
            return true;
        }
        return false;
    }
}

XMLParser::XMLParser()
        : loggingEnabled(true) {
}

void XMLParser::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
}

bool XMLParser::parse(const std::string& filename, MeetingPlanner& planner) const {
    TiXmlDocument doc(filename.c_str());

    if (!doc.LoadFile()) {
        if (loggingEnabled) {
            std::cerr << "Fout bij openen van XML-bestand: " << doc.ErrorDesc() << std::endl;
        }
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("SYSTEM");
    if (root == nullptr) {
        if (loggingEnabled) {
            std::cerr << "Fout: geen SYSTEM root element gevonden." << std::endl;
        }
        return false;
    }

    for (TiXmlElement* elem = root->FirstChildElement();
         elem != nullptr;
         elem = elem->NextSiblingElement()) {

        std::string elementName = elem->Value();

        if (elementName == "CAMPUS") {
            TiXmlElement* nameElem = elem->FirstChildElement("NAME");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");

            if (nameElem == nullptr || idElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in CAMPUS: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string name = nameElem->GetText() ? nameElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";

            if (name.empty() || identifier.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in CAMPUS: lege velden." << std::endl;
                }
                continue;
            }

            try {
                Campus campus(name, identifier);
                planner.addCampus(campus);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van campus: " << name << std::endl;
                }
            }
        }

        else if (elementName == "BUILDING") {
            TiXmlElement* nameElem = elem->FirstChildElement("NAME");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");
            TiXmlElement* campusElem = elem->FirstChildElement("CAMPUS");

            if (nameElem == nullptr || idElem == nullptr || campusElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in BUILDING: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string name = nameElem->GetText() ? nameElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";
            std::string campusIdentifier = campusElem->GetText() ? campusElem->GetText() : "";

            if (name.empty() || identifier.empty() || campusIdentifier.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in BUILDING: lege velden." << std::endl;
                }
                continue;
            }

            try {
                Building building(name, identifier, campusIdentifier);
                planner.addBuilding(building);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van building: " << name << std::endl;
                }
            }
        }

        else if (elementName == "CATERING") {
            TiXmlElement* campusElem = elem->FirstChildElement("CAMPUS");
            TiXmlElement* co2Elem = elem->FirstChildElement("CO2");

            if (campusElem == nullptr || co2Elem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in CATERING: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string campusIdentifier = campusElem->GetText() ? campusElem->GetText() : "";
            std::string co2Text = co2Elem->GetText() ? co2Elem->GetText() : "";

            if (campusIdentifier.empty() || co2Text.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in CATERING: lege velden." << std::endl;
                }
                continue;
            }

            int co2 = 0;
            try {
                co2 = std::stoi(co2Text);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout in CATERING: CO2 is geen geldig getal." << std::endl;
                }
                continue;
            }

            try {
                CateringProvider provider(campusIdentifier, co2);
                planner.addCateringProvider(provider);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van catering provider voor campus: "
                              << campusIdentifier << std::endl;
                }
            }
        }

        else if (elementName == "RENOVATION") {
            TiXmlElement* roomElem = elem->FirstChildElement("ROOM");
            TiXmlElement* startElem = elem->FirstChildElement("START_DATE");
            TiXmlElement* endElem = elem->FirstChildElement("END_DATE");

            if (roomElem == nullptr || startElem == nullptr || endElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in RENOVATION: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string roomIdentifier = roomElem->GetText() ? roomElem->GetText() : "";
            std::string startDate = startElem->GetText() ? startElem->GetText() : "";
            std::string endDate = endElem->GetText() ? endElem->GetText() : "";

            if (roomIdentifier.empty() || startDate.empty() || endDate.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in RENOVATION: lege velden." << std::endl;
                }
                continue;
            }

            try {
                Renovation renovation(roomIdentifier, startDate, endDate);
                planner.addRenovation(renovation);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van renovation voor room: "
                              << roomIdentifier << std::endl;
                }
            }
        }

        else if (elementName == "ROOM") {
            TiXmlElement* nameElem = elem->FirstChildElement("NAME");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");
            TiXmlElement* capElem = elem->FirstChildElement("CAPACITY");

            if (nameElem == nullptr || idElem == nullptr || capElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in ROOM: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string name = nameElem->GetText() ? nameElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";
            std::string capacityText = capElem->GetText() ? capElem->GetText() : "";

            if (name.empty() || identifier.empty() || capacityText.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in ROOM: lege velden." << std::endl;
                }
                continue;
            }

            int capacity = 0;
            try {
                capacity = std::stoi(capacityText);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout in ROOM: capacity is geen geldig getal." << std::endl;
                }
                continue;
            }

            try {
                Room room(name, identifier, capacity);
                planner.addRoom(room);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van room: " << name << std::endl;
                }
            }
        }

        else if (elementName == "MEETING") {
            TiXmlElement* labelElem = elem->FirstChildElement("LABEL");
            TiXmlElement* idElem = elem->FirstChildElement("IDENTIFIER");
            TiXmlElement* roomElem = elem->FirstChildElement("ROOM");
            TiXmlElement* dateElem = elem->FirstChildElement("DATE");
            TiXmlElement* onlineElem = elem->FirstChildElement("ONLINE");
            TiXmlElement* cateringElem = elem->FirstChildElement("CATERING");

            if (labelElem == nullptr || idElem == nullptr || dateElem == nullptr || onlineElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in MEETING: ontbrekende verplichte velden." << std::endl;
                }
                continue;
            }

            std::string label = labelElem->GetText() ? labelElem->GetText() : "";
            std::string identifier = idElem->GetText() ? idElem->GetText() : "";
            std::string roomIdentifier = roomElem && roomElem->GetText() ? roomElem->GetText() : "";
            std::string date = dateElem->GetText() ? dateElem->GetText() : "";
            std::string onlineText = onlineElem->GetText() ? onlineElem->GetText() : "";

            bool online = false;
            bool catering = false;

            if (label.empty() || identifier.empty() || date.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in MEETING: lege verplichte velden." << std::endl;
                }
                continue;
            }

            if (!parseBooleanText(onlineText, online)) {
                if (loggingEnabled) {
                    std::cerr << "Fout in MEETING: ONLINE moet TRUE of FALSE zijn." << std::endl;
                }
                continue;
            }

            if (cateringElem != nullptr) {
                std::string cateringText = cateringElem->GetText() ? cateringElem->GetText() : "";

                if (!parseBooleanText(cateringText, catering)) {
                    if (loggingEnabled) {
                        std::cerr << "Fout in MEETING: CATERING moet TRUE of FALSE zijn." << std::endl;
                    }
                    continue;
                }
            }

            // Use case 3.4:
            // - online meeting heeft geen room nodig
            // - fysieke meeting moet wel een room hebben
            // - online meeting mag geen catering hebben
            if (online && catering) {
                if (loggingEnabled) {
                    std::cerr << "Fout in MEETING: online meeting mag geen catering hebben." << std::endl;
                }
                continue;
            }

            if (!online && roomIdentifier.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in MEETING: fysieke meeting moet een room hebben." << std::endl;
                }
                continue;
            }

            try {
                Meeting meeting(label, identifier, roomIdentifier, date);

                // Eerst online zetten, daarna catering,
                // zodat de contracten logisch blijven.
                meeting.setOnline(online);
                meeting.setCatering(catering);

                planner.addMeeting(meeting);
            } catch (...) {
                if (loggingEnabled) {
                    std::cerr << "Fout bij maken van meeting: " << label << std::endl;
                }
            }
        }

        else if (elementName == "PARTICIPATION") {
            TiXmlElement* meetingElem = elem->FirstChildElement("MEETING");
            TiXmlElement* userElem = elem->FirstChildElement("USER");

            if (meetingElem == nullptr || userElem == nullptr) {
                if (loggingEnabled) {
                    std::cerr << "Fout in PARTICIPATION: ontbrekende velden." << std::endl;
                }
                continue;
            }

            std::string meetingId = meetingElem->GetText() ? meetingElem->GetText() : "";
            std::string user = userElem->GetText() ? userElem->GetText() : "";

            if (meetingId.empty() || user.empty()) {
                if (loggingEnabled) {
                    std::cerr << "Fout in PARTICIPATION: lege velden." << std::endl;
                }
                continue;
            }

            bool found = planner.addParticipation(meetingId, user);
            if (!found) {
                if (loggingEnabled) {
                    std::cerr << "Fout: participation verwijst naar onbekende meeting: "
                              << meetingId << std::endl;
                }
            }
        }
    }

    return true;
}