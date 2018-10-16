##=============================================================================
##
## HEPDOrbital Analysis Software CMake Build System
##
## Details: This is a convenience function to produce a file with the header
## locations.
##
##=============================================================================

# make sure that the CMAKE_INSTALL_PREFIX is initialized
if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  #read Limadou version from file saved during cmake configure
  set(HEPDOrbital_INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
  message(STATUS "Searching for HEPDOrbital header files in: ${HEPDOrbital_INCLUDE_DIRECTORY}")

  #Set the commands
  set(find_command find ${HEPDOrbital_INCLUDE_DIRECTORY} -name "*.h")
  set(xargs_command xargs -I{} dirname {})
  set(sort_command sort)
  set(uniq_command uniq)
  set(tr_command tr '\n' ':')

  execute_process(COMMAND ${find_command}
    COMMAND ${xargs_command}
    COMMAND ${sort_command}
    COMMAND ${uniq_command}
    COMMAND ${tr_command}
    WORKING_DIRECTORY ${HEPDOrbital_INCLUDE_DIRECTORY}
    RESULT_VARIABLE FIND_HEPDOrbital_HEADERS_RESULT
    OUTPUT_VARIABLE FIND_HEPDOrbital_HEADERS_POST_INSTALL
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

# set the name of the output file
  set(outfile "${CMAKE_INSTALL_PREFIX}/HEPDOrbital_header_locations")


# Save paths to file
  file(WRITE ${outfile} ${FIND_HEPDOrbital_HEADERS_POST_INSTALL})

# Display important information at the end of the installation process
# for example the systems environment paths that need to be set.

  if(${FIND_HEPDOrbital_HEADERS_RESULT} EQUAL 0)
    message(STATUS "Successfully found the HEPDOrbital header files: ${FIND_HEPDOrbital_HEADERS_RESULT}")
    message(STATUS "HEPDOrbital header files are located in the following directories:")
    message(STATUS "${FIND_HEPDOrbital_HEADERS_POST_INSTALL}")
    message("\n\n----------- IMPORTANT INFORMATION ----------\n\n")
    message(STATUS "The HEPDOrbital header file paths have been saved to the file HEPDOrbital_header_locations")
    message(STATUS "This file is located in the ${CMAKE_INSTALL_PREFIX} directory")
    message(STATUS "For HEPDOrbital to work you need to set the following environment variables e.g. for BASH:")
    message(STATUS "export HEPDOrbital=${CMAKE_INSTALL_PREFIX}")
    message(STATUS "export PATH=\${PATH}:\${HEPDOrbital}/bin")
    message(STATUS "MacOS and OSX:")
    message(STATUS "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:\${HEPDOrbital}/lib")
    message(STATUS "Linux:")
    message(STATUS "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:\${HEPDOrbital}/lib")
    message(STATUS "You also need to set the root include path:")
    message(STATUS "export ROOT_INCLUDE_PATH=\${ROOT_INCLUDE_PATH}:\$(cat \${HEPDOrbital}/HEPDOrbital_header_locations)")
    set(HEPDOrbital ${CMAKE_INSTALL_PREFIX})
    file(WRITE HEPDOrbital.sh "export HEPDOrbital=${CMAKE_INSTALL_PREFIX}\n")
    file(APPEND HEPDOrbital.sh "export PATH=$PATH:${HEPDOrbital}/bin\n")
    if(${APPLE})
    file(APPEND HEPDOrbital.sh "export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:${HEPDOrbital}/lib\n") 
    else()
    file(APPEND HEPDOrbital.sh "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HEPDOrbital}/lib\n")
    endif()
    file(APPEND HEPDOrbital.sh "export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$(cat ${HEPDOrbital}/HEPDOrbital_header_locations)\n")
    message(STATUS "Source the file HEPDOrbital.sh in your shell to set the variables")
  else()
    message(FATAL_ERROR "Could not correctly produce HEPDOrbital_header_locations file")
  endif()
else()
  message(FATAL_ERROR "Sorry we do not support installing HEPDOrbital in the default system directories e.g. ${CMAKE_INSTALL_PREFIX}.")
  message(FATAL_ERROR "Please set an appropriate installation path using the -DCMAKE_INSTALL_PREFIX variable.")
endif()
