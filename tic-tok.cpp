#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>   // for system()
using namespace std;

// --- Game constants ---
const char EMPTY = ' ';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

// All 8 winning patterns (indices for 3x3 board as flat list)
const int WIN_PATTERNS[8][3] = {
    {0, 1, 2},  // row 1
    {3, 4, 5},  // row 2
    {6, 7, 8},  // row 3
    {0, 3, 6},  // col 1
    {1, 4, 7},  // col 2
    {2, 5, 8},  // col 3
    {0, 4, 8},  // main diagonal
    {2, 4, 6}   // anti diagonal
};

// --- Helper functions ---

// Clear console screen (Windows: cls, Linux/Mac: clear)
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Create empty board (vector of 9 cells)
vector<char> createBoard() {
    return vector<char>(9, EMPTY);
}

// Display board with cell numbers for empty spots
void displayBoard(const vector<char>& board) {
    auto cell = [&](int i) {
        return (board[i] == EMPTY) ? to_string(i + 1) : string(1, board[i]);
    };

    cout << "\n";
    cout << " " << cell(0) << " | " << cell(1) << " | " << cell(2) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(3) << " | " << cell(4) << " | " << cell(5) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(6) << " | " << cell(7) << " | " << cell(8) << "\n";
    cout << "\n";
}

// Get valid move from player
int getPlayerMove(const vector<char>& board, char currentPlayer) {
    while (true) {
        cout << "Player " << currentPlayer
             << " — choose a cell (1-9), or 0 to quit: ";
        string input;
        cin >> input;

        // Quit option
        if (input == "0") {
            return -1;
        }

        // Check numeric
        if (input.size() != 1 || !isdigit(input[0])) {
            cout << "Invalid input. Enter a number between 1 and 9.\n";
            continue;
        }

        int idx = input[0] - '1'; // convert char digit to int index
        if (idx < 0 || idx > 8) {
            cout << "Out of range. Choose 1–9.\n";
            continue;
        }
        if (board[idx] != EMPTY) {
            cout << "Cell already taken. Choose again.\n";
            continue;
        }
        return idx;
    }
}

// Place move
void placeMove(vector<char>& board, int index, char player) {
    board[index] = player;
}

// Check if someone won
char checkWinner(const vector<char>& board) {
    for (auto& pattern : WIN_PATTERNS) {
        char a = board[pattern[0]];
        char b = board[pattern[1]];
        char c = board[pattern[2]];
        if (a != EMPTY && a == b && b == c) {
            return a;
        }
    }
    return EMPTY; // no winner
}

// Check if draw
bool isDraw(const vector<char>& board) {
    for (char cell : board) {
        if (cell == EMPTY) return false;
    }
    return (checkWinner(board) == EMPTY);
}

// Switch player
char switchPlayer(char current) {
    return (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

// Play one round
string playRound(char startingPlayer) {
    vector<char> board = createBoard();
    char current = startingPlayer;

    while (true) {
        clearConsole();
        cout << "Tic-Tac-Toe (Two Players)\n";
        displayBoard(board);

        int move = getPlayerMove(board, current);
        if (move == -1) {
            cout << "Game quit.\n";
            return "quit";
        }

        placeMove(board, move, current);

        char winner = checkWinner(board);
        if (winner != EMPTY) {
            clearConsole();
            displayBoard(board);
            cout << "Player " << winner << " wins! 🎉\n";
            return string(1, winner);
        }

        if (isDraw(board)) {
            clearConsole();
            displayBoard(board);
            cout << "It's a draw!\n";
            return "draw";
        }

        current = switchPlayer(current);
    }
}

// --- Main program ---
int main() {
    cout << "Welcome to Tic-Tac-Toe (C++ version)\n";
    cout << "Players alternate X and O. First to get 3 in a row wins.\n";
    cout << "Enter 0 anytime to quit.\n\n";

    int scoreX = 0, scoreO = 0, draws = 0;
    char startingPlayer = PLAYER_X;

    while (true) {
        string result = playRound(startingPlayer);

        if (result == "quit") {
            cout << "Goodbye!\n";
            break;
        } else if (result == "X") {
            scoreX++;
        } else if (result == "O") {
            scoreO++;
        } else if (result == "draw") {
            draws++;
        }

        cout << "\nScoreboard:\n";
        cout << "  X: " << scoreX << "\n";
        cout << "  O: " << scoreO << "\n";
        cout << "  Draws: " << draws << "\n";

        cout << "\nPlay again? (y/n): ";
        char again;
        cin >> again;
        if (again == 'n' || again == 'N') {
            cout << "Final scores — X: " << scoreX
                 << "  O: " << scoreO
                 << "  Draws: " << draws << "\n";
            break;
        }

        startingPlayer = switchPlayer(startingPlayer); // fairness
    }

    return 0;
}
