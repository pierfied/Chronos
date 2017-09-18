cmake_minimum_required(VERSION 3.8)
project(Chronos)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB SOURCES */*.h */*.c)
add_library(tests SHARED ${SOURCES})

file(GLOB CHRONOS_LIB ../libchronos.so)
target_link_libraries(tests ${CHRONOS_LIB})