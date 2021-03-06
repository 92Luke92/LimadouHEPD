# Config file for the HEPDOrbital package.
# It defines the following variables:
#  HEPDOrbital_INCLUDE_DIRS - include directories for HEPDOrbital
#  HEPDOrbital_LIBRARIES    - libraries to link against
#  HEPDOrbital_EXECUTABLES   - the HEPDOrbital executable
#
#  To Do:
#  HEPDOrbital_DEFINITIONS  - compile definitions needed to use HEPDOrbital
#  HEPDOrbital_USE_FILE     - path to a CMake module which may be included to help
#                      setup CMake variables and useful Macros
#
# You may supply a version number through find_package which will be checked
# against the version of this build. Standard CMake logic is used so that
# the EXACT flag may be passed, and otherwise this build will report itself
# as compatible with the requested version if:
#
#  VERSION_OF_THIS_BUILD >= VERSION_REQUESTED
#

# Find the HEPDOrbital include paths
get_filename_component(HEPDOrbital_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(HEPDOrbital_OUTPUT_INCLUDE_DIRS "")
foreach(_dir /home/fool/REPOFICO/LimadouHEPD/OrbitReconstruction/build/../HEPDOrbital-v1-0-2/include
	
	)
	list(APPEND HEPDOrbital_OUTPUT_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES HEPDOrbital_OUTPUT_INCLUDE_DIRS)
set(HEPDOrbital_INCLUDE_DIRS ${HEPDOrbital_OUTPUT_INCLUDE_DIRS})

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT HEPDOrbital_BINARY_DIR)
  include("${HEPDOrbital_CMAKE_DIR}/HEPDOrbitalTargets.cmake")
endif()

# These are IMPORTED targets created by HEPDOrbitalTargets.cmake
set(HEPDOrbital_LIBRARIES "/home/fool/REPOFICO/LimadouHEPD/OrbitReconstruction/build/../HEPDOrbital-v1-0-2/lib")
set(HEPDOrbital_EXECUTABLES "/home/fool/REPOFICO/LimadouHEPD/OrbitReconstruction/build/../HEPDOrbital-v1-0-2/bin")
