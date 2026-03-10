#ifndef PROJECTTITLE_ROOM_H
#define PROJECTTITLE_ROOM_H

#include <string>
#include <vector>

class Room {
public:
    Room(const std::string& name, int capacity);
    std::string getName() const { return name; }
    int getCapacity() const { return capacity; }
    void addPerson(std::string name);

private:
    std::string name;
    int capacity;
    std::vector<std::string> persons;
};

#endif //PROJECTTITLE_ROOM_H