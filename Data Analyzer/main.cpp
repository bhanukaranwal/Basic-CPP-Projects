#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
using namespace std;

vector<string> split(const string& line, char delim=',') {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

bool is_number(const string& s) {
    if (s.empty()) return false;
    char* endptr = nullptr;
    strtod(s.c_str(), &endptr);
    return *endptr == '\0';
}

int main() {
    cout << "--- CSV Data Analyzer ---\n";
    cout << "Enter CSV filename: ";
    string filename;
    getline(cin, filename);

    ifstream fin(filename);
    if (!fin) {
        cout << "Cannot open file.\n";
        return 1;
    }

    string line;
    vector<vector<string>> data;
    while (getline(fin, line)) {
        if (!line.empty())
            data.push_back(split(line));
    }
    fin.close();

    if (data.empty()) {
        cout << "CSV is empty.\n";
        return 0;
    }

    // Ask if header is present
    cout << "Does the first row contain column headers? (y/n): ";
    char y; cin >> y; cin.ignore();

    vector<string> headers;
    int startRow = 0;
    if (y=='y' || y=='Y') {
        headers = data[0];
        startRow = 1;
    } else {
        headers.resize(data[0].size());
        for (size_t i=0; i<headers.size(); ++i)
            headers[i] = "Col" + to_string(i+1);
    }

    size_t cols = headers.size();
    vector<bool> isNum(cols, true);
    // Detect numeric columns
    for (size_t c=0; c<cols; ++c) {
        for (size_t r=startRow; r<data.size(); ++r) {
            if (c >= data[r].size() || !is_number(data[r][c])) {
                isNum[c] = false;
                break;
            }
        }
    }

    // Analyze numeric columns
    cout << "\n--- Column Statistics ---\n";
    cout << left << setw(16) << "Column"
         << setw(8) << "Count"
         << setw(12) << "Sum"
         << setw(12) << "Mean"
         << setw(12) << "Min"
         << setw(12) << "Max"
         << endl;
    cout << string(60, '-') << endl;

    for (size_t c=0; c<cols; ++c) {
        if (!isNum[c]) continue;
        int count = 0;
        double sum = 0, minv = numeric_limits<double>::max(), maxv = numeric_limits<double>::lowest();
        for (size_t r=startRow; r<data.size(); ++r) {
            double val = stod(data[r][c]);
            count++;
            sum += val;
            minv = min(minv, val);
            maxv = max(maxv, val);
        }
        double mean = count ? sum/count : 0;
        cout << left << setw(16) << headers[c]
             << setw(8) << count
             << setw(12) << sum
             << setw(12) << mean
             << setw(12) << minv
             << setw(12) << maxv
             << endl;
    }

    cout << "\nDone. Only numeric columns are analyzed.\n";
    return 0;
}
