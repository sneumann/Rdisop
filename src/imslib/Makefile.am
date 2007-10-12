## Process this file with automake to produce Makefile.in

## If you want to know why this is -- contrary to popular tradition -- *one* big Makefile.am,
## then please read "Recursive Make Considered Harmful" by Peter Miller available at
## http://aegis.sourceforge.net/auug97.pdf

AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip 1.8

EXTRA_DIST = libims.pc.in Doxyfile doc/imslib-doc.h doc/imslib.css CODING imslib.ebuild

## pkg-config

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libims.pc

## directories
## we used to have $(pkgincludedir) in here, but then files get installed
## into /usr/include/imslib/ whereas we want them in /usr/include/ims/.
imsdir = $(includedir)/ims
utilsdir = $(imsdir)/utils
decompdir = $(imsdir)/decomp
basedir = $(imsdir)/base
exceptiondir = $(basedir)/exception
parserdir = $(basedir)/parser
tclapdir = $(imsdir)/tclap
modifierdir = $(imsdir)/modifier
functorsdir = $(imsdir)/functors
calibdir = $(imsdir)/calib

## imslib

lib_LTLIBRARIES = src/libims.la
src_libims_la_LDFLAGS = -no-undefined -version-info 0:0:0

src_libims_la_SOURCES = \
	src/ims/element.cpp \
	src/ims/composedelement.cpp \
	src/ims/isotopedistribution.cpp \
	src/ims/alphabet.cpp \
	src/ims/weights.cpp \
	src/ims/distributedalphabet.cpp \
	src/ims/transformation.cpp \
	src/ims/isotopespecies.cpp \
	src/ims/base/parser/alphabettextparser.cpp \
	src/ims/base/parser/distributedalphabettextparser.cpp \
	src/ims/base/parser/massestextparser.cpp \
	src/ims/base/parser/moleculesequenceparser.cpp \
	src/ims/base/parser/standardmoleculesequenceparser.cpp \
	src/ims/base/parser/keggligandcompoundsparser.cpp \
	src/ims/base/parser/moleculeionchargemodificationparser.cpp \
	src/ims/calib/linepairstabber.cpp \
	src/ims/calib/matchmatrix.cpp \
	src/ims/calib/linearpointsetmatcher.cpp \
	src/ims/decomp/realmassdecomposer.cpp \
	src/ims/utils/distribution.cpp \
	src/ims/distributionprobabilityscorer.cpp \
	src/ims/characteralphabet.cpp \
	src/ims/nitrogenrulefilter.cpp 


## headers

ims_HEADERS = \
	src/ims/element.h \
	src/ims/elementsortcriteria.h \
	src/ims/composedelement.h \
	src/ims/isotopedistribution.h \
	src/ims/isotopespecies.h \
	src/ims/alphabet.h \
	src/ims/weights.h \
	src/ims/distributedalphabet.h \
	src/ims/masspeak.h \
	src/ims/intensitypeak.h \
	src/ims/tofpeak.h \
	src/ims/fragmentpeak.h \
	src/ims/massintensitypeak.h \
	src/ims/masstofpeak.h \
	src/ims/massintensitytofpeak.h \
	src/ims/peakpropertyiterator.h \
	src/ims/peaklist.h \
	src/ims/randomsequencegenerator.h \
	src/ims/sequencegenerator.h \
	src/ims/markovsequencegenerator.h \
	src/ims/distributeddata.h \
	src/ims/fragmenter.h \
	src/ims/idsequence.h \
	src/ims/pmffragmenter.h \
	src/ims/tandemfragmenter.h \
	src/ims/logger.h \
	src/ims/transformation.h \
	src/ims/chebyshevfitter.h \
	src/ims/peakcompare.h \
	src/ims/peakequalby.h \
	src/ims/peakequalto.h \
	src/ims/distributionprobabilityscorer.h \
	src/ims/characteralphabet.h \
	src/ims/nitrogenrulefilter.h

modifier_HEADERS = \
	src/ims/modifier/intensitynormalizermodifier.h \
	src/ims/modifier/massrangemodifier.h \
	src/ims/modifier/modifier.h \
	src/ims/modifier/multimodifier.h \
	src/ims/modifier/noisemodifier.h \
	src/ims/modifier/shiftmodifier.h \
	src/ims/modifier/sortmodifier.h \
	src/ims/modifier/unificationmodifier.h \
	src/ims/modifier/voidmodifier.h

functors_HEADERS = \
	src/ims/functors/alphabetgetmass.h

