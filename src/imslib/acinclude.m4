# Henner Sudek <henner.sudek@cebitec.uni-bielefeld.de>, November 2004


# Check sizeof(long)
AC_DEFUN([IMS_SIZEOF_LONG],[
	AC_MSG_CHECKING([for sizeof long])
	AC_REQUIRE([AC_PROG_CC])
	AC_RUN_IFELSE([
	int main() { return(sizeof(long)); }],
		AC_MSG_ERROR("Unable to detect size of long!") ,
	[
		AC_DEFINE_UNQUOTED(SIZEOF_LONG, ${?} , SIZEOF_LONG )
		AC_MSG_RESULT(${?})
	]
	)
	]
)

# Check sizeof(long long int)
AC_DEFUN([IMS_SIZEOF_LONG_LONG],[
	AC_MSG_CHECKING([for sizeof long long])
	AC_REQUIRE([AC_PROG_CC])
	AC_RUN_IFELSE([
	int main() { return(sizeof(long long)); }],
		AC_MSG_ERROR("Unable to detect size of long long!") ,
	[
		AC_DEFINE_UNQUOTED(SIZEOF_LONG_LONG, ${?} , SIZEOF_LONG_LONG )
		AC_MSG_RESULT(${?})
	]
	)
	]
)

# Check for --enable-64bit
AC_DEFUN([IMS_ENABLE_64BIT],[
	AC_ARG_ENABLE(64bit,AC_HELP_STRING([--enable-64bit],[turn on 64bit support [default=no]]),
	[
	AC_MSG_CHECKING([if 64bit support can be used])
		case "$enableval" in
		yes)
			AC_MSG_RESULT([yes])
			CXXFLAGS="${CXXFLAGS} -m64"
			CFLAGS="${CFLAGS} -m64"
		;;
		no)
			AC_MSG_RESULT([no])
		;;
		esac
	]
	)
	]
)

# Check for --enable-debug
AC_DEFUN([IMS_ENABLE_DEBUG],[
	AC_ARG_ENABLE(debug,AC_HELP_STRING([--enable-debug],[turn on debugging mode [default=yes]]),
	[
	AC_MSG_CHECKING([if debug is set])
	case "$enableval" in
		yes)
			AC_DEFINE(DEBUG, 1, Debug)
			AC_MSG_RESULT([yes])
		;;
		no)
			AC_DEFINE([NDEBUG], 1, No Debug)
			AC_MSG_RESULT([no])
		;;
		esac
	],
	AC_DEFINE(DEBUG, 1, Debug)
	)
	]
)

# Check for --enable-tests
AC_DEFUN([IMS_ENABLE_TESTS],[
	AC_ARG_ENABLE(tests,AC_HELP_STRING([--enable-tests],[compile tests [default=no]]),
	[
	AC_MSG_CHECKING([if tests is set])
	case "$enableval" in
		yes)
			IMS_WANT_TESTS="yes"
			AC_MSG_RESULT([yes])
		;;
		no)
			IMS_WANT_TESTS="no"
			AC_MSG_RESULT([no])
		;;
		esac
	],
	IMS_WANT_TESTS="no"
	)
	]
)

