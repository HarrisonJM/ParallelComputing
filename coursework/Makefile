CXX := mpic++
CXX_FLAGS := -g -fopenmp -std=c++14 -lpthread -Wall
LD_LIBS := -lcrypto

BUILDROOT := $(shell pwd)/src/
CXX_INCLUDE := -I$(BUILDROOT)/etc/

SOURCES := $(shell find $(BUILDROOT) -name "*.cpp")
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
ALLFILES := $(SOURCES)

all: coursework.out

coursework.out: $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(CXX_INCLUDE) $^ -o $(BUILDROOT)/$@ $(LD_LIBS)
#
#
#
#coursework.out: cipherDoer.o key.o courseworkHandler.o
#	$(CXX) cipherDoer.o key.o courseworkHandler.o
#
#pc1.o: $(BUILDROOT)/src/etc/binLib/pc1.pp
#
#cipherDoer.o: $(BUILDROOT)/src/etc/ssl/cipherDoer.cpp
#	$(CXX) $(CXX_FLAGS) -I$(CXX_INCLUDE) -c $(BUILDROOT)/etc/ssl/cipherDoer.cpp
#
#key.o: $(BUILDROOT)/src/etc/ssl/key.cpp
#	$(CXX) $(CXX_FLAGS) -I$(CXX_INCLUDE) -c $(BUILDROOT)/etc/key/key.cpp
#
#courseworkHandler.o: $(BUILDROOT)/src/etc/ssl/courseworkHandler.cpp
#	$(CXX) $(CXX_FLAGS) -I$(CXX_INCLUDE) -c $(BUILDROOT)/etc/courseworkHandler.cpp

.PHONY: all