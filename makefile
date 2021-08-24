export
OBJ_SW = main.o
OBJ_SW_MT = main-mt.o

DEB = -g -D DEBUG
CXXFLAGS=-Wall -fopenmp

LDFLAGS= -lgmp -lgmpxx

all: sudoku-walker sudoku-walker-mt

sudoku-walker: $(OBJ_SW)
	g++ -o $@ $(OBJ_SW) $(LDFLAGS)

sudoku-walker-mt: $(OBJ_SW_MT)
	g++ -o $@ $(OBJ_SW_MT) $(LDFLAGS) -fopenmp

#scorpire come fare assegnamenti di variabili senza avere un target
deb: CXXFLAGS+=$(DEB)
deb: cleanall all sudoku-walker

-include dependencies.txt

#---	PHONY target

#	phony comuni
.PHONY: clean cleanall all

depend:
	g++ -MM *.cpp > dependencies.txt
	 $(MAKE) -C $(SUBDIRS) depend

clean:
	rm -f -v *.o
	#$(MAKE) -C $(SUBDIRS) clean
cleanall:
	rm -f -v sudoku-walker *.o
	#$(MAKE) -C $(SUBDIRS) cleanall
