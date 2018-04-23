cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project("Limadou event reconstruction")

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "LCaloCalibration definietely aligned.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)



add_subdirectory(Common)
add_subdirectory(Tracker)
add_subdirectory(Calo)
add_subdirectory(exe)


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

