
CSPATH = ./src
BINPATH = ./bin
LIBPATH = ./lib

# the source files 

sources = ride_barrier.cc main.cc

# objects are the same as the source files with .cc replaced with .o

objects = ride_barrier.o main.o

# headers files 

headers = ride_barrier.h

AR 		= ar
ARFLAGS 	= rvs
CC          	= gcc
CLINKER     	= gcc
CXX 		= g++
CXXFLAGS	= -O
CCFLAGS 	= -O
LIB_DIRS = 
LIBS 		= -lpthread #-lefence

exec = ridecheck

default: all

# make all will run commands regardless of whether there is a file "all"
all:    $(exec)

$(exec): $(objects) $(headers)
	$(CXX) $(OPTFLAGS) -o $(exec) $(objects) $(LIB_DIRS) $(LIBS)

# make clean will run commands regardless of whether there is a file "clean"
clean:
	rm -rf *.o
	rm -f $(exec)

lib/%.o : %.c
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< $(OUTPUT_OPTION)

lib/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(TARGET_ARCH) -c $< -o 

lib/%.o : %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(TARGET_ARCH) -c $< -o $@

run:
	$(exec)
