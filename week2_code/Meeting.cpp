#include "Meeting.h"
#include "DesignByContract.h"

Meeting::Meeting(const std::string& label,
                 const std::string& identifier,
                 const std::string& roomIdentifier,
                 const std::string& date)
        : label(label), identifier(identifier), roomIdentifier(roomIdentifier), date(date) {
    REQUIRE(!label.empty(), "Meeting label mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Meeting identifier mag niet leeg zijn");
    REQUIRE(!roomIdentifier.empty(), "Meeting room identifier mag niet leeg zijn");
    REQUIRE(!date.empty(), "Meeting date mag niet leeg zijn");

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