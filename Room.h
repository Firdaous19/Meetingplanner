#ifndef ROOM_H
#define ROOM_H

#include <string>
#include "DesignByContract.h"

class Room {
public:
    Room(const std::string& name, int capacity);
    std::string getName() const { return name; }
    int getCapacity() const { return name; }

private:
    std::string name;
    int capacity;
};

#endif