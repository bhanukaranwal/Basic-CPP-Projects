#include <iostream>
#include <vector>
#include <set>
using namespace std;

// Checks if all numbers 1-4 appear exactly once in a vector
bool isValidSet(const vector<int>& vec) {
    set<int> nums(vec.begin(), vec.end());
    return nums.size() == 4 && *nums.begin() == 1 && *nums.rbegin() == 4;
}

int main() {
    vector<vector<int>> grid(4, vector<int>(4));
    cout << "--- Mini Sudoku Validator (4x4) ---\n";
    cout << "Enter your 4x4 grid (numbers 1-4, row by row):\n";
    for (int i = 0; i < 4; ++i) {
        cout << "Row " << i+1 << ": ";
        for (int j = 0; j < 4; ++j)
            cin >> grid[i][j];
    }

    // Check rows
    for (int i = 0; i < 4; ++i) {
        if (!isValidSet(grid[i])) {
            cout << "Row " << (i + 1) << " is invalid.\n";
            cout << "Result: Invalid Sudoku.\n";
            return 0;
        }
    }

    // Check columns
    for (int j = 0; j < 4; ++j) {
        vector<int> col;
        for (int i = 0; i < 4; ++i)
            col.push_back(grid[i][j]);
        if (!isValidSet(col)) {
            cout << "Column " << (j + 1) << " is invalid.\n";
            cout << "Result: Invalid Sudoku.\n";
            return 0;
        }
    }

    // Check 2x2 boxes
    for (int boxRow = 0; boxRow < 4; boxRow += 2)
        for (int boxCol = 0; boxCol < 4; boxCol += 2) {
            vector<int> box;
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    box.push_back(grid[boxRow + i][boxCol + j]);
            if (!isValidSet(box)) {
                cout << "Box starting at (" << boxRow+1 << "," << boxCol+1 << ") is invalid.\n";
                cout << "Result: Invalid Sudoku.\n";
                return 0;
            }
        }

    cout << "Result: Valid Mini Sudoku Solution!\n";
    return 0;
}
