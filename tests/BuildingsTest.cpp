//
// Created by ali_s on 26/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/Building.h"

TEST(BuildingTest, ConstructorStoresValuesCorrectly) {
    Building building("Gebouw R", "CDE_R", "Campus_CDE");

    EXPECT_EQ(building.getName(), "Gebouw R");
    EXPECT_EQ(building.getIdentifier(), "CDE_R");
    EXPECT_EQ(building.getCampusIdentifier(), "Campus_CDE");
}

TEST(BuildingTest, EmptyNameIsRejected) {
    EXPECT_DEATH(
            Building("", "CDE_R", "Campus_CDE"),
            "Building name mag niet leeg zijn"
    );
}

TEST(BuildingTest, EmptyIdentifierIsRejected) {
    EXPECT_DEATH(
            Building("Gebouw R", "", "Campus_CDE"),
            "Building identifier mag niet leeg zijn"
    );
}

TEST(BuildingTest, EmptyCampusIdentifierIsRejected) {
    EXPECT_DEATH(
            Building("Gebouw R", "CDE_R", ""),
            "Campus identifier mag niet leeg zijn"
    );
}