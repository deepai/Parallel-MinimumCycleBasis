IDIR =include
ODIR =obj
SRCDIR=src

EX_INCLUDE = ../include

DFLAGS = -DMULTI_THREAD -DNDEBUG
#-DPRINT -DVERBOSE

CC=g++
CFLAGS=-I$(IDIR) -I$(EX_INCLUDE) -fopenmp -lgomp $(DFLAGS) -w -std=c++11 -fpermissive
CXXFLAGS = -I$(IDIR) -I$(EX_INCLUDE) -fopenmp -lgomp $(DFLAGS) -w -std=c++11 -fpermissive

#CFLAGS=-I$(IDIR) -I$(EX_INCLUDE) -fopenmp -lgomp $(DFLAGS) -g -w -std=c++11 -fpermissive
#CXXFLAGS = -I$(IDIR) -I$(EX_INCLUDE) -fopenmp -lgomp $(DFLAGS) -g -w -std=c++11 -fpermissive

#-DMULTI_THREAD for multi-thread support
#-DPRINT to print the bccs in a file

LIBS=-lm

_DEPS = *.h 
DEPS = $(patsubst %,$(EX_INCLUDE)/%,$(_DEPS))

_EXTRA_DEPS = mmio.o \
			  Host_Timer.o \
			  CsrGraphSpanningTree.o \
			  CsrGraphDegreeTwoRemoval.o \
			  CsrGraphMultiSpanningTree.o \
			  FVS.o \
			  compressed_trees.o
EXTRA_DEPS = $(patsubst %,$(EX_INCLUDE)/%,$(_EXTRA_DEPS))

OBJ2 = mcb_cycle.o

all: mcb_cycle Test

mcb_cycle: $(OBJ2) $(EXTRA_DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(DFLAGS)

mcb_cycle.o: mcb_cycle.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS)

Test: Test.o $(EXTRA_DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(DFLAGS)

Test.o: Test.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS)

$(include)/%.o: $(include)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $(LIBS) $(DFLAGS) -o $@ $<
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~  *.o ../include/*.o mcb_cycle Test
