CXX := g++
MPICXX := mpic++
CXX_FLAGS := -g -fopenmp -std=c++14 -lpthread -Wall
LD_LIBS := -lcrypto

BUILDROOT := $(shell pwd)/src/
CXX_INCLUDE := -I$(BUILDROOT)/etc/

SOURCES := $(shell find $(BUILDROOT) -name "*.cpp")
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
ALLFILES := $(SOURCES)

all: coursework.mpi coursework.omp
	[ -d bin ] || mkdir bin
	mv src/coursework.* bin/

coursework.omp: $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(CXX_INCLUDE) $^ -o $(BUILDROOT)/$@ $(LD_LIBS) -D_USE_OPEN_MP_

coursework.mpi: $(OBJECTS)
	$(MPICXX) $(CXX_FLAGS) $(CXX_INCLUDE) $^ -o $(BUILDROOT)/$@ $(LD_LIBS) -D_USE_MPI_

.PHONY: all clean
