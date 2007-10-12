# Author: Marcel Martin
#
# To Do:
# - FEATURES="maketest"

inherit debug

DESCRIPTION="Informatics for Mass Spectrometry Library"
HOMEPAGE="http://gi.cebitec.uni-bielefeld.de/ims/"
SRC_URI="${P}.tar.bz2"
RESTRICT="fetch nomirror"
LICENSE=""
SLOT="0"
KEYWORDS="~x86"
IUSE="doc debug"
DEPEND="virtual/libc
	doc? ( >=app-doc/doxygen-1.4.4 )"

pkg_nofetch() {
	einfo "Please obtain ${P}.tar.bz2 and move it to ${DISTDIR}"
}

src_compile() {
	econf $(use_enable debug) || die "econf failed"
	emake
	use doc && { doxygen || die "failed to build API docs" ; }
}

src_install() {
	make DESTDIR=${D} install || die
	if use doc; then
		dodoc README
		dohtml -r doxygen-doc/html/*
	fi
}
