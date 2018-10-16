##=============================================================================
##
## HEPDOrbital Analysis Software CMake Build System
##
## Details: This produces the event package, a library dictionary
## and a main program
##
##=============================================================================

project(event CXX)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# set paths
set(common_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(common_SOURCE_DIR ${common_DIR}/src)
set(common_INCLUDE_DIR ${common_DIR}/include)

set(common_INSTALL_INCLUDE_DIR ${HEPDOrbital_INSTALL_INCLUDE_DIR}/lib/include
    CACHE STRING "event header installation directory")

# set library names
set(common_LIBRARY_SHARED HEPDOrbitalCommon CACHE STRING "HEPDOrbitalCommon")
set(common_DICTIONARY G__HEPDOrbitalCommon CACHE STRING "G__HEPDOrbitalCommon")

# include cern root cmake macros
include(${ROOT_USE_FILE})

# compiler include directories
include_directories(
    ${ROOT_INCLUDE_DIRS}
    ${common_INCLUDE_DIR}
)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB common_SOURCE_FILES ${common_SOURCE_DIR}/*.cpp)
file(GLOB common_INCLUDE_FILES ${common_INCLUDE_DIR}/*.h)

# because we used glob we need to remove LinkDef.hh - this is the downside
list(REMOVE_ITEM common_INCLUDE_FILES "${common_INCLUDE_DIR}/LinkDef.hh")

# Build ROOT dictionary
ROOT_GENERATE_DICTIONARY(${common_DICTIONARY} ${common_INCLUDE_FILES} LINKDEF ${common_INCLUDE_DIR}/LinkDef.hh)

#build shared library
add_library(${common_LIBRARY_SHARED} SHARED ${common_SOURCE_FILES} ${common_DICTIONARY}.cxx)

target_link_libraries(${common_LIBRARY_SHARED} ${ROOT_LIBRARIES})

##=============================================================================
##
## Installation
##
##=============================================================================

#install the library in the lib directory
install(TARGETS ${common_LIBRARY_SHARED}
    EXPORT HEPDOrbitalTargets
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION "${HEPDOrbital_HEADER_OUTPUT_DIRECTORY}/lib/include"
)

#install the root dictionary files
if(${ROOT_VERSION} VERSION_EQUAL 6.08.00 OR ${ROOT_VERSION} VERSION_GREATER 6.08.00)
        install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/lib${common_LIBRARY_SHARED}_rdict.pcm
                ${CMAKE_CURRENT_BINARY_DIR}/lib${common_LIBRARY_SHARED}.rootmap
                DESTINATION lib
        )
else()
        install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/G__${common_LIBRARY_SHARED}_rdict.pcm
                ${CMAKE_CURRENT_BINARY_DIR}/lib${common_LIBRARY_SHARED}.rootmap
                DESTINATION lib
        )
endif()


#install the event header files in the appropriate include directory
foreach(headerFile ${common_INCLUDE_FILES})
        install(FILES ${headerFile}
        DESTINATION ${HEPDOrbital_HEADER_OUTPUT_DIRECTORY}/common/include)
        unset(headerFile CACHE)
endforeach()



#########################

#
## You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
##   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
##   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#
##---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS RIO Net Geom MathCore GenVector)
#
##---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE})
#
#message(STATUS "Found source directory ${CMAKE_CURRENT_SOURCE_DIR}")
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)
##---Locate sources and headers
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
#
#find_library(HEPDOrbital_LIBRARIES NAMES SimpleDetEvent PATHS ${PROJECT_SOURCE_DIR}/../lib/)
#
#
## create the root dictionary for cling
#ROOT_GENERATE_DICTIONARY(src/G__GGeometry include/GGeometry.hh LINKDEF include/GGeometryLinkDef.hh MODULE GGeometry)
##
##---Create a shared library with generated dictionary
#add_library(GGeometry SHARED src/GGeometry.cc src/G__GGeometry.cxx ${headers})
#target_link_libraries(GGeometry ${ROOT_LIBRARIES})
#
##---- a primitive test
#add_executable(TestGeo test/TestGeo.cc)
#target_link_libraries(TestGeo ${ROOT_LIBRARIES})
#target_link_libraries(TestGeo GGeometry ${HEPDOrbital_LIBRARIES})
#
#
#
#install(TARGETS GGeometry DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
#install(FILES libGGeometry_rdict.pcm libGGeometry.rootmap DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
#
#
#
#
