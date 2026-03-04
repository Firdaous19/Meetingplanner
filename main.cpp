#include <iostream>
#include "tinyxml/tinyxml.h"
#include "week2_code/Room.h"

int main() {
    TiXmlDocument doc("een CD.xml");
    if (!doc.LoadFile()) {
        std::cerr << "Fout bij laden: " << doc.ErrorDesc() << std::endl;
        return 1;
    }
    TiXmlElement *root = doc.FirstChildElement("CD");
    if (!root) {
        std::cerr << "Geen <CD> tag gevonden!" << std::endl;
        return 1;
    }
    for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

        std::string name = elem->Value();
        const char* text = elem->GetText();

        if (text != NULL) {
            std::cout << name << ": " << text << std::endl;
        }
    }

    doc.Clear();
    return 0;
}
