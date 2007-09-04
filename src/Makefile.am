## Makefile.am for kwest

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kwest

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
kwest_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kwest_LDADD = $(top_builddir)/src/babel/libbabel.a \
	$(top_builddir)/src/frotz/libfrotz.a $(LIB_KDEPRINT) $(LIB_KFILE)

# which sources should be compiled for kwest

# these are the headers for your project
noinst_HEADERS = kwest.h kwestview.h

# client stuff

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kwest.pot

KDE_ICON = AUTO

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Games
kdelnk_DATA = kwest.desktop

mimedir = $(kde_mimedir)/application
mime_DATA = zmachine.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kwest
rc_DATA = kwestui.rc
kwest_SOURCES = k_audio_oss.cpp k_file.cpp k_frotz.h k_init.cpp k_input.cpp \
	k_pic.cpp k_screen.cpp k_text.cpp kwest.cpp kwest.h kwestview.cpp kwestview.h \
	main.cpp
SUBDIRS = babel frotz
