##### User configurable options #####

ARCH        = linux
CC          = mpicc
CLINKER     = $(CC)
CCC         = mpic++ 
CCLINKER    = $(CCC)
F77         = f77
FLINKER     = f77
OPTFLAGS    = -g 
LIB_PATH    = -L/usr/lib/openmpi/lib/ -L/usr/lib64/openmpi/lib/ 
LIB_LIST    = #-lmpich
INCLUDE_DIR =  -I/usr/include/openmpi-i386 -I/usr/include/openmpi-x86_64

### End User configurable options ###

SHELL = /bin/bash

CFLAGS  =  $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
CCFLAGS = $(CFLAGS)
FFLAGS =  $(OPTFLAGS)
FLIBS = $(FLIB_PATH) $(LIB_LIST)
LIBS = $(LIB_PATH) $(PROFLIB) $(LIB_LIST)
LIBSPP = $(MPIPPLIB) $(LIBS)
EXECS = puzzle

default: $(EXECS)

OBJS = worker.o master.o main.o puzzle.o
HEADERS = worker.h master.h common_decl.h puzzle.h

all: default

default: $(EXECS)

$(EXECS): $(OBJS) 
	$(CCLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJS) $(LIBS)

clean:
	rm -f *.o $(EXECS)

run:
	mpirun  -np 8 ./puzzle

.c.o:
	$(CC) $(CFLAGS) -w -c $*.c
.cc.o:
	$(CCC) $(CFLAGS) -w -c $*.cc
.f.o:
	$(F77) $(FFLAGS) -w -c $*.f

