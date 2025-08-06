/*
  Sorting Algorithm Visualizer (main.cpp)
  ------------------------------------------
  - Sorts a random array using multiple algorithms (Bubble, Insertion, QuickSort)
  - Visualizes each sort step with ASCII bars and highlights comparisons/swaps
  - Step-by-step animation in terminal, all C++17 single file
  - Press ENTER to proceed to next step, or set auto mode

  Concepts: sorting, visualization, I/O, modular design
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <string>
#include <functional>
using namespace std;

// --- Visualization ---
void showArray(const vector<int>& arr, int hi = -1, int hj = -1, const string& legend = "") {
    cout << "\n";
    if (!legend.empty()) cout << "--- " << legend << " ---\n";
    int maxVal = *max_element(arr.begin(), arr.end());
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << setw(2) << i << " | ";
        int bars = arr[i] * 60 / maxVal;
        for (int b = 0; b < bars; ++b)
            cout << (i == hi || i == hj ? '*' : '='); // highlight swapped indices
        cout << " (" << arr[i] << ")";
        if (i == hi || i == hj) cout << " <";
        cout << "\n";
    }
    cout << endl;
}

// --- Algorithms ---
void bubbleSort(vector<int> arr, bool autoMode = false) {
    cout << "\n--- Bubble Sort Visualization ---\n";
    showArray(arr, -1, -1, "Start");
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-1-i; ++j) {
            showArray(arr, j, j+1, "Compare & Swap");
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                showArray(arr, j, j+1, "After Swap");
            }
            if (!autoMode) { cout << "Press ENTER to continue..."; cin.get(); }
            else this_thread::sleep_for(chrono::milliseconds(220));
        }
    }
    showArray(arr, -1, -1, "Sorted");
    cout << "Bubble Sort Complete!\n";
}

void insertionSort(vector<int> arr, bool autoMode = false) {
    cout << "\n--- Insertion Sort Visualization ---\n";
    showArray(arr, -1, -1, "Start");
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i], j = i-1;
        while (j >= 0 && arr[j] > key) {
            showArray(arr, j+1, j, "Shift Right");
            arr[j+1] = arr[j];
            --j;
            if (!autoMode) { cout << "Press ENTER..."; cin.get(); }
            else this_thread::sleep_for(chrono::milliseconds(190));
        }
        arr[j+1] = key;
        showArray(arr, j+1, -1, "Insert");
        if (!autoMode) { cout << "Press ENTER..."; cin.get(); }
        else this_thread::sleep_for(chrono::milliseconds(190));
    }
    showArray(arr, -1, -1, "Sorted");
    cout << "Insertion Sort Complete!\n";
}

void quickSortRec(vector<int>& arr, int lo, int hi, bool autoMode) {
    if (lo >= hi) return;
    int pivot = arr[hi], i = lo;
    for (int j = lo; j < hi; ++j) {
        showArray(arr, j, hi, "Pivot Comparison");
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            showArray(arr, i, j, "After Swap");
            ++i;
        }
        if (!autoMode) { cout << "Press ENTER..."; cin.get(); }
        else this_thread::sleep_for(chrono::milliseconds(170));
    }
    swap(arr[i], arr[hi]);
    showArray(arr, i, hi, "Pivot Placement");
    if (!autoMode) { cout << "Press ENTER..."; cin.get(); }
    else this_thread::sleep_for(chrono::milliseconds(170));
    quickSortRec(arr, lo, i-1, autoMode);
    quickSortRec(arr, i+1, hi, autoMode);
}
void quickSort(vector<int> arr, bool autoMode = false) {
    cout << "\n--- QuickSort Visualization ---\n";
    showArray(arr, -1, -1, "Start");
    quickSortRec(arr, 0, arr.size()-1, autoMode);
    showArray(arr, -1, -1, "Sorted");
    cout << "QuickSort Complete!\n";
}

// --- Main UI ---
int main() {
    cout << "=== Sorting Algorithm Visualizer (Text-Based, C++) ===\n";
    int N = 14;
    vector<int> arr(N);
    iota(arr.begin(), arr.end(), 9); // Ensure positive, nice bars
    // Shuffle
    mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
    shuffle(arr.begin(), arr.end(), rng);

    cout << "Array size: " << arr.size() << "\n";
    cout << "Select algorithm:\n  1=BubbleSort\n  2=InsertionSort\n  3=QuickSort\n> ";
    int algo; cin >> algo;
    cout << "Step-by-step (0) or auto mode animation (1)? ";
    int autoModeInt; cin >> autoModeInt;
    bool autoMode = (autoModeInt == 1);
    cin.ignore(); // Clear newline

    switch(algo) {
        case 1: bubbleSort(arr, autoMode); break;
        case 2: insertionSort(arr, autoMode); break;
        case 3: quickSort(arr, autoMode); break;
        default: cout << "Invalid option.\n";
    }

    cout << "Visualization done.\n";
    return 0;
}
