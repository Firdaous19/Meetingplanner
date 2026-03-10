#include <iostream>
#include "tinyxml/tinyxml.h"
int main() {
    TiXmlDocument doc;
    if (!doc.LoadFile("een CD.xml")) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return 1;
    }
    TiXmlElement *root = doc.FirstChildElement();
    if (root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return 1;
    }
    for (TiXmlElement *elem = root->FirstChildElement();
         elem != NULL;
         elem = elem->NextSiblingElement()) {

        std::string name = elem->Value();

        TiXmlNode *child = elem->FirstChild();
        if (!child) continue;

        TiXmlText *text = child->ToText();
        if (!text) continue;

        std::string value = text->Value();

        if (name == "ARTIST" || name == "Artist")
            std::cout << "Artist: " << value << std::endl;

        if (name == "TITLE" || name == "Title")
            std::cout << "Title: " << value << std::endl;


    }

    doc.Clear();
    return 0;
}
