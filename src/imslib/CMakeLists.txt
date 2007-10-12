# noch zu implementierende Features (die es in der autotools-Version schon gibt)
#
# - 64 Bit aktivieren oder nicht
# - Debugmodus an oder nicht (NDEBUG setzen)
# - make check
# - (make examples)
# - CppUnit-Version ueberprufen (cppunit-config benutzen)
# - CONFIGURE_ARGS define (?)
# - libims.a
# - libims.so.0 und libims.so.0.0.0
# should we check for pkg-config? if yes, should FindPkgConfig or UsePkgConfig modules be used? 
# here is a link to FindPkgConfig.cmake http://public.kitware.com/pipermail/cmake/2005-March/006165.html

############## scrap
#include (CheckIncludeFiles)
# usage: check_include_files (<header> <RESULT_VARIABLE> )
##########

cmake_minimum_required(VERSION 2.4.3 FATAL_ERROR)

# creates variables imslib_SOURCE_DIR and imslib_BINARY_DIR
project(imslib)
message("imslib binary dir: ${imslib_BINARY_DIR}. source: ${imslib_SOURCE_DIR}")
include_directories(${imslib_SOURCE_DIR}/src ${imslib_BINARY_DIR}/src ${imslib_BINARY_DIR})
# AC_SUBST([AM_CPPFLAGS],['-I. -I./src -I$(srcdir)/src'])

# sets various variables to be used by pkgconfig and cmake amongst others
set(imslib_MAJOR_VERSION "0")
set(imslib_MINOR_VERSION "5")
set(imslib_PATCH_VERSION "0")
set(imslib_PACKAGE_DESCRIPTION "Informatics for Mass Spectrometry in Genomics and Proteomics")
set(imslib_PACKAGE_NAME "libims")
set(imslib_PACKAGE_VENDOR "Chair of Bioinformatics, FSU Jena")

subdirs(src tools)

include(TestCXXAcceptsFlag)

# other possibly useful warning flags:
# -Wfloat-equal -Weffc++ -Winline -Wno-unused
foreach(flag -Wall -W -Wwrite-strings -Wundef -O2)
	check_cxx_accepts_flag("${flag}" FLAG${flag})
	if(FLAG${flag})
		set(CMAKE_CXX_FLAGS "${flag} ${CMAKE_CXX_FLAGS}")
	endif(FLAG${flag})
endforeach(flag)

# TODO: NDEBUG

# AC_DEFINE_UNQUOTED(CONFIGURE_ARGS, "${ac_configure_args}", "configure arguments")
#
# IMS_ENABLE_64BIT

# INCLUDE(UsePkgConfig)
# PKGCONFIG(cppunit CPPUNIT_INCLUDE_DIR CPPUNIT_LINK_DIR CPPUNIT_LINK_FLAGS CPPUNIT_CFLAGS)
# IF(CPPUNIT_INCLUDE_DIR)
# 	MESSAGE("CppUnit found")
# ELSE(CPPUNIT_INCLUDE_DIR)
# 	MESSAGE("Cppunit not found!")
## ENDIF(CPPUNIT_INCLUDE_DIR)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(FindCppUnit)

if(CPPUNIT_FOUND)
	message(STATUS "CppUnit found")
	message("CPPUNIT_LIBRARIES = " ${CPPUNIT_LIBRARIES})
	ENABLE_TESTING()
    ADD_TEST(imslib-tests ${CMAKE_CTEST_COMMAND}
    --build-and-test
    "${imslib_SOURCE_DIR}/tests"
    "${imslib_BINARY_DIR}/tests"
    --build-generator ${CMAKE_GENERATOR}
    --build-project imslib-tests 
    --build-makeprogram ${CMAKE_MAKE_PROGRAM}
    --build-two-config
    --test-command imslib-tests)

else(CPPUNIT_FOUND)
	message(STATUS "CppUnit not found, unit testing won't work")
