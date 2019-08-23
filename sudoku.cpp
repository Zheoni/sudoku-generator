#include <array>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>

#define EASY 25
#define MEDIUM 35
#define HARD 50
#define INSANE 64

// saves two arrays of 81 unsigned shorts, one for the solution and another for
// the puzzle
struct sudoku {
    std::array<unsigned short, 81> solution, puzzle;
};

// returns the index for a one dimensional array as it were 9x9
inline static unsigned short index(int i, int j) { return i * 9 + j; }

// returns a random number from 1 to 9
static unsigned short dice() {
    static std::random_device rd;
    static std::ranlux24 generator(rd());
    static std::uniform_int_distribution<unsigned short> distribution(1, 9);
    return distribution(generator);
}

// checks if num is valid in the row "row" in board
static bool isValidRow(int num, int row,
                       const std::array<unsigned short, 81>& board) {
    for (int i = 0; i < 9; ++i) {
        if (num == board[index(row, i)]) return false;
    }

    return true;
}

// checks is num is valid in the column col in board
static bool isValidCol(int num, int col,
                       const std::array<unsigned short, 81>& board) {
    for (int i = 0; i < 9; ++i) {
        if (num == board[index(i, col)]) return false;
    }

    return true;
}

// check if num is valid at row, col in board, checking only the box constraint
static bool isValidBox(int num, int row, int col,
                       const std::array<unsigned short, 81>& board) {
    const int R = row - row % 3;
    const int C = col - col % 3;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (num == board[index(R + i, C + j)]) return false;
        }
    }

    return true;
}

// checks if num is valid at row, col in board
inline static bool isValid(int num, int row, int col,
                           const std::array<unsigned short, 81>& board) {
    return isValidBox(num, row, col, board) && isValidCol(num, col, board) &&
           isValidRow(num, row, board);
}

// fills a box in a sudoku
// bRow and bCol have to be in range [1, 3]
static void fillBox(int bRow, int bCol, std::array<unsigned short, 81>& board,
                    unsigned short (*dice)()) {
    for (int i = bRow * 3; i < (bRow + 1) * 3; ++i) {
        for (int j = bCol * 3; j < (bCol + 1) * 3; ++j) {
            bool filled = false;
            while (!filled) {
                int num = dice();

                if (isValidBox(num, i, j, board)) {
                    board[index(i, j)] = num;
                    filled = true;
                }
            }
        }
    }
}

// returns a array with numbers 1 to 9 shuffled
static std::array<unsigned short, 9> getShuffledNumbers() {
    std::array<unsigned short, 9> numbers;
    for (int i = 0; i < 9; ++i) {
        numbers[i] = i + 1;
    }
    std::random_shuffle(numbers.begin(), numbers.end());
    return numbers;
}

// finds the first empty cell (value 0) in a board
static int findEmptyCell(const std::array<unsigned short, 81>& board) {
    int pos = -1;

    for (int i = 0; i < 81 && pos == -1; ++i) {
        if (board[i] == 0) {
            pos = i;
        }
    }

    return pos;
}

// solves a sudoku
static bool solveBoard(std::array<unsigned short, 81>& board) {
    // increasing a little bit randomness, it can also be without shuffle
    static std::array<unsigned short, 9> numbers(getShuffledNumbers());

    // find first empty value
    int pos = findEmptyCell(board);

    // if no more empty values, we are done
    if (pos == -1) return true;

    // try values if they are valid and if it end up solved, we are done.
    for (int num = 0; num < 9; num++) {
        const unsigned short number = numbers[num];
        if (isValid(number, pos / 9, pos % 9, board)) {
            board[pos] = number;
            if (solveBoard(board)) {
                return true;
            } else {
                board[pos] = 0;
            }
        }
    }
    return false;  // backtrack
}

// generate a random full sudoku board
void genBoard(std::array<unsigned short, 81>& board) {
    board.fill(0);

    // The diagonal boxes, independent of the others. Use this as seed
    for (int i = 0; i < 3; ++i) fillBox(i, i, board, dice);

    // solve the sudoku from the 3 boxes filled
    solveBoard(board);
}

