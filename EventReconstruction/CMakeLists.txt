cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#include(${CMAKE_CURRENT_SOURCE_DIR}/Tracker/CMakeLists.txt)
add_subdirectory(Common)
add_subdirectory(Tracker)
add_subdirectory(Calo)
add_subdirectory(exe)

add_definitions( -std=c++11)


#other cmakelists to be added here when written
