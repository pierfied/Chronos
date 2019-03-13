cmake_minimum_required(VERSION 2.8)
project(Chronos)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

FIND_PACKAGE(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

file(GLOB SOURCES src/*.h src/*.c)
add_library(chronos_shared SHARED ${SOURCES})
add_library(chronos_static STATIC ${SOURCES})

set_target_properties(chronos_shared PROPERTIES OUTPUT_NAME "chronos")
set_target_properties(chronos_static PROPERTIES OUTPUT_NAME "chronos")

install(TARGETS chronos_shared chronos_static DESTINATION lib)
file(GLOB HEADERS src/*.h)
install(FILES ${HEADERS} DESTINATION include)