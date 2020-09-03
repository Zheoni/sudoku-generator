STD = c++11
CXX = clang++

DEPENDENCIESDIR = ./dependencies

VPATH = $(DEPENDENCIESDIR)/parsegar

CXXFLAGS = -std=$(STD) -O3
CPPFLAGS = -I$(DEPENDENCIESDIR)

OBJS = parsegar.o
EXES = sudoku

all: $(EXES)
	./sudoku --help

$(EXES): $(OBJS)

$(OBJS):

clean:
	$(RM) $(OBJS) $(EXES)

.PHONY: clean all