endif(CPPUNIT_FOUND)

include(CheckLibraryExists)
# some C99 functions
check_library_exists(m round "" HAVE_ROUND)
check_library_exists(m floor "" HAVE_FLOOR)

# or perhaps (doesn't work)
# include(CheckSymbolExists)
# check_symbol_exists(round math.h HAVE_ROUND)
# check_symbol_exists(floor math.h HAVE_FLOOR)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${imslib_BINARY_DIR}/config.h)
#CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if(NOT HAVE_ROUND AND NOT HAVE_FLOOR)
	message(FATAL_ERROR "neither floor nor round available in libm")
endif(NOT HAVE_ROUND AND NOT HAVE_FLOOR)

# generate ims/utils/math.h
if(NOT HAVE_ROUND)
	SET(ROUND_FUNCTION
"inline double round(double x) {
	if (x>=0) {
		return floor(x+0.5);
	} else {
		return -floor(-x+0.5);
	}
}")
endif(NOT HAVE_ROUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/math.h.cmake ${imslib_BINARY_DIR}/src/ims/utils/math.h)
install(FILES ${imslib_BINARY_DIR}/src/ims/utils/math.h DESTINATION include/ims/utils/)

# (perhaps) TODO: In this version exec_prefix, libdir, and includedir
# are always subdirectories of prefix. This restriction didn't exist
# while using autoconf.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libims.pc.cmake ${imslib_BINARY_DIR}/libims.pc @ONLY)
install(FILES ${imslib_BINARY_DIR}/libims.pc DESTINATION lib/pkgconfig/)

message("CURSRCDIR ${CMAKE_CURRENT_SOURCE_DIR}")

# Eine Loesung fuer make dist:
# (ergibt ein 'dist' target, das dann package.sh ausfuehrt)
#if(UNIX)
#	ADD_CUSTOM_TARGET(dist ${PROJECT_SOURCE_DIR}/package.sh)
#endif(UNIX)

# eine zweite Loesung fuer make dist:
# string(REGEX REPLACE ".*\\/" DIR_NAME "${PROJECT_SOURCE_DIR}")
# set(TOPLEVEL "${PROJECT_SOURCE_DIR}/..)
# add_custom_target(
# 	dist
# 	${CMAKE_COMMAND} -E chdir ${TOPLEVEL}
# 	${TAR_COMMAND} cvf ${PROJECT_NAME}.tar ${DIR_NAME})


####### old Makefile.am ###########

