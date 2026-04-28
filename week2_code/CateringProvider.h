//
// Created by firdi on 28/04/2026.
//

#ifndef PROJECTTITLE_CATERINGPROVIDER_H
#define PROJECTTITLE_CATERINGPROVIDER_H
#include <string>

class CateringProvider {
public:
    CateringProvider(const std::string& campusIdentifier,
                     int co2);

    std::string getCampusIdentifier() const { return campusIdentifier; }
    int getCO2() const { return co2; }

private:
    std::string campusIdentifier;
    int co2;
};
#endif //PROJECTTITLE_CATERINGPROVIDER_H
