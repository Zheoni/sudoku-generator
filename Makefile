STD = c++11
CXX = clang++

DEPENDENCIESDIR = ./dependencies

VPATH = $(DEPENDENCIESDIR)/parsegar

CXXFLAGS = -std=$(STD) -O3
CPPFLAGS = -I$(DEPENDENCIESDIR)


all: sudoku.cpp parsegar.o
	$(CXX) $^ $(CXXFLAGS) $(CPPFLAGS) -o genSudoku
	./genSudoku --help

parsegar.o: parsegar.cpp

.PHONY: clean
clean:
	rm genSudoku parsegar.o
