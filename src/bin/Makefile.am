if HAVE_READLINE
  AM_CPPFLAGS = -I$(srcdir)/../include -DHAVE_READLINE
else
  AM_CPPFLAGS = -I$(srcdir)/../include
endif

if HAVE_BIN
bin_PROGRAMS = thraxcompiler thraxrewrite-tester thraxrandom-generator

if HAVE_READLINE
  LDADD= -L/usr/local/lib/fst ../lib/libthrax.la -lfstscript -lfstfar -lfst -lm -ldl -lreadline -lcurses
else
  LDADD= -L/usr/local/lib/fst ../lib/libthrax.la -lfstscript -lfstfar -lfst -lm -ldl
endif

thraxcompiler_SOURCES = compiler.cc

thraxrewrite_tester_SOURCES = rewrite-tester.cc rewrite-tester-utils.cc rewrite-tester-utils.h utildefs.cc utildefs.h

thraxrandom_generator_SOURCES = random-generator.cc utildefs.cc utildefs.h
endif

EXTRA_DIST = thraxmakedep regression_test.cc

install-exec-local: $(EXTRA_DIST)
	-mkdir -p -m 755 $(DESTDIR)$(bindir)
	-cp -p $(srcdir)/thraxmakedep $(DESTDIR)$(bindir)
	-chmod a+x $(DESTDIR)$(bindir)/thraxmakedep

uninstall-local:
	-rm -f $(DESTDIR)$(bindir)/thraxmakedep
