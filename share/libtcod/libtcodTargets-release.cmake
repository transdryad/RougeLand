#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libtcod::libtcod" for configuration "Release"
set_property(TARGET libtcod::libtcod APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libtcod::libtcod PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtcod.so.2.1.1"
  IMPORTED_SONAME_RELEASE "libtcod.so.2"
  )

list(APPEND _cmake_import_check_targets libtcod::libtcod )
list(APPEND _cmake_import_check_files_for_libtcod::libtcod "${_IMPORT_PREFIX}/lib/libtcod.so.2.1.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
