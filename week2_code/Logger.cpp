//
// Created by firdi on 17/06/2026.
//
#include "Logger.h"
#include <iostream>

Logger::Logger(bool enabled)
        : enabled(enabled) {
}

void Logger::setEnabled(bool value) {
    enabled = value;
}

bool Logger::isEnabled() const {
    return enabled;
}

void Logger::info(const std::string& message) const {
    if (enabled) {
        std::cout << message << std::endl;
    }
}

void Logger::error(const std::string& message) const {
    if (enabled) {
        std::cerr << message << std::endl;
    }
}