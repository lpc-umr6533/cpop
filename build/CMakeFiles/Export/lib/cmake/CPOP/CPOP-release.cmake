#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "InformationSystem" for configuration "Release"
set_property(TARGET InformationSystem APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(InformationSystem PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/CPOP/lib/libInformationSystem.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS InformationSystem )
list(APPEND _IMPORT_CHECK_FILES_FOR_InformationSystem "${_IMPORT_PREFIX}/CPOP/lib/libInformationSystem.a" )

# Import target "cReader" for configuration "Release"
set_property(TARGET cReader APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cReader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/CPOP/lib/libcReader.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS cReader )
list(APPEND _IMPORT_CHECK_FILES_FOR_cReader "${_IMPORT_PREFIX}/CPOP/lib/libcReader.a" )

# Import target "Platform_SMA" for configuration "Release"
set_property(TARGET Platform_SMA APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Platform_SMA PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/CPOP/lib/libPlatform_SMA.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Platform_SMA )
list(APPEND _IMPORT_CHECK_FILES_FOR_Platform_SMA "${_IMPORT_PREFIX}/CPOP/lib/libPlatform_SMA.a" )

# Import target "Modeler" for configuration "Release"
set_property(TARGET Modeler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Modeler PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/CPOP/lib/libModeler.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Modeler )
list(APPEND _IMPORT_CHECK_FILES_FOR_Modeler "${_IMPORT_PREFIX}/CPOP/lib/libModeler.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
