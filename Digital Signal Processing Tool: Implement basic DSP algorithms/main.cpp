#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Print signal
void printSignal(const vector<double>& sig) {
    cout << "\nSignal: [ ";
    for (double x : sig) cout << fixed << setprecision(2) << x << " ";
    cout << "]\n";
}

// Amplify/scale signal
vector<double> scale(const vector<double>& sig, double k) {
    vector<double> out(sig.size());
    for (size_t i = 0; i < sig.size(); ++i)
        out[i] = sig[i] * k;
    return out;
}

// Time-reverse signal
vector<double> reverse(const vector<double>& sig) {
    vector<double> out(sig.rbegin(), sig.rend());
    return out;
}

// Moving average filter
vector<double> movingAverage(const vector<double>& sig, int window) {
    vector<double> out(sig.size());
    for (size_t i = 0; i < sig.size(); ++i) {
        double sum = 0;
        int cnt = 0;
        for (int j = int(i)-window/2; j <= int(i)+window/2; ++j)
            if (j >= 0 && j < int(sig.size())) {
                sum += sig[j];
                cnt++;
            }
        out[i] = cnt ? sum / cnt : sig[i];
    }
    return out;
}

void printMenu() {
    cout << "\n--- Digital Signal Processing Tool ---\n";
    cout << "1. Enter Signal\n";
    cout << "2. Amplify (Scale) Signal\n";
    cout << "3. Time-Reverse Signal\n";
    cout << "4. Moving Average Filter\n";
    cout << "5. Show Current Signal\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

int main() {
    vector<double> signal;
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        if (choice == 1) {
            signal.clear();
            int n;
            cout << "Enter signal length: ";
            cin >> n;
            signal.resize(n);
            cout << "Enter signal samples:\n";
            for (int i = 0; i < n; ++i) cin >> signal[i];
            cout << "Signal loaded.\n";
        } else if (choice == 2) {
            if (signal.empty()) { cout << "No signal loaded.\n"; continue; }
            double k;
            cout << "Enter scaling factor: ";
            cin >> k;
            signal = scale(signal, k);
            cout << "Signal scaled.\n";
        } else if (choice == 3) {
            if (signal.empty()) { cout << "No signal loaded.\n"; continue; }
            signal = reverse(signal);
            cout << "Signal time-reversed.\n";
        } else if (choice == 4) {
            if (signal.empty()) { cout << "No signal loaded.\n"; continue; }
            int w;
            cout << "Enter moving average window size (odd integer): ";
            cin >> w;
            if (w < 1 || w > int(signal.size()) || w%2==0) { cout << "Invalid window.\n"; continue; }
            signal = movingAverage(signal, w);
            cout << "Filter applied.\n";
        } else if (choice == 5) {
            if (signal.empty()) cout << "No signal loaded.\n";
            else printSignal(signal);
        } else if (choice == 6) {
            cout << "Goodbye!\n"; break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
