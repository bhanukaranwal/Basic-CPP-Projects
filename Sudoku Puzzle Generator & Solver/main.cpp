#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

const int N = 9; // Board size

void printBoard(const vector<vector<int>> &board) {
    cout << "-------------------------\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (j % 3 == 0) cout << "| ";
            if (board[i][j] == 0)
                cout << ". ";
            else
                cout << board[i][j] << " ";
        }
        cout << "|\n";
        if ((i+1) % 3 == 0)
            cout << "-------------------------\n";
    }
}

bool isSafe(const vector<vector<int>> &board, int row, int col, int num) {
    for (int x = 0; x < N; ++x)
        if (board[row][x] == num || board[x][col] == num)
            return false;
    int startRow = row - row%3, startCol = col - col%3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[startRow+i][startCol+j] == num)
                return false;
    return true;
}

bool solveSudoku(vector<vector<int>> &board) {
    for (int row = 0; row < N; ++row)
        for (int col = 0; col < N; ++col)
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; ++num)
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board))
                            return true;
                        board[row][col] = 0;
                    }
                return false;
            }
    return true;
}

// Util for shuffling and filling diagonal blocks
void fillDiagonal(vector<vector<int>> &board) {
    for (int k = 0; k < N; k += 3) {
        vector<int> nums{1,2,3,4,5,6,7,8,9};
        random_shuffle(nums.begin(), nums.end());
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board[k+i][k+j] = nums[i*3+j];
    }
}

bool fillRest(vector<vector<int>> &board, int i, int j) {
    if (j >= N && i < N-1) { i++; j = 0; }
    if (i >= N && j >= N) return true;
    if (i < 3)               { if (j < 3) j = 3; }
    else if (i < N-3)        { if (j == (int)(i/3)*3) j += 3; }
    else                     { if (j == 6) { i++; j = 0; if (i >= N) return true; } }

    for (int num = 1; num <= 9; ++num) {
        if (isSafe(board, i, j, num)) {
            board[i][j] = num;
            if (fillRest(board, i, j+1))
                return true;
            board[i][j] = 0;
        }
    }
    return false;
}

vector<vector<int>> generateSudoku() {
    vector<vector<int>> board(N, vector<int>(N,0));
    fillDiagonal(board);
    fillRest(board, 0, 3);
    return board;
}

void removeNumbers(vector<vector<int>> &board, int count = 40) {
    while (count > 0) {
        int i = rand() % 9, j = rand() % 9;
        if (board[i][j] != 0) { board[i][j] = 0; --count; }
    }
}

int main() {
    srand(time(0));
    cout << "Generating Sudoku Puzzle...\n";
    vector<vector<int>> board = generateSudoku();
    vector<vector<int>> puzzle = board;
    removeNumbers(puzzle);

    cout << "\nHere is your Sudoku Puzzle:\n";
    printBoard(puzzle);

    cout << "\nDo you want the solution? (y/n): ";
    char ans;
    cin >> ans;
    if (ans == 'y' || ans == 'Y') {
        solveSudoku(puzzle);
        cout << "\nSolved Sudoku:\n";
        printBoard(puzzle);
    } else {
        cout << "Enjoy solving!\n";
    }
    return 0;
}
