cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project("MC2LVL0")

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


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

if(WIN32)
 find_package( ROOT REQUIRED libCore libRIO libRint libHist libNet libTree libHist)
elseif(UNIX)
 find_package( ROOT REQUIRED RIO Net Tree Hist)
endif()


set(EXECUTABLE_OUTPUT_PATH "exe/")

# Regular includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# MC includes ("RootEvent.hh", etc.)

include_directories(${PROJECT_SOURCE_DIR}/../../Simulation/source/Dataformats/include/)
include_directories(${PROJECT_SOURCE_DIR}/../../EventReconstruction/Common/include/)
include_directories(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../Simulation/build/lib/)

file(GLOB_RECURSE MCsources ${PROJECT_SOURCE_DIR}/../../Simulation/source/Dataformats/src/*.cc)
file(GLOB_RECURSE LVL0sources ${PROJECT_SOURCE_DIR}/../../EventReconstruction/Common/src/LEvRec0.cc ${PROJECT_SOURCE_DIR}/../../EventReconstruction/Common/src/LEvRec0File.cc)
file(GLOB_RECURSE CCsources ${PROJECT_SOURCE_DIR}/src/LEvRec0Writer.cc ${PROJECT_SOURCE_DIR}/src/trackeradc.cc ${PROJECT_SOURCE_DIR}/src/ecaladc.cc ${PROJECT_SOURCE_DIR}/src/MCcoorPhysicalFrame.cc ${PROJECT_SOURCE_DIR}/src/hits2edeppos.cc)
file(GLOB_RECURSE ecaladcsources  ${PROJECT_SOURCE_DIR}/src/calomev2adcmethod.cc ${PROJECT_SOURCE_DIR}/src/csv2fvec.cc  ${PROJECT_SOURCE_DIR}/src/laurentmethod.cc ${PROJECT_SOURCE_DIR}/src/francescomethod.cc)
message(${CCsources})
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/*/src/*.cc)

add_executable(testDetID src/testDetID.cc ${sources} ${PROJECT_SOURCE_DIR}/src/MCcoorPhysicalFrame.cc)
add_executable(trkTTree2csv src/trkTTree2csv.cc ${LVL0sources} )
add_executable(MC2Lvl0 src/MC2Lvl0.cc ${sources}  ${LVL0sources} ${CCsources} ${ecaladcsources})
add_executable(testmethod src/testmethod.cc ${ecaladcsources})

install(FILES DataFormatDict_rdict.pcm DESTINATION ${PROJECT_SOURCE_DIR}/../../Simulation/build/)
target_link_libraries(testDetID ${ROOT_LIBRARIES})
target_link_libraries(MC2Lvl0 ${ROOT_LIBRARIES} hepdsw)
target_link_libraries(trkTTree2csv ${ROOT_LIBRARIES})
target_link_libraries(testmethod ${ROOT_LIBRARIES})




#other cmakelists to be added here when written
