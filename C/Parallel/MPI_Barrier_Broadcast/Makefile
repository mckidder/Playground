##### User configurable options #####

ARCH        = linux
CC          = mpicc
CLINKER     = $(CC)
CCC         = mpic++ #mpicxx on karst
CCLINKER    = $(CCC)
F77         = mpif77
FLINKER     = mpif77
OPTFLAGS    = -g 
LIB_PATH    = #-L/usr/lib/openmpi/lib/ #-L/usr/lib/mpich2/lib/  #-L/usr/lib/lam/lib/
LIB_LIST    = #-lmpich
INCLUDE_DIR =  -I/usr/include/openmpi-x86_64/ #-I/usr/include/lam/ -I/usr/include/mpi2c++

### End User configurable options ###

SHELL = /bin/bash

CFLAGS  =  $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
CCFLAGS = $(CFLAGS)
FFLAGS =  $(OPTFLAGS)
FLIBS = $(FLIB_PATH) $(LIB_LIST)
LIBS = $(LIB_PATH) $(PROFLIB) $(LIB_LIST)
LIBSPP = $(MPIPPLIB) $(LIBS)
EXECS = barrier

all: default

default: $(EXECS)

barrier: barrier_bcast.o $(OBJS) 
	$(CCLINKER) $(OPTFLAGS) -o barrier barrier_bcast.o $(LIBS)

clean:
	rm -f *.o $(EXECS)

run:
	mpirun  -np 10 ./barrier # -hostfile hosts

.c.o:
	$(CC) $(CFLAGS) -w -c $*.c
.cc.o:
	$(CCC) $(CFLAGS) -w -c $*.cc
.f.o:
	$(F77) $(FFLAGS) -w -c $*.f

