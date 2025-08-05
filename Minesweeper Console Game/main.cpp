#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Cell {
    bool mine = false;
    bool revealed = false;
    bool flagged = false;
    int adjacent = 0;
};

void printBoard(const vector<vector<Cell>>& board, bool revealAll = false) {
    int H = board.size(), W = board[0].size();
    cout << "    ";
    for (int x = 0; x < W; ++x) cout << setw(2) << x << " ";
    cout << "\n";

    for (int y = 0; y < H; ++y) {
        cout << setw(2) << y << ": ";
        for (int x = 0; x < W; ++x) {
            if (revealAll) {
                if (board[y][x].mine) cout << " * ";
                else if (board[y][x].adjacent) cout << " " << board[y][x].adjacent << " ";
                else cout << "   ";
            } else {
                if (board[y][x].flagged) cout << " F ";
                else if (!board[y][x].revealed) cout << " # ";
                else if (board[y][x].mine) cout << " * ";
                else if (board[y][x].adjacent) cout << " " << board[y][x].adjacent << " ";
                else cout << "   ";
            }
        }
        cout << "\n";
    }
}

vector<pair<int,int>> neighbors(int y, int x, int H, int W) {
    vector<pair<int,int>> res;
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if ((dy || dx) && y+dy >= 0 && y+dy < H && x+dx >= 0 && x+dx < W)
                res.emplace_back(y+dy, x+dx);
    return res;
}

void reveal(vector<vector<Cell>>& board, int y, int x) {
    if (board[y][x].revealed || board[y][x].flagged) return;
    board[y][x].revealed = true;
    if (board[y][x].adjacent == 0 && !board[y][x].mine) {
        for (auto [ny, nx] : neighbors(y, x, board.size(), board[0].size()))
            reveal(board, ny, nx);
    }
}

int main() {
    srand(time(0));
    int H = 9, W = 9, mines = 10;
    cout << "--- MINESWEEPER (Console) ---\n";
    cout << "Enter board size (rows cols): ";
    cin >> H >> W;
    cout << "Number of mines: ";
    cin >> mines;

    vector<vector<Cell>> board(H, vector<Cell>(W));
    // Place mines randomly
    vector<pair<int,int>> all;
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            all.emplace_back(y, x);
    random_shuffle(all.begin(), all.end());
    for (int i = 0; i < mines && i < (int)all.size(); ++i)
        board[all[i].first][all[i].second].mine = true;
    // Calculate adjacent mine counts
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (!board[y][x].mine)
                for (auto [ny, nx] : neighbors(y, x, H, W))
                    board[y][x].adjacent += board[ny][nx].mine ? 1 : 0;

    bool lost = false;
    int cellsToReveal = H * W - mines;
    int revealedCount = 0;

    while (true) {
        printBoard(board);
        cout << "Commands: \n";
        cout << "  r Y X   (reveal cell Y X)\n";
        cout << "  f Y X   (flag/unflag cell)\n";
        cout << "  q       (quit)\n";
        cout << "> ";
        string cmd;
        cin >> cmd;
        if (cmd == "q") {
            cout << "Goodbye!\n";
            break;
        }
        int y, x;
        if ((cmd == "r" || cmd == "f") && cin >> y >> x) {
            if (y < 0 || y >= H || x < 0 || x >= W) {
                cout << "Invalid coordinates.\n";
                continue;
            }
            if (cmd == "r") {
                if (board[y][x].flagged || board[y][x].revealed) continue;
                if (board[y][x].mine) {
                    lost = true;
                    break;
                }
                reveal(board, y, x);
            } else if (cmd == "f") {
                board[y][x].flagged = !board[y][x].flagged;
            }
        }
        revealedCount = 0;
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                revealedCount += board[i][j].revealed;
        if (revealedCount == cellsToReveal) break;
    }

    printBoard(board, true);
    if (lost)
        cout << "BOOM! You hit a mine. Game over.\n";
    else if (revealedCount == H * W - mines)
        cout << "Congratulations! You cleared the board!\n";
    return 0;
}
