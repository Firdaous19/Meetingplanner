//
// Created by firdi on 11/03/2026.
//
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
}

void Meeting::addParticipant(const std::string& user) {
    REQUIRE(!user.empty(), "Participant name mag niet leeg zijn");
    participants.push_back(user);
}