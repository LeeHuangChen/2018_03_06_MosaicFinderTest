#
# boost librairy
#
PATH_BOOST_LIB = -L /usr/lib 
# 
# source files 
#
SOURCES = $(wildcard ./bin/*.cpp)

#
# executable name and paths def
#
EXECNAME = mosaicFinder
EXECPATH = ./

#
# parameters for various compilers
#
GCC_NAME    = g++
GCC_FLAGS   = -Wall -ansi -O2 -pedantic -Werror
GCC_LIBS    = -lm -lboost_program_options
GCC_DEFINES = 
GCC_OBJECTS = $(SOURCES:.cpp=.o)
GCC_REMOVE  = 'rm' ./bin/*.o
CC_NAME     = CC
CC_FLAGS    = 
CC_DEFINES  = 
CC_LIBS     = -lm
CC_OBJECTS  = $(SOURCES:.cpp=.o)
CC_REMOVE   = 'rm' *.o

#
# CHANGE THESE LINES TO USE YOUR FAVORITE COMPILER
#
CCC      = $(GCC_NAME)
FLAGS    = $(GCC_FLAGS)
LIBS     = $(GCC_LIBS)
DEFINES  = $(GCC_DEFINES)
OBJECTS  = $(GCC_OBJECTS)
REMOVE   = $(GCC_REMOVE)
INCLUDES = 

.SUFFIXES: .cpp

exec: $(OBJECTS)
	$(CCC) $(FLAGS) $(PATH_BOOST_LIB) $(DEFINES) $(INCLUDES) $(OBJECTS) $(LIBS) -o $(EXECPATH)$(EXECNAME)
all: clean exec

clean:
	$(REMOVE)
	
.cpp.o:
	$(CCC) $(INCLUDES)  $(DEFINES) $(FLAGS) -c $< -o $@

.cpp.obj:
	$(CCC) $(INCLUDES) $(DEFINES) $(FLAGS) -c $< -o $@

bigclean:
	$(REMOVE)
	rm $(EXENAME)

dep:
	$(CCC) $(INCLUDES) -MM *.cpp > makefile.dep

ifneq ($(wildcard makefile.dep),)
 include makefile.dep
endif
