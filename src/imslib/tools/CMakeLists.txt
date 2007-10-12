set(TOOLS
	scoredecompositions
	peaklistvalidation
	numberdecompositions
	keggruntimes
	imsfrag
	imsdecomp
	decompvalidation
	elementalcompositions)

foreach(tool ${TOOLS})
	add_executable(${tool} ${tool}.cpp)
	target_link_libraries(${tool} ims)
endforeach(tool)
add_executable(imsintdecomp imsintdecomp.cpp options.cpp)
target_link_libraries(imsintdecomp ims)
add_executable(histogram histogram.cpp)

install(TARGETS ${TOOLS} DESTINATION bin/)