# EXTRA_DIST = libims.pc.in Doxyfile doc/imslib-doc.h doc/imslib.css CODING imslib.ebuild
#
# ## imslib
#
# lib_LTLIBRARIES = src/libims.la
# src_libims_la_LDFLAGS = -no-undefined -version-info 0:0:0
#
#
# ##bin_peakexample_SOURCES = \
# ##	tools/tmp.cpp
# ##      src/ims/base/parser/alphabettextparser.cpp
# ##bin_test_LDADD = src/libims.la
#
# ## TODO: The examples should be compilable via 'make examples'
# ## and they should probably not be compiled during 'make check'.
# ## For now this is ok since we 'check' if they compile.
#
# if CPPUNIT
# TESTS = tests/imslib-tests tests/decomp-tests tests/modifier-tests tests/calib-tests
# examples = examples/peaklist
# check_PROGRAMS = $(TESTS) $(examples)
#
# ## sources for imslib-tests. add your test .cpp file here to
# ## have the test included in the binary
# tests_imslib_tests_SOURCES = \
# 	tests/tests.cpp \
# 	tests/exceptiontest.cpp \
# 	tests/alphabettest.cpp \
# 	tests/weightstest.cpp \
# 	tests/distributedalphabettest.cpp \
# 	tests/elementtest.cpp \
# 	tests/composedelementtest.cpp \
# 	tests/masspeaktest.cpp \
# 	tests/tofpeaktest.cpp \
# 	tests/massintensitypeaktest.cpp \
# 	tests/peaklisttest.cpp \
# 	tests/base/parser/massestextparsertest.cpp \
# 	tests/base/parser/moleculesequenceparsertest.cpp \
# 	tests/randomsequencegeneratortest.cpp \
# 	tests/markovsequencegeneratortest.cpp \
# 	tests/identitytransformationtest.cpp \
# 	tests/shifttransformationtest.cpp \
# 	tests/lineartransformationtest.cpp \
# 	tests/polynomialtransformationtest.cpp \
# 	tests/chebyshevfittertest.cpp \
# 	tests/isotopedistributiontest.cpp \
# 	tests/isotopespeciestest.cpp \
# 	tests/pmffragmentertest.cpp \
# 	tests/stopwatchtest.cpp \
# 	tests/massintensitytofpeaktest.cpp \
# 	tests/masstofpeaktest.cpp \
# 	tests/intensitypeaktest.cpp \
# 	tests/fragmentpeaktest.cpp \
# 	tests/peakpropertyiteratortest.cpp \
# 	tests/distributionprobabilityscorertest.cpp\
# 	tests/roundtest.cpp
#
# tests_imslib_tests_LDADD = src/libims.la
# tests_imslib_tests_LDFLAGS = $(CPPUNIT_LIBS)
#
# ## tests/decomp-tests
# tests_decomp_tests_SOURCES = \
# 	tests/tests.cpp \
# 	tests/decomp/twomassdecomposer2test.cpp \
# 	tests/decomp/integermassdecomposertest.cpp \
# 	tests/decomp/realmassdecomposertest.cpp
#
# tests_decomp_tests_LDADD = src/libims.la
# tests_decomp_tests_LDFLAGS = $(CPPUNIT_LIBS)
#
# ## tests/modifier-tests
# tests_modifier_tests_SOURCES = \
# 	tests/tests.cpp \
# 	tests/modifier/intensitynormalizermodifiertest.cpp \
# 	tests/modifier/massrangemodifiertest.cpp \
# 	tests/modifier/multimodifiertest.cpp \
# 	tests/modifier/shiftmodifiertest.cpp \
# 	tests/modifier/sortmodifiertest.cpp \
# 	tests/modifier/unificationmodifiertest.cpp \
# 	tests/modifier/voidmodifiertest.cpp
# ## 	tests/modifier/noisemodifiertest.cpp
#
# tests_modifier_tests_LDADD = src/libims.la
# tests_modifier_tests_LDFLAGS = $(CPPUNIT_LIBS)
#
# ## tests/calib-tests
# tests_calib_tests_SOURCES = \
# 	tests/tests.cpp \
# 	tests/calib/linearpointsetmatchertest.cpp \
# 	tests/calib/linepairstabbertest.cpp \
# 	tests/calib/matchmatrixtest.cpp
#
# tests_calib_tests_LDADD = src/libims.la
# tests_calib_tests_LDFLAGS = $(CPPUNIT_LIBS)
#
# ## examples
# examples_peaklist_SOURCES = \
# 	examples/peaklist.cpp
# examples_peaklist_LDADD = src/libims.la
#
# else
# check:
# 	@echo
# 	@echo "Sorry, Cppunit was not found during ./configure, can not compile unit tests."
# 	@echo
# 	@exit 1
# endif
############ end old Makefile.am ##########


# -------- funktioniert noch nicht ------

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${imslib_PACKAGE_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${imslib_PACKAGE_VENDOR})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")
#set(CPACK_INSTALL_CMAKE_PROJECTS "lib")
#set(CPACK_PACKAGE_EXECUTABLES "lua" "lua")

# TODO check why adding TBZ2 here doesn't influence command "make package_source"
set(CPACK_SOURCE_GENERATOR "TGZ")
#SET(CPACK_GENERATOR "TBZ2;TGZ")
if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
	include(CPack)
endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