utils_HEADERS = \
	$(top_builddir)/src/ims/utils/math.h \
	src/ims/utils/gcd.h \
	src/ims/utils/stopwatch.h \
	src/ims/utils/distribution.h \
	src/ims/utils/print.h \
	src/ims/utils/matrix.h \
	src/ims/utils/compose_f_gx_t.h \
	src/ims/utils/compose_f_gx_hy_t.h

decomp_HEADERS = \
	src/ims/decomp/massdecomposer.h \
	src/ims/decomp/integermassdecomposer.h \
	src/ims/decomp/realmassdecomposer.h \
	src/ims/decomp/twomassdecomposer.h \
	src/ims/decomp/twomassdecomposer2.h \
	src/ims/decomp/classicaldpmassdecomposer.h \
	src/ims/decomp/decomputils.h \
	src/ims/decomp/residuetable.h

exception_HEADERS = \
	src/ims/base/exception/exception.h \
	src/ims/base/exception/invalidargumentexception.h \
	src/ims/base/exception/ioexception.h \
	src/ims/base/exception/unknowncharacterexception.h

parser_HEADERS = \
	src/ims/base/parser/alphabetparser.h \
	src/ims/base/parser/alphabettextparser.h \
	src/ims/base/parser/distributedalphabetparser.h \
	src/ims/base/parser/distributedalphabettextparser.h \
	src/ims/base/parser/massesparser.h \
	src/ims/base/parser/massestextparser.h \
	src/ims/base/parser/abstractmoleculesequenceparser.h \
	src/ims/base/parser/moleculesequenceparser.h \
	src/ims/base/parser/standardmoleculesequenceparser.h \
	src/ims/base/parser/keggligandcompoundsparser.h \
	src/ims/base/parser/moleculeionchargemodificationparser.h

tclap_HEADERS = \
	src/ims/tclap/CmdLineInterface.h \
	src/ims/tclap/ArgException.h \
	src/ims/tclap/CmdLine.h \
	src/ims/tclap/XorHandler.h \
	src/ims/tclap/MultiArg.h \
	src/ims/tclap/UnlabeledMultiArg.h \
	src/ims/tclap/ValueArg.h \
	src/ims/tclap/UnlabeledValueArg.h \
	src/ims/tclap/Visitor.h \
	src/ims/tclap/Arg.h \
	src/ims/tclap/HelpVisitor.h \
	src/ims/tclap/SwitchArg.h \
	src/ims/tclap/VersionVisitor.h \
	src/ims/tclap/IgnoreRestVisitor.h \
	src/ims/tclap/CmdLineOutput.h \
	src/ims/tclap/StdOutput.h \
	src/ims/tclap/DocBookOutput.h

calib_HEADERS =\
	src/ims/calib/calibrator.h \
	src/ims/calib/geometriccalibrator.h \
	src/ims/calib/linestabbingcalibrator.h \
	src/ims/calib/pointsetmatchercalibrator.h \
	src/ims/calib/linepairstabber.h \
	src/ims/calib/matchmatrix.h \
	src/ims/calib/linearpointsetmatcher.h
##	src/ims/calib/lmsregressioncalibrator.h

EXTRA_DIST += tools/decompcommandline.h tools/options.h

## ims tools
bin_PROGRAMS = \
	tools/scoredecompositions \
	tools/peaklistvalidation \
	tools/numberdecompositions \
	tools/keggruntimes \
	tools/imsfrag \
	tools/imsdecomp \
	tools/imsintdecomp \
	tools/histogram \
	tools/decompvalidation \
	tools/imsdecompstatic \
	tools/elementalcompositions

tools_imsfrag_SOURCES = tools/imsfrag.cpp
tools_imsfrag_LDADD = src/libims.la

tools_scoredecompositions_SOURCES = tools/scoredecompositions.cpp
tools_scoredecompositions_LDADD = src/libims.la

tools_peaklistvalidation_SOURCES = tools/peaklistvalidation.cpp
tools_peaklistvalidation_LDADD = src/libims.la

tools_numberdecompositions_SOURCES = tools/numberdecompositions.cpp
tools_numberdecompositions_LDADD = src/libims.la

tools_keggruntimes_SOURCES = tools/keggruntimes.cpp
tools_keggruntimes_LDADD = src/libims.la

tools_imsdecomp_SOURCES = tools/imsdecomp.cpp
tools_imsdecomp_LDADD = src/libims.la

tools_imsintdecomp_SOURCES = tools/imsintdecomp.cpp tools/options.cpp
tools_imsintdecomp_LDADD = src/libims.la

