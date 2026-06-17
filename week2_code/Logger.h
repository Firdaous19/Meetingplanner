#ifndef PROJECTTITLE_LOGGER_H
#define PROJECTTITLE_LOGGER_H

#include <string>

class Logger {
public:
    explicit Logger(bool enabled = true);

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void info(const std::string& message) const;
    void error(const std::string& message) const;

private:
    bool enabled;
};

#endif