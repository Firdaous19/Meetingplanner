//
// Created by firdi on 27/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/Campus.h"

TEST(CampusTest, ConstructorStoresValuesCorrectly) {
    Campus campus("Campus Drie Eiken", "CDE");

    EXPECT_EQ(campus.getName(), "Campus Drie Eiken");
    EXPECT_EQ(campus.getIdentifier(), "CDE");
}

TEST(CampusTest, EmptyNameIsRejected) {
    EXPECT_DEATH(
            Campus("", "CDE"),
            "Campus name mag niet leeg zijn"
    );
}

TEST(CampusTest, EmptyIdentifierIsRejected) {
    EXPECT_DEATH(
            Campus("Campus Drie Eiken", ""),
            "Campus identifier mag niet leeg zijn"
    );
}