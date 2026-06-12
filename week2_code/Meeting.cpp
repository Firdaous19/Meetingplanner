#include "Meeting.h"
#include "DesignByContract.h"
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
          co2Emission(0.0f),
          occupancyPercentage(0),
          cateringCost(0.0f) {
    REQUIRE(!label.empty(), "Meeting label mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!date.empty(), "Meeting date mag niet leeg zijn");
    REQUIRE(isValidDateFormat(date), "Meeting date moet formaat YYYY-MM-DD hebben");

    ENSURE(this->label == label, "Meeting label correct opgeslagen");
    ENSURE(this->identifier == identifier, "Meeting identifier correct opgeslagen");
    ENSURE(this->roomIdentifier == roomIdentifier, "Meeting room identifier correct opgeslagen");
    ENSURE(this->date == date, "Meeting date correct opgeslagen");
    ENSURE(!online, "Nieuwe meeting is standaard niet online");
    ENSURE(!catering, "Nieuwe meeting heeft standaard geen catering");
    ENSURE(!(online && catering),
           "Nieuwe meeting mag niet tegelijk online en catering hebben");
    ENSURE(this->co2Emission == 0.0f,
           "Nieuwe meeting heeft standaard 0 CO2-uitstoot");
    ENSURE(this->occupancyPercentage == 0,
           "Nieuwe meeting heeft standaard 0% occupancy");
    ENSURE(this->cateringCost == 0.0f,
           "Nieuwe meeting heeft standaard 0 cateringkost");
}

void Meeting::addParticipant(const std::string& user) {
    addParticipant(user, false);
}

void Meeting::setExternalsAllowed(bool value) {
    externalsAllowed = value;

    ENSURE(externalsAllowed == value, "Externals allowed flag correct opgeslagen");
}

void Meeting::setCatering(bool value) {
    REQUIRE(!(online && value),
            "Online meeting mag geen catering hebben");

    catering = value;

    ENSURE(catering == value, "Catering flag correct opgeslagen");
    ENSURE(!(online && catering),
           "Meeting mag niet tegelijk online en catering hebben");
}

void Meeting::setOnline(bool value) {
    REQUIRE(!(value && catering),
            "Online meeting mag geen catering hebben");

    online = value;

    ENSURE(online == value, "Online flag correct opgeslagen");
    ENSURE(!(online && catering),
           "Meeting mag niet tegelijk online en catering hebben");
}

void Meeting::setCO2Emission(float value) {
    REQUIRE(value >= 0, "CO2 emission mag niet negatief zijn");

    co2Emission = value;

    ENSURE(co2Emission == value, "CO2 emission correct opgeslagen");
}

void Meeting::setOccupancyPercentage(int percentage) {
    REQUIRE(percentage >= 0 && percentage <= 100,
            "Occupancy percentage moet tussen 0 en 100 liggen");

    occupancyPercentage = percentage;

    ENSURE(occupancyPercentage == percentage,
           "Occupancy percentage correct opgeslagen");
}

void Meeting::setCateringCost(float value) {
    REQUIRE(value >= 0, "Catering cost mag niet negatief zijn");

    cateringCost = value;

    ENSURE(cateringCost == value, "Catering cost correct opgeslagen");
}
void Meeting::addParticipant(const std::string& user, bool external) {
    REQUIRE(!user.empty(), "Participant name mag niet leeg zijn");

    size_t oldSize = participants.size();

    participants.push_back(user);
    externalParticipants.push_back(external);

    ENSURE(participants.size() == oldSize + 1,
           "Participant moet toegevoegd zijn");
    ENSURE(externalParticipants.size() == participants.size(),
           "Voor elke participant moet een external flag bestaan");
    ENSURE(externalParticipants.back() == external,
           "External participant flag moet correct opgeslagen zijn");
}
int Meeting::getExternalParticipantCount() const {
    int count = 0;

    for (bool external : externalParticipants) {
        if (external) {
            count++;
        }
    }

    return count;
}

int Meeting::getInternalParticipantCount() const {
    int count = 0;

    for (bool external : externalParticipants) {
        if (!external) {
            count++;
        }
    }

    return count;
}

bool Meeting::hasExternalParticipants() const {
    return getExternalParticipantCount() > 0;
}

bool Meeting::hasInternalParticipants() const {
    return getInternalParticipantCount() > 0;
}