# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Tracker)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})


find_package( ROOT REQUIRED COMPONENTS RIO Net Tree Hist )
include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(CalibrateTracker CalibrateTracker.cc ${sources})

target_link_libraries(CalibrateTracker ${ROOT_LIBRARIES})


