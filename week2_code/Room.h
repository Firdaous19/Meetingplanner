#ifndef PROJECTTITLE_ROOM_H
#define PROJECTTITLE_ROOM_H

#include <string>
#include <vector>

class Room {
public:
    Room(const std::string& name, const std::string& identifier, int capacity);

    std::string getName() const { return name; }
    std::string getIdentifier() const { return identifier; }
    int getCapacity() const { return capacity; }

    void addPerson(const std::string& personName);
    int getNumberOfPersons() const { return persons.size(); }

private:
    std::string name;
    std::string identifier;
    int capacity;
    std::vector<std::string> persons;
};

#endif