// returns 0, 1 or 2. 2 means more than one solution
static int numberOfSolutions(std::array<unsigned short, 81>& board, int pos = 0,
                             int count = 0) {
    if (pos == 81) return 1 + count;

    if (board[pos] != 0) {
        // skip filled cells
        return numberOfSolutions(board, pos + 1, count);
    } else {
        // search for 2 solutions instead of 1
        // break, if 2 solutions are found
        for (int val = 1; val <= 9 && count < 2; ++val) {
            if (isValid(val, pos / 9, pos % 9, board)) {
                board[pos] = val;
                // add additional solutions
                count = numberOfSolutions(board, pos + 1, count);
            }
        }
        board[pos] = 0;  // reset on backtrack
        return count;
    }
}

// remove elements from a board
// n elements are going to be removed
// unique ensures the puzzle only have one solution
void removeElements(std::array<unsigned short, 81>& board, int n, bool unique) {
    int pos, value;
    for (int i = 0; i < n;) {
        pos = index(dice() - 1, dice() - 1);
        value = board[pos];
        if (value != 0) {
            board[pos] = 0;
            if (numberOfSolutions(board) > 1) {
                board[pos] = value;
                continue;
            }
            ++i;
        }
    }
}

// generate a sudoku and it solution
// difficulty means the number of empty cells
// unique means that the puzzle generated only have 1 solution
sudoku genSudoku(int difficulty = MEDIUM, bool unique = true) {
    std::array<unsigned short, 81> solution;
    genBoard(solution);
    std::array<unsigned short, 81> puzzle(solution);
    removeElements(solution, difficulty, unique);

    return {puzzle, solution};
}

// prints the sudoku board formatted
static void printBoard(const std::array<unsigned short, 81>& board) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int val = board[index(i, j)];
            std::cout << (val ? std::to_string(val) : " ") << " ";
            if (j == 2 || j == 5) std::cout << "| ";
        }
        std::cout << "\n";
        if (i == 2 || i == 5)
            std::cout << std::string(6, '-') << "+" << std::string(7, '-')
                      << "+" << std::string(6, '-') << "\n";
    }
}

// returns a string representing the sudoku board
std::string boardString(const std::array<unsigned short, 81>& board) {
    std::string str;
    std::for_each(board.begin(), board.end(), [&](unsigned short n) {
        str += (n ? std::to_string(n) : ".");
    });
    return str;
}

int main(int argc, const char** argv) {
    int N = 1, difficulty = MEDIUM;
    bool showSolutions = true, unique = true;

    // parsing command line args
    if (argc > 1) {
        if (std::strcmp(argv[1], "--help") == 0 ||
            std::strcmp(argv[1], "-help") == 0 ||
            std::strcmp(argv[1], "-h") == 0) {
            std::cout << "USAGE: " << argv[0] << " N D U S\nWhere:\n"
                      << "\tN (number) : Generate N puzzles\n"
                      << "\tD (number) : The puzzles have D empty cells\n"
                      << "\tU (1 or 0) : Require the puzzles to have an unique "
                         "solution\n"
                      << "\tS (1 or 0) : Show the solutions of the puzzles\n";
            return 0;
        }
        if (argc != 5) {
            std::cerr << "Use --help to see the usage." << std::endl;
            return 1;
        } else {
            N = std::stoi(argv[1]);
            difficulty = std::stoi(argv[2]);
            unique = std::stoi(argv[3]);
            showSolutions = std::stoi(argv[4]);
        }
    }

    // random seed for std::random_shuffle
    std::srand(std::time(nullptr));

    // one thread for every sudoku to be generate thus we can speed up things
    // when we need to generate several puzzles
    sudoku* sudokus = new sudoku[N];
    std::thread* ths = new std::thread[N];
    for (int i = 0; i < N; ++i) {
        ths[i] = std::thread(
            [&, i]() { sudokus[i] = genSudoku(difficulty, unique); });
    }
    for (int i = 0; i < N; ++i) {
        ths[i].join();
    }

    // printing results
    for (int i = 0; i < N; ++i) {
        if (N > 1) std::cout << "Puzzle " << i + 1 << std::endl;

        sudoku& s = sudokus[i];

        printBoard(s.puzzle);
        std::cout << std::endl;

        if (showSolutions) {
            printBoard(s.solution);
            std::cout << std::endl;
        }

        std::cout << boardString(s.puzzle) << std::endl;
        std::cout << std::endl;

        if (i != N - 1) {
            std::cout << std::string(25, '-') << std::endl;
        }
    }
    return 0;
}