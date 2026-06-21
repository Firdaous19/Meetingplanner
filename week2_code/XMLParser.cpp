//
// Created by firdi on 21/04/2026.
//
#include "XMLParser.h"
#include "DesignByContract.h"

#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "tinyxml/tinyxml.h"
#include "Campus.h"
#include "Building.h"
#include "Renovation.h"
#include "CateringProvider.h"

namespace {

    std::string trim(const std::string& value) {
        std::string::size_type first = 0;

        while (first < value.size() &&
               std::isspace(static_cast<unsigned char>(value[first]))) {
            ++first;
        }

        std::string::size_type last = value.size();

        while (last > first &&
               std::isspace(static_cast<unsigned char>(value[last - 1]))) {
            --last;
        }

        return value.substr(first, last - first);
    }

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

    bool parsePositiveInt(const std::string& text, int& value) {
        std::size_t parsedCharacters = 0;

        try {
            value = std::stoi(text, &parsedCharacters);
        } catch (...) {
            return false;
        }

        return parsedCharacters == text.size() && value > 0;
    }

    bool parsePositiveFloat(const std::string& text, float& value) {
        std::size_t parsedCharacters = 0;

        try {
            value = std::stof(text, &parsedCharacters);
        } catch (...) {
            return false;
        }

        return parsedCharacters == text.size() &&
               std::isfinite(value) &&
               value > 0.0f;
    }

    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) ||
               year % 400 == 0;
    }

    bool isValidDateFormat(const std::string& date) {
        if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
            return false;
        }

        for (std::size_t i = 0; i < date.size(); ++i) {
            if (i == 4 || i == 7) {
                continue;
            }

            if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
                return false;
            }
        }

        const int year = std::stoi(date.substr(0, 4));
        const int month = std::stoi(date.substr(5, 2));
        const int day = std::stoi(date.substr(8, 2));

        if (year < 1 || month < 1 || month > 12) {
            return false;
        }

        int daysInMonth = 0;

        switch (month) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                daysInMonth = 31;
                break;

            case 4:
            case 6:
            case 9:
            case 11:
                daysInMonth = 30;
                break;

            case 2:
                daysInMonth = isLeapYear(year) ? 29 : 28;
                break;

            default:
                return false;
        }

        return day >= 1 && day <= daysInMonth;
    }

    bool readRequiredText(TiXmlElement* parent,
                          const char* childName,
                          const char* parentName,
                          std::string& value,
                          std::ostream& errStream) {
        TiXmlElement* child = parent->FirstChildElement(childName);

        if (child == nullptr) {
            errStream << "XML PARTIAL IMPORT: Expected <"
                      << childName
                      << "> ... </"
                      << childName
                      << "> in <"
                      << parentName
                      << ">."
                      << std::endl;

            return false;
        }

        value = trim(child->GetText() ? child->GetText() : "");

        if (value.empty()) {
            errStream << "XML PARTIAL IMPORT: Expected non-empty <"
                      << childName
                      << "> ... </"
                      << childName
                      << "> in <"
                      << parentName
                      << ">."
                      << std::endl;

            return false;
        }

        return true;
    }

    std::string readOptionalText(TiXmlElement* parent,
                                 const char* childName) {
        TiXmlElement* child = parent->FirstChildElement(childName);

        if (child == nullptr || child->GetText() == nullptr) {
            return "";
        }

        return trim(child->GetText());
    }

    bool roomExistsInPlanner(const MeetingPlanner& planner,
                             const std::string& roomIdentifier) {
        const std::vector<Room>& rooms = planner.getRooms();

        for (const Room& room : rooms) {
            if (room.getIdentifier() == roomIdentifier) {
                return true;
            }
        }

        return false;
    }

    const Meeting* findMeetingInPlanner(
            const MeetingPlanner& planner,
            const std::string& meetingIdentifier) {
        const std::vector<Meeting>& meetings = planner.getMeetings();

        for (const Meeting& meeting : meetings) {
            if (meeting.getIdentifier() == meetingIdentifier) {
                return &meeting;
            }
        }

        return nullptr;
    }
}

XMLParser::XMLParser()
        : loggingEnabled(true) {
}

void XMLParser::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
}

bool XMLParser::parse(const std::string& filename,
                      MeetingPlanner& planner) const {
    REQUIRE(!filename.empty(), "Bestandsnaam mag niet leeg zijn");

    if (loggingEnabled) {
        return parse(filename.c_str(), std::cerr, planner) != ImportAborted;
    }

    std::ostringstream ignoredErrors;
    return parse(filename.c_str(), ignoredErrors, planner) != ImportAborted;
}

