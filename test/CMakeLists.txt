cmake_minimum_required(VERSION 3.8)
project(Chronos)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB SOURCES */*.h */*.c)
add_library(tests SHARED ${SOURCES})

file(GLOB CHRONOS_LIB ../libchronos.so)
target_link_libraries(tests ${CHRONOS_LIB})