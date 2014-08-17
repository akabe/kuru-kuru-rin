# /src/Makefile.am

include_HEADERS = \
	kkr-mainwindow.hpp \
	kkr-newspacedialog.hpp \
	kkr-rotationconfigdialog.hpp \
	kkr-grapharea.hpp \
	kkr-viewconfig.hpp \
	kkr-hypercube.hpp \
	kkr-color.hpp

bin_PROGRAMS    = kurukururin
kurukururin_CPPFLAGS = -ansi -W -Wall -g -ggdb $(Gtkmm_CFLAGS)
kurukururin_LDADD    = -lm $(Gtkmm_LIBS)
kurukururin_SOURCES  = \
	kkr-main.cpp \
	kkr-mainwindow.cpp \
	kkr-newspacedialog.cpp \
	kkr-rotationconfigdialog.cpp \
	kkr-grapharea.cpp \
	kkr-viewconfig.cpp \
	kkr-hypercube.cpp
