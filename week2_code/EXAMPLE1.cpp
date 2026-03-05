#include <iostream>
#include "Room.h"

void runEXAMPLE1() {
    Room meetingRoom("Vergaderzaal A", 10);
    std::cout << "Kamer aangemaakt: " << meetingRoom.getName() << std::endl;

    std::cout << "Proberen een kamer met capaciteit 0 aan te maken..." << std::endl;
    Room invalidRoom("Foutieve kamer", 0);
}