#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

const int WIDTH = 20, HEIGHT = 15;
const char LIVE = 'O', DEAD = '.';

void clearScreen() {
    // ANSI escape code to clear terminal (works in most), else just print newlines
    cout << "\033[2J\033[H";
}

void printGrid(const vector<vector<char>>& grid) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x)
            cout << grid[y][x] << ' ';
        cout << '\n';
    }
}

int countLiveNeighbours(const vector<vector<char>>& grid, int x, int y) {
    int cnt = 0;
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if (!(dx == 0 && dy == 0)) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
                    cnt += grid[ny][nx] == LIVE ? 1 : 0;
            }
    return cnt;
}

void randomizeGrid(vector<vector<char>> &grid) {
    for (auto& row : grid)
        for (char &cell : row)
            cell = (rand() % 4 == 0) ? LIVE : DEAD;  // ~25% chance of being alive
}

int main() {
    srand(time(0));
    vector<vector<char>> grid(HEIGHT, vector<char>(WIDTH, DEAD));
    vector<vector<char>> nextGrid = grid;

    randomizeGrid(grid);

    cout << "--- Conway's Game of Life ---\n";
    cout << "Press Ctrl+C to stop\n";
    int gen = 0;

    while (true) {
        clearScreen();
        cout << "Generation: " << ++gen << "\n";
        printGrid(grid);

        // Compute next generation
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int live = countLiveNeighbours(grid, x, y);
                if (grid[y][x] == LIVE) {
                    nextGrid[y][x] = (live == 2 || live == 3) ? LIVE : DEAD;
                } else {
                    nextGrid[y][x] = (live == 3) ? LIVE : DEAD;
                }
            }
        }
        grid.swap(nextGrid);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    return 0;
}
