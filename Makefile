STD = c++11
CXX = clang++
CXXFLAGS = -std=$(STD) -O3

all: sudoku.cpp
	$(CXX) $^ $(CXXFLAGS) -o genSudoku
	./genSudoku --help
.PHONY: clean
clean:
	rm genSudoku
