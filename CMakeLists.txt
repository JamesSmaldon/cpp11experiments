cmake_minimum_required (VERSION 2.6)
project (cpp11experiments)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable (cpp11experiments ${SOURCES} ${HEADERS})
