#ifndef PROJECTTITLE_ROOM_H
#define PROJECTTITLE_ROOM_H

#include <string>

class Room {
public:
    Room(const std::string& name, int capacity);
    std::string getName() const { return name; }
    int getCapacity() const { return capacity; }

private:
    std::string name;
    int capacity;
};

#endif //PROJECTTITLE_ROOM_H