cmake_minimum_required(VERSION 2.8.12)
project(JsonFetcher)

find_package(Qt4 REQUIRED QtCore QtNetwork)
include(${QT_USE_FILE})

# CRITICAL: Flags for old ARMv5 kernel
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv5te -msoft-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv5te -msoft-float")

set(SOURCES main.cpp)

qt4_automoc(${SOURCES})

add_executable(JsonFetcher ${SOURCES})

target_link_libraries(JsonFetcher ${QT_LIBRARIES})