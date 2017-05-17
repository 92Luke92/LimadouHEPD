cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#project(Calo)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

if(WIN32)
 find_package( ROOT REQUIRED COMPONENTS libCore libRIO libNet libTree libHist libSpectrum)
elseif(UNIX)
 find_package( ROOT REQUIRED COMPONENTS RIO Net Tree Hist Spectrum)
endif()

include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/Common/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)






