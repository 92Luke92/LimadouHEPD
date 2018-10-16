##=============================================================================
##
## Limadou HEPD Geo Coord Software CMake Build System
##
## Details: This is the top level CMakeLists.txt file
##
##=============================================================================

## Set project name
project(HEPDOrbital C CXX)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

## Set the software version number
## Set to 0.0.0 for now but will be used later
set(HEPDOrbital_VERSION_MAJOR 1)
set(HEPDOrbital_VERSION_MINOR 0)
set(HEPDOrbital_VERSION_PATCH 2)

set(HEPDOrbital_VERSION_NUM "HEPDOrbital-v${HEPDOrbital_VERSION_MAJOR}-${HEPDOrbital_VERSION_MINOR}-${HEPDOrbital_VERSION_PATCH}" CACHE STRING "gaps-v${HEPDOrbital_VERSION_MAJOR}-${HEPDOrbital_VERSION_MINOR}-${HEPDOrbital_VERSION_PATCH}")

message(STATUS "Running CMake configuration for HEPDOrbital software version: ${HEPDOrbital_VERSION_NUM}")

## ============================================================================
##
##  Setup install directories
##
## ============================================================================

# if CMAKE_INSTALL_PREFIX set to default (i.e. /usr/local) then change
# to binary directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "default install path" FORCE)
endif()

# define the install directory as CMAKE_INSTALL_PREFIX/HEPDOrbital_VERSION_NUM
if(NOT(CMAKE_INSTALL_PREFIX MATCHES "/${HEPDOrbital_VERSION_NUM}$"))
    set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/../${HEPDOrbital_VERSION_NUM}" CACHE PATH "HEPDOrbital installation directory." FORCE)
endif()

message(STATUS "Installing into ${CMAKE_INSTALL_PREFIX}")


# define the install lib, include and bin directories
set(HEPDOrbital_INSTALL_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "HEPDOrbital installation directory for libraries")
set(HEPDOrbital_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "HEPDOrbital installation directory for header files")
set(HEPDOrbital_INSTALL_BINARY_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "HEPDOrbital installation directory for executables")
set(HEPDOrbital_INSTALL_TEST_DIR   "${CMAKE_INSTALL_PREFIX}/tests" CACHE PATH "HEPDOrbital installation directory for test executables")


# Make relative paths absolute (needed later on)
foreach(p LIBRARY BINARY INCLUDE MACRO)
  set(var HEPDOrbital_INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()


# set the HEPDOrbital header output directory
# all HEPDOrbital header files will be copied into the installataion directory
# the headers will be placed withing their HEPDOrbital module paths inside
# ${CMAKE_INSTALL_PREFIX}/${HEPDOrbital_VERSION_NUM}/include
set(HEPDOrbital_HEADER_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "default header install path" FORCE)

# set the HEPDOrbital macros output directory
# all HEPDOrbital GEANT4 macros will be copied into the installataion directory
#set(HEPDOrbital_MACRO_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/bin/macros CACHE PATH "default macro install path" FORCE)

## ============================================================================
##
##  Set options for building libraries
##
## ============================================================================

SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")


# In many cases we want to make sure that the required libraries are
# always found independently from LD_LIBRARY_PATH and the install location.
# To always use RPATH we set the following options:

#To enable RPATH usage on OSX and osMac
set(CMAKE_MACOSX_RPATH 1)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${HEPDOrbital_INSTALL_LIBRARY_DIR}")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${HEPDOrbital_INSTALL_LIBRARY_DIR}" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${HEPDOrbital_INSTALL_LIBRARY_DIR}")
ENDIF("${isSystemDir}" STREQUAL "-1")

## ============================================================================
##
##  CERN ROOT ANALYSIS FRAMEWORK
##
## ============================================================================


# You need to tell CMake where to find the ROOT installation.
# For ROOT built with CMake.
# Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
if(APPLE)
    find_package(ROOT 6.00 )
elseif(UNIX)
    find_package(ROOT 6.00 )
endif()

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
if(ROOT_FOUND)
    include(${ROOT_USE_FILE})
else(NOT ROOT_FOUND)
    message(FATAL_ERROR "You need CERN ROOT for this program to work!")
endif(ROOT_FOUND)


## ============================================================================
##
##  Determine where all the header files are located
##  and set global include directories
##
## ============================================================================

file(GLOB_RECURSE HEPDOrbital_HEADER_FILES
    "*.h"
    )

set(HEPDOrbital_INCLUDE_DIRS "")
foreach(_headerFile ${HEPDOrbital_HEADER_FILES})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND HEPDOrbital_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES HEPDOrbital_INCLUDE_DIRS)

include_directories(${HEPDOrbital_INCLUDE_DIRS})


## ============================================================================
##
##  Set directories to include in project
##
## ============================================================================

# a bunch of on/off switches for the different projects
set(BUILD_MERGER ON CACHE BOOL "build merger (SimpleDigi) ")



add_subdirectory(common)

if (BUILD_MERGER)
  add_subdirectory(merger)
  add_subdirectory(unifier)
endif()


#this needs to be last
add_subdirectory(cmake)

## ============================================================================
##
##  Create the HEPDOrbitalConfig.cmake file for including HEPDOrbital in external projects.
##  This allows CMake to find HEPDOrbital using the find_package command.
##
## ============================================================================

export(TARGETS
    FILE "${PROJECT_BINARY_DIR}/HEPDOrbitalTargets.cmake"
    )

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE HEPDOrbital)

# Create the HEPDOrbitalConfig.cmake and HEPDOrbitalConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}" "${HEPDOrbital_HEADER_OUTPUT_DIRECTORY}")

# ... for the build tree
set(CONF_INCLUDE_DIRS "${HEPDOrbital_HEADER_OUTPUT_DIRECTORY}")
configure_file(HEPDOrbitalConfig.cmake.in "${PROJECT_BINARY_DIR}/HEPDOrbitalConfig.cmake" @ONLY)
## ... for the install tree
set(CONF_INCLUDE_DIRS "${HEPDOrbital_HEADER_OUTPUT_DIRECTORY}")
configure_file(HEPDOrbitalConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HEPDOrbitalConfig.cmake" @ONLY)
# ... for both
configure_file(HEPDOrbitalConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/HEPDOrbitalConfigVersion.cmake" @ONLY)

# Install the HEPDOrbitalConfig.cmake and HEPDOrbitalConfigVersion.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HEPDOrbitalConfig.cmake"
    "${PROJECT_BINARY_DIR}/HEPDOrbitalConfigVersion.cmake"
    DESTINATION lib/cmake
    COMPONENT dev
    )

# Install the export set for use with the install-tree
install(EXPORT HEPDOrbitalTargets
    DESTINATION lib/cmake
    COMPONENT dev
    )


#configure the pkg-config output file
#for the build tree
configure_file(HEPDOrbital.pc.cmake.in "${PROJECT_BINARY_DIR}/HEPDOrbital.pc" @ONLY)
#for the install tree
configure_file(HEPDOrbital.pc.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HEPDOrbital.pc" @ONLY)
#Install the HEPDOrbital pkg-config file
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HEPDOrbital.pc"
    DESTINATION share
    )
