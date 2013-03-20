# Find FIELD3D headers and libraries.
#
#  FIELD3D_INCLUDE_DIRS - where to find Field3D uncludes.
#  FIELD3D_LIBRARIES    - List of libraries when using Field3D.
#  FIELD3D_FOUND        - True if Field3D found.

# Look for the header file.
FIND_PATH( FIELD3D_INCLUDE_DIR NAMES Field3D/Field.h)

# Look for the library.
FIND_LIBRARY( FIELD3D_LIBRARY NAMES Field3D)

# handle the QUIETLY and REQUIRED arguments and set FIELD3D_FOUND to TRUE if all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( FIELD3D DEFAULT_MSG FIELD3D_LIBRARY FIELD3D_INCLUDE_DIR)

# Copy the results to the output variables.
IF( FIELD3D_FOUND)
  SET( FIELD3D_LIBRARIES ${FIELD3D_LIBRARY})
  SET( FIELD3D_INCLUDE_DIRS ${FIELD3D_INCLUDE_DIR})
ELSE()
  SET( FIELD3D_LIBRARIES)
  SET( FIELD3D_INCLUDE_DIRS)
ENDIF()

MARK_AS_ADVANCED( FIELD3D_INCLUDE_DIR FIELD3D_LIBRARY)
