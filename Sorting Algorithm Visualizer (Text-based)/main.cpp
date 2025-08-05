#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

void printArray(const vector<int>& arr) {
    for (int val : arr) {
        cout << setw(2) << val << " : ";
        for (int i = 0; i < val; ++i) cout << "|";
        cout << endl;
    }
    cout << endl;
}

void bubbleSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Bubble Sort Steps ---\n";
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
            printArray(arr);
        }
    }
    cout << "--- Sorted! ---\n";
    printArray(arr);
}

void insertionSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Insertion Sort Steps ---\n";
    for (int i = 1; i < n; ++i) {
        int key = arr[i], j = i-1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            --j;
            printArray(arr);
        }
        arr[j+1] = key;
        printArray(arr);
    }
    cout << "--- Sorted! ---\n";
    printArray(arr);
}

void selectionSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Selection Sort Steps ---\n";
    for (int i = 0; i < n-1; ++i) {
        int minIdx = i;
        for (int j = i+1; j < n; ++j)
            if (arr[j] < arr[minIdx]) minIdx = j;
        swap(arr[i], arr[minIdx]);
        printArray(arr);
    }
    cout << "--- Sorted! ---\n";
    printArray(arr);
}

int main() {
    srand(time(0));
    int size;
    cout << "Enter array size (5-20): ";
    cin >> size;
    if (size < 5) size = 5;
    if (size > 20) size = 20;
    vector<int> arr(size);
    for (int& v : arr) v = 1 + rand() % 15;

    cout << "\nGenerated Array:\n";
    printArray(arr);

    cout << "\nChoose algorithm:\n";
    cout << "1. Bubble Sort\n2. Insertion Sort\n3. Selection Sort\nChoice: ";
    int alg;
    cin >> alg;
    if (alg == 1) bubbleSort(arr);
    else if (alg == 2) insertionSort(arr);
    else if (alg == 3) selectionSort(arr);
    else cout << "Invalid choice.\n";

    return 0;
}
