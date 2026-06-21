//
// Created by firdi on 17/06/2026.
//
#include "Logger.h"
#include <iostream>
#include "DesignByContract.h"

Logger::Logger(bool enabled)
        : enabled(enabled) {

    ENSURE(this->enabled == enabled,
           "Logger status correct opgeslagen");
}

void Logger::setEnabled(bool value) {
    enabled = value;

    ENSURE(enabled == value,
           "Logger status correct opgeslagen");
}

bool Logger::isEnabled() const {
    return enabled;
}

void Logger::info(const std::string& message) const {
    REQUIRE(!message.empty(),
            "Logbericht mag niet leeg zijn");
    if (enabled) {
        std::cout << message << std::endl;
    }
}

void Logger::error(const std::string& message) const {
    REQUIRE(!message.empty(),
            "Logbericht mag niet leeg zijn");
    if (enabled) {
        std::cerr << message << std::endl;
    }
}