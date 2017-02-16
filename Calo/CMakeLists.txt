# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(event)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT

        EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
        EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})

message (${CMAKE_EXE_LINKER_FLAGS})



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#ROOT_GENERATE_DICTIONARY(G__Event Event.h LINKDEF EventLinkDef.h)
#to take into account also the objects inside the src folder
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

#---Create a shared library with geneated dictionary
#add_library(Event SHARED ${SOURCES})
#target_link_libraries(Event ${ROOT_LIBRARIES})



#---Create  a main program using the library

add_definitions( -std=c++11)

add_executable(main.cc ${sources})

target_link_libraries(main ${ROOT_LIBRARIES})




#target_link_libraries(Main Event)
