#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Cell {
    bool visited = false;
    bool top_wall = true, bot_wall = true, left_wall = true, right_wall = true;
    bool part_of_solution = false;
};

struct Pos { int x, y; };

// Maze size
const int MAZE_W = 10, MAZE_H = 10;

// Directions: up, right, down, left
const int DX[4] = {0, 1, 0, -1};
const int DY[4] = {-1, 0, 1, 0};

void printMaze(const vector<vector<Cell>> &maze) {
    int W = maze[0].size(), H = maze.size();

    // Top border
    for (int i = 0; i < W; ++i) cout << "+---";
    cout << "+\n";

    for (int y = 0; y < H; ++y) {
        // Left walls and cells
        for (int x = 0; x < W; ++x)
            cout << (maze[y][x].left_wall ? "|" : " ") 
                 << (maze[y][x].part_of_solution ? " * " : "   ");
        cout << "|\n";
        // Bottom walls
        for (int x = 0; x < W; ++x)
            cout << "+" << (maze[y][x].bot_wall ? "---" : "   ");
        cout << "+\n";
    }
}

// Maze generation using DFS
void generateMaze(vector<vector<Cell>> &maze, int cx, int cy) {
    maze[cy][cx].visited = true;
    vector<int> dirs = {0,1,2,3};
    random_shuffle(dirs.begin(), dirs.end());
    for (int dir : dirs) {
        int nx = cx + DX[dir], ny = cy + DY[dir];
        if (nx >= 0 && nx < MAZE_W && ny >= 0 && ny < MAZE_H && !maze[ny][nx].visited) {
            // Remove wall between maze[cy][cx] and maze[ny][nx]
            if (dir == 0) { maze[cy][cx].top_wall = maze[ny][nx].bot_wall = false; }
            if (dir == 1) { maze[cy][cx].right_wall = maze[ny][nx].left_wall = false; }
            if (dir == 2) { maze[cy][cx].bot_wall = maze[ny][nx].top_wall = false; }
            if (dir == 3) { maze[cy][cx].left_wall = maze[ny][nx].right_wall = false; }
            generateMaze(maze, nx, ny);
        }
    }
}

// Maze solving using DFS, marks solution path
bool solveMaze(vector<vector<Cell>>& maze, int x, int y) {
    if (x == MAZE_W-1 && y == MAZE_H-1) {
        maze[y][x].part_of_solution = true;
        return true;
    }
    maze[y][x].visited = true;
    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + DX[dir], ny = y + DY[dir];
        if (nx < 0 || nx >= MAZE_W || ny < 0 || ny >= MAZE_H) continue;
        // Check wall
        if ((dir == 0 && maze[y][x].top_wall)   ||
            (dir == 1 && maze[y][x].right_wall) ||
            (dir == 2 && maze[y][x].bot_wall)   ||
            (dir == 3 && maze[y][x].left_wall)) continue;
        if (!maze[ny][nx].visited && solveMaze(maze, nx, ny)) {
            maze[y][x].part_of_solution = true;
            return true;
        }
    }
    return false;
}

int main() {
    srand(time(0));
    vector<vector<Cell>> maze(MAZE_H, vector<Cell>(MAZE_W));
    // Generate maze
    generateMaze(maze, 0, 0);

    // Print initial maze
    cout << "\nGenerated Maze:\n";
    printMaze(maze);

    // Clear visited for solver
    for (int y = 0; y < MAZE_H; ++y)
        for (int x = 0; x < MAZE_W; ++x)
            maze[y][x].visited = false;

    // Solve and mark solution path
    solveMaze(maze, 0, 0);

    // Print solved maze (with solution path)
    cout << "\nMaze with Solution Path (*):\n";
    printMaze(maze);

    cout << "Start at (0,0), End at (" << MAZE_W-1 << "," << MAZE_H-1 << ")\n";
    return 0;
}
