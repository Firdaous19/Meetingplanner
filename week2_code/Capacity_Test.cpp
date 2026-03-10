#include <gtest/gtest.h>
#include "week2_code/Room.h"

TEST(RoomTest, ThrowExceptionWhenAddingToFullRoom) {
    Room kamer("TestKamer", 1);

    kamer.addPerson("Gebruiker 1");

    EXPECT_THROW(kamer.addPerson("Gebruiker 2"), std::runtime_error);
}