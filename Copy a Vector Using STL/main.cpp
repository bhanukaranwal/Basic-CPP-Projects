#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> vec1, vec2;
    int n, x;
    cout << "--- Copy a Vector Using STL ---\n";
    cout << "Enter the number of elements: ";
    cin >> n;
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; ++i) {
        cin >> x;
        vec1.push_back(x);
    }

    // Copy vec1 to vec2 using std::copy
    vec2.resize(vec1.size());
    copy(vec1.begin(), vec1.end(), vec2.begin());

    cout << "\nOriginal vector: ";
    for (int v : vec1) cout << v << " ";
    cout << "\nCopied vector:   ";
    for (int v : vec2) cout << v << " ";
    cout << endl;

    return 0;
}
