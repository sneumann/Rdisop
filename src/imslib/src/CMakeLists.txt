add_library(ims SHARED
	ims/element.cpp
	ims/composedelement.cpp
	ims/isotopedistribution.cpp
	ims/alphabet.cpp
	ims/weights.cpp
	ims/distributedalphabet.cpp
	ims/transformation.cpp
	ims/isotopespecies.cpp
	ims/base/parser/alphabettextparser.cpp
	ims/base/parser/distributedalphabettextparser.cpp
	ims/base/parser/massestextparser.cpp
	ims/base/parser/moleculesequenceparser.cpp
	ims/base/parser/standardmoleculesequenceparser.cpp
	ims/base/parser/keggligandcompoundsparser.cpp
	ims/base/parser/moleculeionchargemodificationparser.cpp
	ims/calib/linepairstabber.cpp
	ims/calib/matchmatrix.cpp
	ims/calib/linearpointsetmatcher.cpp
	ims/decomp/realmassdecomposer.cpp
	ims/utils/distribution.cpp
	ims/distributionprobabilityscorer.cpp
	ims/characteralphabet.cpp
	ims/nitrogenrulefilter.cpp)

install(TARGETS ims DESTINATION lib/)

# install all header files
# we can't use "file(GLOB_RECURSE ...)" because that would also
# include src/ims/tclap/*.h.
#
# The file src/ims/calib/lmsregressioncalibrator.h should not be
# installed.
file(GLOB IMS_HEADERS ims/*.h)
install(FILES ${IMS_HEADERS} DESTINATION include/ims)
# subdirectories
set(header_dirs modifier functors utils decomp calib base/exception base/parser)
foreach(header_dir ${header_dirs})
	file(GLOB var${header_dir} ims/${header_dir}/*.h)
	# TODO this is a bit ugly
	list(REMOVE_ITEM var${header_dir} ${CMAKE_SOURCE_DIR}/src/ims/calib/lmsregressioncalibrator.h)
	install(FILES ${var${header_dir}} DESTINATION include/ims/${header_dir})
endforeach(header_dir)