tools_imsdecompstatic_SOURCES = tools/imsdecomp.cpp
tools_imsdecompstatic_LDADD = src/libims.la
tools_imsdecompstatic_LDFLAGS = -static

tools_histogram_SOURCES = tools/histogram.cpp

tools_decompvalidation_SOURCES = tools/decompvalidation.cpp
tools_decompvalidation_LDADD = src/libims.la

tools_elementalcompositions_SOURCES = tools/elementalcompositions.cpp
tools_elementalcompositions_LDADD = src/libims.la

##bin_peakexample_SOURCES = \
##	tools/tmp.cpp
##      src/ims/base/parser/alphabettextparser.cpp
##bin_test_LDADD = src/libims.la

## TODO: The examples should be compilable via 'make examples'
## and they should probably not be compiled during 'make check'.
## For now this is ok since we 'check' if they compile.

if CPPUNIT
TESTS = tests/imslib-tests tests/decomp-tests tests/modifier-tests tests/calib-tests
examples = examples/peaklist
check_PROGRAMS = $(TESTS) $(examples)

## sources for imslib-tests. add your test .cpp file here to
## have the test included in the binary
tests_imslib_tests_SOURCES = \
	tests/tests.cpp \
	tests/exceptiontest.cpp \
	tests/alphabettest.cpp \
	tests/weightstest.cpp \
	tests/distributedalphabettest.cpp \
	tests/elementtest.cpp \
	tests/composedelementtest.cpp \
	tests/masspeaktest.cpp \
	tests/tofpeaktest.cpp \
	tests/massintensitypeaktest.cpp \
	tests/peaklisttest.cpp \
	tests/base/parser/massestextparsertest.cpp \
	tests/base/parser/moleculesequenceparsertest.cpp \
	tests/randomsequencegeneratortest.cpp \
	tests/markovsequencegeneratortest.cpp \
	tests/identitytransformationtest.cpp \
	tests/shifttransformationtest.cpp \
	tests/lineartransformationtest.cpp \
	tests/polynomialtransformationtest.cpp \
	tests/chebyshevfittertest.cpp \
	tests/isotopedistributiontest.cpp \
	tests/isotopespeciestest.cpp \
	tests/pmffragmentertest.cpp \
	tests/stopwatchtest.cpp \
	tests/massintensitytofpeaktest.cpp \
	tests/masstofpeaktest.cpp \
	tests/intensitypeaktest.cpp \
	tests/fragmentpeaktest.cpp \
	tests/peakpropertyiteratortest.cpp \
	tests/distributionprobabilityscorertest.cpp\
	tests/roundtest.cpp

tests_imslib_tests_LDADD = src/libims.la
tests_imslib_tests_LDFLAGS = $(CPPUNIT_LIBS)

## tests/decomp-tests
tests_decomp_tests_SOURCES = \
	tests/tests.cpp \
	tests/decomp/twomassdecomposer2test.cpp \
	tests/decomp/integermassdecomposertest.cpp \
	tests/decomp/realmassdecomposertest.cpp

tests_decomp_tests_LDADD = src/libims.la
tests_decomp_tests_LDFLAGS = $(CPPUNIT_LIBS)

## tests/modifier-tests
tests_modifier_tests_SOURCES = \
	tests/tests.cpp \
	tests/modifier/intensitynormalizermodifiertest.cpp \
	tests/modifier/massrangemodifiertest.cpp \
	tests/modifier/multimodifiertest.cpp \
	tests/modifier/shiftmodifiertest.cpp \
	tests/modifier/sortmodifiertest.cpp \
	tests/modifier/unificationmodifiertest.cpp \
	tests/modifier/voidmodifiertest.cpp
## 	tests/modifier/noisemodifiertest.cpp

tests_modifier_tests_LDADD = src/libims.la
tests_modifier_tests_LDFLAGS = $(CPPUNIT_LIBS)

## tests/calib-tests
tests_calib_tests_SOURCES = \
	tests/tests.cpp \
	tests/calib/linearpointsetmatchertest.cpp \
	tests/calib/linepairstabbertest.cpp \
	tests/calib/matchmatrixtest.cpp

tests_calib_tests_LDADD = src/libims.la
tests_calib_tests_LDFLAGS = $(CPPUNIT_LIBS)

## examples
examples_peaklist_SOURCES = \
	examples/peaklist.cpp
examples_peaklist_LDADD = src/libims.la

else
check:
	@echo
	@echo "Sorry, Cppunit was not found during ./configure, can not compile unit tests."
	@echo
	@exit 1
endif
