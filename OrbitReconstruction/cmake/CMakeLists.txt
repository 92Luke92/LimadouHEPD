##=============================================================================
##
## HEPDOrbital Analysis Software CMake Build System
##
## Details: This runs the HEPDOrbital install function
##
##=============================================================================

# call cmake install script to search for the HEPDOrbital header files
install(CODE "set(HEPDOrbital_VERSION_IN \"${HEPDOrbital_VERSION}\")")
install(SCRIPT HEPDOrbitalinstallScript.cmake)
