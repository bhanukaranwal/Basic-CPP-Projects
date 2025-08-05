#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const char HUMAN = 'X';
const char AI = 'O';

void printBoard(const vector<char> &board) {
    cout << "\n";
    for (int i = 0; i < 9; ++i) {
        cout << (board[i] == ' ' ? to_string(i + 1) : string(1, board[i]));
        if ((i + 1) % 3 == 0)
            cout << "\n";
        else
            cout << " | ";
    }
    cout << "\n";
}

bool isMovesLeft(const vector<char> &board) {
    for (char c : board)
        if (c == ' ') return true;
    return false;
}

int evaluate(const vector<char> &b) {
    for (int row = 0; row < 3; ++row)
        if (b[row*3] == b[row*3+1] && b[row*3+1] == b[row*3+2]) {
            if (b[row*3] == AI) return +10;
            else if (b[row*3] == HUMAN) return -10;
        }
    for (int col = 0; col < 3; ++col)
        if (b[col] == b[col+3] && b[col+3] == b[col+6]) {
            if (b[col] == AI) return +10;
            else if (b[col] == HUMAN) return -10;
        }
    if (b[0] == b[4] && b[4] == b[8]) {
        if (b[0] == AI) return +10;
        else if (b[0] == HUMAN) return -10;
    }
    if (b[2] == b[4] && b[4] == b[6]) {
        if (b[2] == AI) return +10;
        else if (b[2] == HUMAN) return -10;
    }
    return 0;
}

int minimax(vector<char> board, int depth, bool isMax) {
    int score = evaluate(board);
    if (score == 10 || score == -10) return score;
    if (!isMovesLeft(board)) return 0;
    if (isMax) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i)
            if (board[i] == ' ') {
                board[i] = AI;
                best = max(best, minimax(board, depth + 1, false));
                board[i] = ' ';
            }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i)
            if (board[i] == ' ') {
                board[i] = HUMAN;
                best = min(best, minimax(board, depth + 1, true));
                board[i] = ' ';
            }
        return best;
    }
}

int findBestMove(vector<char> board) {
    int bestVal = numeric_limits<int>::min();
    int bestMove = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            board[i] = AI;
            int moveVal = minimax(board, 0, false);
            board[i] = ' ';
            if (moveVal > bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}

bool checkWin(const vector<char> &b, char player) {
    for (int i = 0; i < 3; ++i)
        if ((b[i*3] == player && b[i*3+1] == player && b[i*3+2] == player) ||
            (b[i] == player && b[i+3] == player && b[i+6] == player)) return true;
    if ((b[0] == player && b[4] == player && b[8] == player) ||
        (b[2] == player && b[4] == player && b[6] == player)) return true;
    return false;
}

int main() {
    vector<char> board(9, ' ');
    int move;
    cout << "Tic-Tac-Toe\nYou: X | Computer: O\n";
    cout << "Board positions:\n";
    for (int i = 0; i < 9; ++i)
        cout << (i+1) << ((i%3==2) ? "\n" : " ");
    while (true) {
        printBoard(board);
        cout << "Your move (1-9): ";
        cin >> move;
        if (move < 1 || move > 9 || board[move-1] != ' ') {
            cout << "Invalid move. Try again.\n";
            continue;
        }
        board[move-1] = HUMAN;
        if (checkWin(board, HUMAN)) {
            printBoard(board);
            cout << "Congratulations! You win!\n";
            break;
        }
        if (!isMovesLeft(board)) {
            printBoard(board);
            cout << "It's a draw.\n";
            break;
        }
        int aiMove = findBestMove(board);
        board[aiMove] = AI;
        cout << "Computer plays position " << (aiMove + 1) << "\n";
        if (checkWin(board, AI)) {
            printBoard(board);
            cout << "Computer wins! Better luck next time.\n";
            break;
        }
        if (!isMovesLeft(board)) {
            printBoard(board);
            cout << "It's a draw.\n";
            break;
        }
    }
    return 0;
}