SuccessEnum XMLParser::parse(const char* inputFilename,
                             std::ostream& errStream,
                             MeetingPlanner& planner) const {
    REQUIRE(inputFilename != nullptr,
            "Bestandsnaam mag niet null zijn");

    REQUIRE(inputFilename[0] != '\0',
            "Bestandsnaam mag niet leeg zijn");

    TiXmlDocument doc(inputFilename);
    SuccessEnum endResult = Success;

    if (!doc.LoadFile()) {
        errStream << "XML IMPORT ABORTED: "
                  << doc.ErrorDesc()
                  << std::endl;

        return ImportAborted;
    }

    bool foundRootElement = false;

    for (TiXmlElement* root = doc.FirstChildElement();
         root != nullptr;
         root = root->NextSiblingElement()) {
        foundRootElement = true;

        const std::string rootName = root->Value();

        if (rootName != "SYSTEM") {
            errStream << "XML PARTIAL IMPORT: Expected <SYSTEM> ... </SYSTEM> and got <"
                      << rootName
                      << "> ... </"
                      << rootName
                      << ">."
                      << std::endl;

            endResult = PartialImport;
            continue;
        }

        for (TiXmlElement* elem = root->FirstChildElement();
             elem != nullptr;
             elem = elem->NextSiblingElement()) {
            const std::string elementName = elem->Value();

            if (elementName == "CAMPUS") {
                std::string name;
                std::string identifier;

                if (!readRequiredText(elem, "NAME", "CAMPUS", name, errStream) ||
                    !readRequiredText(elem, "IDENTIFIER", "CAMPUS", identifier, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                try {
                    planner.addCampus(Campus(name, identifier));
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import CAMPUS '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "BUILDING") {
                std::string name;
                std::string identifier;
                std::string campusIdentifier;

                if (!readRequiredText(elem, "NAME", "BUILDING", name, errStream) ||
                    !readRequiredText(elem, "IDENTIFIER", "BUILDING", identifier, errStream) ||
                    !readRequiredText(elem, "CAMPUS", "BUILDING", campusIdentifier, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                try {
                    planner.addBuilding(
                            Building(name, identifier, campusIdentifier));
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import BUILDING '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "CATERING") {
                std::string campusIdentifier;
                std::string co2Text;

                if (!readRequiredText(elem, "CAMPUS", "CATERING",
                                      campusIdentifier, errStream) ||
                    !readRequiredText(elem, "CO2", "CATERING",
                                      co2Text, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                float co2 = 0.0f;

                if (!parsePositiveFloat(co2Text, co2)) {
                    errStream << "XML PARTIAL IMPORT: Illegal CO2 value '"
                              << co2Text
                              << "' in <CATERING>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                try {
                    planner.addCateringProvider(
                            CateringProvider(campusIdentifier, co2));
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import CATERING for campus '"
                              << campusIdentifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "ROOM") {
                std::string name;
                std::string identifier;
                std::string capacityText;
                std::string campusIdentifier;
                std::string buildingIdentifier;

                if (!readRequiredText(elem, "NAME", "ROOM", name, errStream) ||
                    !readRequiredText(elem, "IDENTIFIER", "ROOM", identifier, errStream) ||
                    !readRequiredText(elem, "CAPACITY", "ROOM", capacityText, errStream) ||
                    !readRequiredText(elem, "CAMPUS", "ROOM", campusIdentifier, errStream) ||
                    !readRequiredText(elem, "BUILDING", "ROOM", buildingIdentifier, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                int capacity = 0;

                if (!parsePositiveInt(capacityText, capacity)) {
                    errStream << "XML PARTIAL IMPORT: Illegal CAPACITY value '"
                              << capacityText
                              << "' in <ROOM>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                try {
                    planner.addRoom(Room(name,
                                         identifier,
                                         capacity,
                                         campusIdentifier,
                                         buildingIdentifier));
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import ROOM '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "RENOVATION") {
                std::string roomIdentifier;
                std::string startDate;
                std::string endDate;

                if (!readRequiredText(elem, "ROOM", "RENOVATION",
                                      roomIdentifier, errStream) ||
                    !readRequiredText(elem, "START", "RENOVATION",
                                      startDate, errStream) ||
                    !readRequiredText(elem, "END", "RENOVATION",
                                      endDate, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                if (!isValidDateFormat(startDate) ||
                    !isValidDateFormat(endDate)) {
                    errStream << "XML PARTIAL IMPORT: Illegal START or END date in <RENOVATION>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                if (startDate > endDate) {
                    errStream << "XML PARTIAL IMPORT: START date must not be after END date in <RENOVATION>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                if (!roomExistsInPlanner(planner, roomIdentifier)) {
                    errStream << "XML PARTIAL IMPORT: Unknown ROOM '"
                              << roomIdentifier
                              << "' in <RENOVATION>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                try {
                    planner.addRenovation(
                            Renovation(roomIdentifier, startDate, endDate));
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import RENOVATION for room '"
                              << roomIdentifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "MEETING") {
                std::string label;
                std::string identifier;
                std::string date;
                std::string onlineText;

                if (!readRequiredText(elem, "LABEL", "MEETING", label, errStream) ||
                    !readRequiredText(elem, "IDENTIFIER", "MEETING", identifier, errStream) ||
                    !readRequiredText(elem, "DATE", "MEETING", date, errStream) ||
                    !readRequiredText(elem, "ONLINE", "MEETING", onlineText, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                if (!isValidDateFormat(date)) {
                    errStream << "XML PARTIAL IMPORT: Illegal DATE value '"
                              << date
                              << "' in MEETING '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                bool online = false;

                if (!parseBooleanText(onlineText, online)) {
                    errStream << "XML PARTIAL IMPORT: Illegal ONLINE value '"
                              << onlineText
                              << "' in MEETING '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                bool catering = false;
                TiXmlElement* cateringElem =
                        elem->FirstChildElement("CATERING");

                if (cateringElem != nullptr) {
                    const std::string cateringText =
                            trim(cateringElem->GetText()
                                         ? cateringElem->GetText()
                                         : "");

                    if (!parseBooleanText(cateringText, catering)) {
                        errStream << "XML PARTIAL IMPORT: Illegal CATERING value '"
                                  << cateringText
                                  << "' in MEETING '"
                                  << identifier
                                  << "'."
                                  << std::endl;

                        endResult = PartialImport;
                        continue;
                    }
                }

                bool externalsAllowed = false;
                TiXmlElement* externalsElem =
                        elem->FirstChildElement("EXTERNALS");

                if (externalsElem != nullptr) {
                    const std::string externalsText =
                            trim(externalsElem->GetText()
                                         ? externalsElem->GetText()
                                         : "");

                    if (!parseBooleanText(externalsText,
                                          externalsAllowed)) {
                        errStream << "XML PARTIAL IMPORT: Illegal EXTERNALS value '"
                                  << externalsText
                                  << "' in MEETING '"
                                  << identifier
                                  << "'."
                                  << std::endl;

                        endResult = PartialImport;
                        continue;
                    }
                }

                const std::string roomIdentifier =
                        readOptionalText(elem, "ROOM");

                if (online && catering) {
                    errStream << "XML PARTIAL IMPORT: Online MEETING '"
                              << identifier
                              << "' cannot request catering."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                if (!online && roomIdentifier.empty()) {
                    errStream << "XML PARTIAL IMPORT: Expected non-empty <ROOM> ... </ROOM> in physical MEETING '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                try {
                    Meeting meeting(label, identifier, roomIdentifier, date);

                    meeting.setOnline(online);
                    meeting.setCatering(catering);
                    meeting.setExternalsAllowed(externalsAllowed);

                    planner.addMeeting(meeting);
                } catch (...) {
                    errStream << "XML PARTIAL IMPORT: Could not import MEETING '"
                              << identifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else if (elementName == "PARTICIPATION") {
                std::string meetingIdentifier;
                std::string user;

                if (!readRequiredText(elem, "MEETING", "PARTICIPATION",
                                      meetingIdentifier, errStream) ||
                    !readRequiredText(elem, "USER", "PARTICIPATION",
                                      user, errStream)) {
                    endResult = PartialImport;
                    continue;
                }

                bool external = false;
                TiXmlElement* externalElem =
                        elem->FirstChildElement("EXTERNAL");

                if (externalElem != nullptr) {
                    const std::string externalText =
                            trim(externalElem->GetText()
                                         ? externalElem->GetText()
                                         : "");

                    if (!parseBooleanText(externalText, external)) {
                        errStream << "XML PARTIAL IMPORT: Illegal EXTERNAL value '"
                                  << externalText
                                  << "' in <PARTICIPATION>."
                                  << std::endl;

                        endResult = PartialImport;
                        continue;
                    }
                }

                const Meeting* meeting =
                        findMeetingInPlanner(planner, meetingIdentifier);

                if (meeting == nullptr) {
                    errStream << "XML PARTIAL IMPORT: Unknown MEETING '"
                              << meetingIdentifier
                              << "' in <PARTICIPATION>."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                if (external && !meeting->areExternalsAllowed()) {
                    errStream << "XML PARTIAL IMPORT: External participant '"
                              << user
                              << "' is not allowed in MEETING '"
                              << meetingIdentifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                    continue;
                }

                if (!planner.addParticipation(meetingIdentifier,
                                               user,
                                               external)) {
                    errStream << "XML PARTIAL IMPORT: Could not add participant '"
                              << user
                              << "' to MEETING '"
                              << meetingIdentifier
                              << "'."
                              << std::endl;

                    endResult = PartialImport;
                }
            }

            else {
                errStream << "XML PARTIAL IMPORT: Unexpected <"
                          << elementName
                          << "> ... </"
                          << elementName
                          << "> in <SYSTEM>."
                          << std::endl;

                endResult = PartialImport;
            }
        }
    }

    if (!foundRootElement) {
        errStream << "XML PARTIAL IMPORT: Expected <SYSTEM> ... </SYSTEM>."
                  << std::endl;

        return PartialImport;
    }

    return endResult;
}