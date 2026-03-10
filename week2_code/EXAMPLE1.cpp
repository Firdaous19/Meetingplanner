#include <iostream>
#include "tinyxml/tinyxml.h"
#include "Room.h"

int main() {
    TiXmlDocument doc("kamers.xml");

    if (!doc.LoadFile()) {
        std::cerr << "FOUT: Kon kamers.xml niet laden! Controleer of het bestand in de juiste map staat." << std::endl;
        return 1;
    }

    TiXmlElement* root = doc.FirstChildElement("Kamers");
    if (!root) {
        std::cerr << "FOUT: Geen <Kamers> element gevonden in de XML." << std::endl;
        return 1;
    }

    std::cout << "Data inlezen vanuit XML:" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (TiXmlElement* elem = root->FirstChildElement("Kamer"); elem != nullptr; elem = elem->NextSiblingElement("Kamer")) {

        const char* naamAttr = elem->Attribute("naam");
        int capaciteit = 0;
        elem->QueryIntAttribute("capaciteit", &capaciteit);

        if (naamAttr) {
            Room r(naamAttr, capaciteit);
            std::cout << "Kamer ingeladen: " << r.getName()
                      << " (Capaciteit: " << r.getCapacity() << ")" << std::endl;
        }
    }

    std::cout << "------------------------------------------" << std::endl;
    return 0;
}