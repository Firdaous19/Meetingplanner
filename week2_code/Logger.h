#ifndef PROJECTTITLE_LOGGER_H
#define PROJECTTITLE_LOGGER_H

#include <string>

class Logger {
public:
    /**
 * Maak een nieuwe logger.
 * @param enabled true als logging actief moet zijn.
 *
 * ENSURE(isEnabled() == enabled,
 *        "Logger status correct opgeslagen");
 */
    explicit Logger(bool enabled = true);
/**
 * Zet logging aan of uit.
 * @param enabled Nieuwe loggingstatus.
 *
 * ENSURE(isEnabled() == enabled,
 *        "Logger status correct opgeslagen");
 */
    void setEnabled(bool enabled);
    /**
 * Controleer of logging actief is.
 * @return true als logging actief is.
 */
    bool isEnabled() const;
/**
 * Schrijf een informatief bericht.
 * @param message Bericht dat gelogd wordt.
 *
 * REQUIRE(!message.empty(), "Logbericht mag niet leeg zijn");
 */
    void info(const std::string& message) const;
/**
 * Schrijf een foutbericht.
 * @param message Foutbericht dat gelogd wordt.
 *
 * REQUIRE(!message.empty(), "Logbericht mag niet leeg zijn");
 */
    void error(const std::string& message) const;

private:
    bool enabled;
};

#endif