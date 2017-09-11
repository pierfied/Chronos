cmake_minimum_required(VERSION 3.8)
project(Chronos)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES src/*.h src/*.c)
add_library(chronos SHARED ${SOURCES})