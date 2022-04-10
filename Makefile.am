ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libsobacas.la

libsobacas_la_SOURCES = winscard-wrapper.c
libsobacas_la_CFLAGS = -I$(top_srcdir)/PCSC $(AM_CFLAGS) @YAKISOBA_CFLAGS@
libsobacas_la_LIBADD = @YAKISOBA_LIBS@

pcsc_headers = \
	PCSC/winscard.h \
	PCSC/pcsclite.h \
	PCSC/wintypes.h

EXTRA_libsobacas_la_SOURCES = $(pcsc_headers)

if INSTALL_PCSC_HEADERS
nobase_include_HEADERS = $(pcsc_headers)
endif
