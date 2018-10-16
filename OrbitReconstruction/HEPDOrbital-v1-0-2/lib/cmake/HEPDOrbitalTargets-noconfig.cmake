#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HEPDOrbitalCommon" for configuration ""
set_property(TARGET HEPDOrbitalCommon APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(HEPDOrbitalCommon PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libCore.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libRIO.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libNet.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libHist.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libGraf.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libGraf3d.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libGpad.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libTree.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libRint.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libPostscript.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libMatrix.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libPhysics.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libMathCore.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libThread.so;/home/fool/software/root-6.08.00/root-6.08.00-install/lib/libMultiProc.so"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libHEPDOrbitalCommon.so"
  IMPORTED_SONAME_NOCONFIG "libHEPDOrbitalCommon.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS HEPDOrbitalCommon )
list(APPEND _IMPORT_CHECK_FILES_FOR_HEPDOrbitalCommon "${_IMPORT_PREFIX}/lib/libHEPDOrbitalCommon.so" )

# Import target "SimpleDigi" for configuration ""
set_property(TARGET SimpleDigi APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SimpleDigi PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/SimpleDigi"
  )

list(APPEND _IMPORT_CHECK_TARGETS SimpleDigi )
list(APPEND _IMPORT_CHECK_FILES_FOR_SimpleDigi "${_IMPORT_PREFIX}/bin/SimpleDigi" )

# Import target "SimpleAssociate" for configuration ""
set_property(TARGET SimpleAssociate APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SimpleAssociate PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/SimpleAssociate"
  )

list(APPEND _IMPORT_CHECK_TARGETS SimpleAssociate )
list(APPEND _IMPORT_CHECK_FILES_FOR_SimpleAssociate "${_IMPORT_PREFIX}/bin/SimpleAssociate" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
