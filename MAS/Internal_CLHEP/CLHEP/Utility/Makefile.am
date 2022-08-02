## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

includedir = $(prefix)/include/CLHEP
tempincludedir = $(TEMPDIR)/CLHEP/@PACKAGE@

pkginclude_HEADERS =	\
	atomic_int.h	\
	keywords.h	\
	memory.h	\
	noncopyable.h	\
	thread_local.h	\
	type_traits.h	\
	defs.h

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = defs.h

# copy headers into $(tempincludedir)
install-tmpHEADERS:
	@echo " install headers in $(tempincludedir) "
	@list='$(pkginclude_HEADERS)'; \
	if test $(tempincludedir); then \
	  for p in $$list; do \
	    if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	    f="`echo $$p | sed -e 's|^.*/||'`"; \
	    ./copy-header.pl $$d$$p $(tempincludedir)/$$f; \
	  done; \
	fi; test -z "$$fail"
