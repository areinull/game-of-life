cmake_minimum_required(VERSION 3.5)
project(Life)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(Life ${SOURCE_FILES})
target_link_libraries(Life ${CURSES_LIBRARIES} pthread)
