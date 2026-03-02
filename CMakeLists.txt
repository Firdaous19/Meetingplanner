cmake_minimum_required(VERSION 3.10)
project(ProjectTitle)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

# Flags (laat -Werror staan als de prof dat wil)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Zorg dat headers in de root gevonden worden (DesignByContract.h)
include_directories(${CMAKE_SOURCE_DIR})

# --- GoogleTest: cross-platform (Windows/Mac/Linux) ---
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Windows runtime fix (veilig om overal te laten staan)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# --- Sources ---
set(RELEASE_SOURCE_FILES
        ./week2_code/Fighterplane.cpp
        ./week2_code/Largest.cpp
        ./week2_code/main.cpp
)

set(DEBUG_SOURCE_FILES
        ./week2_code/Fighterplane.cpp
        ./week2_code/Largest.cpp
        ./week2_code/tests.cpp
)

add_executable(release_target ${RELEASE_SOURCE_FILES})
add_executable(debug_target ${DEBUG_SOURCE_FILES})

# Link gtest to debug target
target_link_libraries(debug_target PRIVATE gtest gtest_main)