#include "gtest/gtest.h"
#include "week2_code/Room.h"

TEST(RoomTest, ConstructorStoresValuesCorrectly) {
    Room room("M.G.023", "Room123", 10);

    EXPECT_EQ(room.getName(), "M.G.023");
    EXPECT_EQ(room.getIdentifier(), "Room123");
    EXPECT_EQ(room.getCapacity(), 10);
    EXPECT_FALSE(room.isOccupied());
}

TEST(RoomTest, OccupySetsRoomToOccupied) {
    Room room("M.G.023", "Room123", 10);

    room.occupy();

    EXPECT_TRUE(room.isOccupied());
}

TEST(RoomTest, AddPersonIncreasesNumberOfPersons) {
    Room room("M.G.023", "Room123", 10);

    room.addPerson("Alice");

    EXPECT_EQ(room.getNumberOfPersons(), 1);
}

TEST(RoomTest, EmptyNameIsRejected) {
    EXPECT_DEATH(
            Room("", "Room123", 10),
            "Room name mag niet leeg zijn"
    );
}

TEST(RoomTest, EmptyIdentifierIsRejected) {
    EXPECT_DEATH(
            Room("M.G.023", "", 10),
            "Room identifier mag niet leeg zijn"
    );
}

TEST(RoomTest, ZeroCapacityIsRejected) {
    EXPECT_DEATH(
            Room("M.G.023", "Room123", 0),
            "Capaciteit moet groter zijn dan 0"
    );
}

TEST(RoomTest, NegativeCapacityIsRejected) {
    EXPECT_DEATH(
            Room("M.G.023", "Room123", -5),
            "Capaciteit moet groter zijn dan 0"
    );
}
TEST(RoomTest, AddingMultiplePersonsTracksCountCorrectly) {
    Room room("Vergaderzaal A", "A101", 5);

    room.addPerson("Alice");
    room.addPerson("Bob");
    room.addPerson("Charlie");

    EXPECT_EQ(room.getNumberOfPersons(), 3);
}
TEST(RoomTest, EmptyPersonNameIsRejected) {
    Room room("Vergaderzaal A", "A101", 5);

    EXPECT_DEATH(
            room.addPerson(""),
            "Persoonsnaam mag niet leeg zijn"
    );
}
TEST(RoomTest, AddingPersonWhenRoomIsFullIsRejected) {
    Room room("Vergaderzaal A", "A101", 1);

    room.addPerson("Alice");

    EXPECT_DEATH(
            room.addPerson("Bob"),
            "Kamer is al vol"
    );
}