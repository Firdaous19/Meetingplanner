//
// Created by firdi on 28/04/2026.
//

#ifndef PROJECTTITLE_CATERINGPROVIDER_H
#define PROJECTTITLE_CATERINGPROVIDER_H

#include <string>

/**
 * Stelt een cateringprovider voor gekoppeld aan een campus.
 * Elke provider heeft een campusidentifier en een CO2-uitstootwaarde.
 */
class CateringProvider {
public:
    /**
     * Constructor van een cateringprovider.
     * @param campusIdentifier De identifier van de campus waarvoor catering beschikbaar is.
     * @param co2 De CO2-uitstoot van deze provider.
     */
    CateringProvider(const std::string& campusIdentifier,
                     float co2);

    /**
     * Geef de campus identifier van de provider terug.
     * @return Identifier van de campus.
     */
    std::string getCampusIdentifier() const { return campusIdentifier; }

    /**
     * Geef de CO2-uitstoot van deze provider terug.
     * @return CO2-uitstoot in gram.
     */
    float getCO2() const { return co2; }

private:
    std::string campusIdentifier;
    float co2;
};

#endif // PROJECTTITLE_CATERINGPROVIDER_H