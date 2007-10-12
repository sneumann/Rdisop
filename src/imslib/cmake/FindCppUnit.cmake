# - Find CppUnit
# Find the native CppUnit includes and library
#
#  CPPUNIT_INCLUDE_DIR - where to find header files
#  CPPUNIT_LIBRARIES   - List of libraries when using CppUnit
#  CPPUNIT_FOUND       - True if CppUnit found.

FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/TestAssert.h
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(CPPUNIT_LIBRARY
  NAMES cppunit
  PATHS /usr/lib /usr/local/lib
)

IF(CPPUNIT_INCLUDE_DIR)
  IF(CPPUNIT_LIBRARY)
    SET( CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} )
    SET( CPPUNIT_FOUND "YES" )
  ENDIF(CPPUNIT_LIBRARY)
ENDIF(CPPUNIT_INCLUDE_DIR)

MARK_AS_ADVANCED(
  CPPUNIT_LIBRARY
  CPPUNIT_INCLUDE_DIR
  )
