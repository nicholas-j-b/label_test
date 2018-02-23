## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(LABELS_TEST_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = labels_test

labels_test_SOURCES = \
	main.cc \
	cfg.h \
	network.cc \
	network.h \
	todo

labels_test_LDFLAGS = 

labels_test_LDADD = $(LABELS_TEST_LIBS)


