prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: @imslib_PACKAGE_NAME@
Description: @imslib_PACKAGE_DESCRIPTION@
Requires:
Version: @imslib_MAJOR_VERSION@.@imslib_MINOR_VERSION@.@imslib_PATCH_VERSION@
Libs: -L${libdir} -lims
Cflags: -I${includedir}
