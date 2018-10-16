#----------------------------------------------------------------------------
# Setup the project
#
project(digitization CXX)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# set paths
set(digitization_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(digitization_SOURCE_DIR ${digitization_DIR}/src)
set(digitization_INCLUDE_DIR ${digitization_DIR}/include)

# include cern root cmake macros
include(${ROOT_USE_FILE})

include_directories(
    ${ROOT_INCLUDE_DIRS}
    ${common_INCLUDE_DIR}
    ${digitization_INCLUDE_DIR}
)

file(GLOB digitization_SOURCE_FILES ${digitization_SOURCE_DIR}/*.cc)
file(GLOB digitization_INCLUDE_FILES ${digitization_INCLUDE_DIR}/*.hh)

add_executable(SimpleAssociate ${digitization_SOURCE_FILES})
target_link_libraries(SimpleAssociate ${ROOT_LIBRARIES} ${common_LIBRARY_SHARED})


#----------------------------------------------------------------------------
# Install the executables to 'bin' directory under
# CMAKE_INSTALL_PREFIX/HEPDOrbital_VERSION
#

install(TARGETS SimpleAssociate
    EXPORT HEPDOrbitalTargets
    DESTINATION bin
)

