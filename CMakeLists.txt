cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#include(${CMAKE_CURRENT_SOURCE_DIR}/Tracker/CMakeLists.txt)
add_subdirectory(Tracker)
add_subdirectory(Calo)
add_subdirectoy(Common)
#other cmakelists to be added here when written
