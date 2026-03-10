cmake_minimum_required(VERSION 3.10)
project(Meetingplanner)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(RELEASE_SOURCE_FILES
        tinyxml/tinystr.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        week2_code/Room.cpp
        week2_code/EXAMPLE1.cpp
)

set(DEBUG_SOURCE_FILES
        week2_code/Room.cpp
        week2_code/EXAMPLE_TEST1.cpp
        week2_code/tests.cpp
        week2_code/Capacity_Test.cpp
        week2_code/Fighterplane.cpp
        week2_code/Largest.cpp
)

add_executable(release_target ${RELEASE_SOURCE_FILES})
add_executable(debug_target ${DEBUG_SOURCE_FILES})

target_link_libraries(debug_target PRIVATE gtest gtest_main pthread)