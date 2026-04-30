#include "Meeting.h"
#include <cctype>

/**
 * Controleer of een string een geldige datumstructuur heeft in formaat YYYY-MM-DD.
 */
static bool isValidDateFormat(const std::string& date) {
    if (date.size() != 10) {
        return false;
    }

    if (date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    return true;
}

Meeting::Meeting(const std::string& label,
                 const std::string& identifier,
                 const std::string& roomIdentifier,
                 const std::string& date)
        : label(label),
          identifier(identifier),
          roomIdentifier(roomIdentifier),
          date(date),
          occupancyPercentage(0) {
    REQUIRE(!label.empty(), "Meeting label mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!roomIdentifier.empty(), "Meeting room identifier mag niet leeg zijn");
    REQUIRE(!date.empty(), "Meeting date mag niet leeg zijn");
    REQUIRE(isValidDateFormat(date), "Meeting date moet formaat YYYY-MM-DD hebben");

    ENSURE(this->label == label, "Meeting label correct opgeslagen");
    ENSURE(this->identifier == identifier, "Meeting identifier correct opgeslagen");
    ENSURE(this->roomIdentifier == roomIdentifier, "Meeting room identifier correct opgeslagen");
    ENSURE(this->date == date, "Meeting date correct opgeslagen");
}

void Meeting::addParticipant(const std::string& user) {
    REQUIRE(!user.empty(), "Participant name mag niet leeg zijn");

    size_t oldSize = participants.size();
    participants.push_back(user);

    ENSURE(participants.size() == oldSize + 1, "Participant moet toegevoegd zijn");
}