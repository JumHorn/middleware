MAKEFILE      = Makefile

EQ            = =

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -Dlinux
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I/Users/jum/OpenSource/libco
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
LINK          = g++
# LFLAGS        = -Wl,--no-undefined -Wl,-rpath,'$$ORIGIN'
LIBS          = $(SUBLIBS) -L/Users/jum/OpenSource/libco/lib -lcolib
AR            = ar cqs
RANLIB        =
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = .

####### Files

SOURCES       = libco_core.cpp

OBJECTS       = libco_core.o

TARGET        = test


####### Build rules

ALL : test core

test:  libco_test.o
	$(LINK) $(LFLAGS) -o test libco_test.o $(LIBS)

libco_test.o: libco_test.cpp
	$(CXX) -c $(CFLAGS) $(INCPATH) -o libco_test.o libco_test.cpp

core:  libco_core.o
	$(LINK) $(LFLAGS) -o core libco_core.o $(LIBS)

libco_core.o: libco_core.cpp
	$(CXX) -c $(CFLAGS) $(INCPATH) -o libco_core.o libco_core.cpp
