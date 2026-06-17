cmake_minimum_required(VERSION 3.10)
project(ProjectTitle)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

include_directories(${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(RELEASE_SOURCE_FILES
        week2_code/main.cpp
        week2_code/Room.cpp
        week2_code/Meeting.cpp
        week2_code/Building.cpp
        week2_code/MeetingPlanner.cpp
        week2_code/XMLParser.cpp
        week2_code/Campus.cpp
        week2_code/OutputWriter.cpp
        week2_code/Renovation.cpp
        week2_code/CateringProvider.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinystr.cpp
)

set(DEBUG_SOURCE_FILES
        week2_code/Room.cpp
        week2_code/Meeting.cpp
        week2_code/MeetingPlanner.cpp
        week2_code/XMLParser.cpp
        week2_code/OutputWriter.cpp
        week2_code/Building.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinystr.cpp
        week2_code/OutputWriter.cpp
        week2_code/Campus.cpp
        week2_code/Renovation.cpp
        tests/RenovationTests.cpp
        tests/CampusTests.cpp
        tests/RoomTests.cpp
        tests/MeetingTests.cpp
        tests/MeetingPlannerTests.cpp
        tests/XMLParserTests.cpp
        tests/OutputWriterTests.cpp
        tests/BuildingsTest.cpp
        tests/CampusTests.cpp
        week2_code/CateringProvider.cpp
        tests/CateringProviderTests.cpp
        tests/XMLParserTests.cpp
        tests/XMLParserTests.cpp
        week2_code/Logger.h
        week2_code/Logger.cpp

)

add_executable(release_target ${RELEASE_SOURCE_FILES})
add_executable(debug_target ${DEBUG_SOURCE_FILES})

target_link_libraries(debug_target GTest::gtest_main)