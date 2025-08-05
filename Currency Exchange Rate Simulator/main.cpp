#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Rate {
    string from;
    string to;
    double value;
};

void printCurrencies(const vector<string>& currs) {
    cout << "Available Currencies:\n";
    for (size_t i = 0; i < currs.size(); ++i)
        cout << "  " << (i+1) << ". " << currs[i] << endl;
}

void printRates(const map<pair<string,string>, double>& rates, const vector<string>& currs) {
    cout << "\n--- Current Exchange Rates ---\n";
    cout << fixed << setprecision(4);
    for (size_t i = 0; i < currs.size(); ++i) {
        for (size_t j = 0; j < currs.size(); ++j) {
            if (i != j) {
                cout << currs[i] << " -> " << currs[j]
                     << " : " << rates.at({currs[i], currs[j]}) << endl;
            }
        }
    }
}

void fluctuate(map<pair<string,string>, double>& rates, const vector<string>& currs) {
    for (size_t i = 0; i < currs.size(); ++i) {
        for (size_t j = 0; j < currs.size(); ++j) {
            if (i != j) {
                auto p = make_pair(currs[i], currs[j]);
                double factor = 0.98 + (rand() % 5) * 0.01; // ~[-2%, +2%]
                rates[p] *= factor;
                if (rates[p] < 0.001) rates[p] = 0.001;
            }
        }
    }
}

int main() {
    srand(time(0));
    vector<string> currs = {"USD", "EUR", "INR", "JPY"};
    map<pair<string,string>, double> rates;
    // Initialize rates
    rates[{"USD","EUR"}] = 0.92;
    rates[{"EUR","USD"}] = 1.09;
    rates[{"USD","INR"}] = 83.1;
    rates[{"INR","USD"}] = 0.012;
    rates[{"USD","JPY"}] = 156.4;
    rates[{"JPY","USD"}] = 0.00639;
    rates[{"EUR","INR"}] = 90.1;
    rates[{"INR","EUR"}] = 0.0111;
    rates[{"EUR","JPY"}] = 170.0;
    rates[{"JPY","EUR"}] = 0.0059;
    rates[{"INR","JPY"}] = 1.82;
    rates[{"JPY","INR"}] = 0.55;

    int choice;
    while (true) {
        cout << "\n--- Currency Exchange Rate Simulator ---\n";
        cout << "1. View Available Currencies\n";
        cout << "2. View Current Exchange Rates\n";
        cout << "3. Convert Currency\n";
        cout << "4. Simulate New Day (Fluctuate Rates)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (choice == 1) {
            printCurrencies(currs);
        } else if (choice == 2) {
            printRates(rates, currs);
        } else if (choice == 3) {
            printCurrencies(currs);
            cout << "Convert FROM (number): ";
            int from, to; double amount;
            cin >> from;
            if (from < 1 || from > (int)currs.size()) { cout << "Invalid.\n"; continue; }
            cout << "Convert TO (number): ";
            cin >> to;
            if (to < 1 || to > (int)currs.size() || from == to) { cout << "Invalid.\n"; continue; }
            cout << "Amount (" << currs[from-1] << "): ";
            cin >> amount;
            auto rate = rates[{currs[from-1], currs[to-1]}];
            cout << fixed << setprecision(4) << amount << " " << currs[from-1]
                 << " = " << (amount * rate) << " " << currs[to-1] << endl;
        } else if (choice == 4) {
            fluctuate(rates, currs);
            cout << "Simulated rate changes for a new day!\n";
        } else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
