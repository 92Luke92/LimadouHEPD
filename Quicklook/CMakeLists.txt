# CMakeLists.txt for Tracker Quicklook
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TrackerQuicklook)


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

# EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
# EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
# set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )
find_package( ROOT REQUIRED COMPONENTS RIO Net Tree Hist Gpad )
include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(QuickLook QuickLook.cc ${sources})
#target_link_libraries(TrackerQuickLook ${ROOT_LD_FLAGS})
target_link_libraries(QuickLook ${ROOT_LIBRARIES})

message(${ROOT_LIBRARIES})

