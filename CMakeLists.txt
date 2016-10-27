cmake_minimum_required(VERSION 3.5)
project(openMPI)

set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")

add_subdirectory(Seminar1)
add_subdirectory(Seminar2)
add_subdirectory(Trying)
