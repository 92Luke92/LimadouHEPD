cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project("Limadou MC to LVL0 files")

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
#set(VERSION_PATCH   0   CACHE STRING "LCaloCalibration definietely aligned.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)



add_definitions( -std=c++11)


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif(DOXYGEN_FOUND)

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#project(Common)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

if(WIN32)
 find_package( ROOT REQUIRED COMPONENTS libCore libRIO libRint libHist libNet libTree libHist)
elseif(UNIX)
 find_package( ROOT REQUIRED COMPONENTS RIO Net Tree Hist)
endif()

include(${ROOT_USE_FILE})

set(EXECUTABLE_OUTPUT_PATH "exe/")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(testDetID ${PROJECT_SOURCE_DIR}/src/testDetID.cc ${sources})
#target_link_libraries(TrackerQuickLook ${ROOT_LD_FLAGS})
target_link_libraries(testDetID ${ROOT_LIBRARIES})



#other cmakelists to be added here when written
