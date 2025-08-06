/*
  Expense Sharing via QR Generator (main.cpp)
  -----------------------------------------------
  - Input expenses (who paid, total, split among whom)
  - Calculates minimal settlements ("who pays whom how much")
  - Generates an ASCII "QR" (code text block) for each payee that can be copied, scanned, or shared
  - 100% standalone C++17, file-persistent, easily adaptable to real QR APIs

  Concepts: matrix balancing, optimal settlements, string encoding/QR, CLI UX, file I/O
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// -----------------------------------
// Expense logic: balances and splits
struct Expense {
    string payer;
    double amount;
    vector<string> participants; // those sharing cost
    string description;
    Expense(const string& p, double amt, const vector<string>& parts, const string& desc="")
        : payer(p), amount(amt), participants(parts), description(desc) {}
};

struct Settle {
    string from, to;
    double amount;
    Settle(const string& f, const string& t, double a): from(f), to(t), amount(a) {}
};

vector<Settle> computeSettlements(const vector<string>& friends, const vector<Expense>& expenses) {
    map<string, double> bal; // net owed
    for (const auto& n : friends) bal[n] = 0.0;

    for (const auto& e : expenses) {
        double share = e.amount / e.participants.size();
        for (const auto& p : e.participants)
            bal[p] -= share;
        bal[e.payer] += e.amount;
    }

    // Settle debts greedily
    vector<pair<string, double>> owes, gets;
    for (const auto& kv : bal) {
        if (kv.second > 1e-5) gets.emplace_back(kv.first, kv.second);
        else if (kv.second < -1e-5) owes.emplace_back(kv.first, -kv.second);
    }
    sort(gets.begin(), gets.end(), [](auto& a, auto& b) { return a.second > b.second; });
    sort(owes.begin(), owes.end(), [](auto& a, auto& b) { return a.second > b.second; });

    vector<Settle> trans;
    size_t o = 0, g = 0;
    while(o < owes.size() && g < gets.size()) {
        double pay = min(owes[o].second, gets[g].second);
        trans.emplace_back(owes[o].first, gets[g].first, pay);
        owes[o].second -= pay;
        gets[g].second -= pay;
        if (owes[o].second < 1e-5) ++o;
        if (gets[g].second < 1e-5) ++g;
    }
    return trans;
}

// -----------------------------------
// "QR" code as ASCII block for payment details
void printQR(const string& from, const string& to, double amt, const string& note) {
    stringstream ss;
    ss << "PAY|" << from << "->" << to << "|" << fixed << setprecision(2) << amt << "|" << note;
    string code = ss.str();
    cout << "\n--- QR PAYMENT CODE ---\n";
    cout << "+-------------------------+\n";
    for (size_t i = 0; i < code.size(); i += 20) {
        cout << "| " << left << setw(20) << code.substr(i, 20) << " |\n";
    }
    cout << "+-------------------------+\n";
    cout << "(Copy this code or use with a payment QR generator.)\n";
}

int main() {
    cout << "=== Expense Sharing via QR ===\n";
    cout << "Enters expenses, splits, and generates copyable payment QR code blocks per transaction.\n";
    vector<string> pals;
    vector<Expense> exps;
    cout << "Group member names (comma separated): ";
    string names; getline(cin, names);
    stringstream ss(names); string n;
    while (getline(ss, n, ',')) {
        n.erase(remove_if(n.begin(), n.end(), ::isspace), n.end());
        if(!n.empty()) pals.push_back(n);
    }
    if(pals.size() < 2) { cout << "Need at least 2 people.\n"; return 1; }

    while(true) {
        cout << "\nCommands: add, view, split, save, load, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd == "add") {
            cout << "Who paid? "; string who; cin >> who;
            if(find(pals.begin(), pals.end(), who)==pals.end()) { cout << "Invalid name.\n"; continue; }
            cout << "Amount: "; double amt; cin >> amt;
            cout << "Description (optional): "; cin.ignore(); string desc; getline(cin, desc);
            cout << "Who shares? (comma separated or 'all'): "; string part; getline(cin, part);
            vector<string> parts;
            if(part == "all") parts = pals;
            else {
                stringstream sp(part); string pp;
                while(getline(sp, pp, ',')) {
                    pp.erase(remove_if(pp.begin(), pp.end(), ::isspace), pp.end());
                    if(!pp.empty()) parts.push_back(pp);
                }
            }
            if(parts.empty()) { cout << "No participants!\n"; continue; }
            exps.emplace_back(who, amt, parts, desc);
            cout << "Expense added.\n";
        } else if(cmd == "view") {
            cout << "\n--- Expenses ---\n";
            for(size_t i=0;i<exps.size();++i) {
                cout << i+1 << ". " << exps[i].payer << " paid $" << fixed << setprecision(2) << exps[i].amount
                     << " for ";
                for(const auto& p: exps[i].participants) cout << p << " ";
                cout << (exps[i].description.size() ? (" ("+exps[i].description+")") : "") << "\n";
            }
        } else if(cmd == "split") {
            auto settlements = computeSettlements(pals, exps);
            if(settlements.empty()) { cout << "All settled!\n"; continue; }
            cout << "\n--- Settlements Required ---\n";
            for(size_t i=0;i<settlements.size();++i) {
                cout << i+1 << ". " << settlements[i].from << " → " << settlements[i].to
                     << ": $" << fixed << setprecision(2) << settlements[i].amount << "\n";
                printQR(settlements[i].from, settlements[i].to, settlements[i].amount, "GrpSplit");
            }
        } else if(cmd == "save") {
            ofstream f("expshare.txt", ios::trunc);
            for (const auto& e : exps) {
                f << e.payer << "|" << e.amount << "|" << e.description << "|";
                for (auto& p : e.participants) f << p << ",";
                f << "\n";
            }
            cout << "Saved to expshare.txt.\n";
        } else if(cmd == "load") {
            ifstream f("expshare.txt"); exps.clear();
            string ln;
            while(getline(f,ln)) {
                stringstream s(ln); string payer, amount, desc, part; vector<string> parts;
                getline(s, payer, '|'); getline(s, amount, '|'); getline(s, desc, '|'); getline(s, part);
                stringstream sp(part); string pp;
                while(getline(sp,pp,',')) if(!pp.empty()) parts.push_back(pp);
                exps.emplace_back(payer, stod(amount), parts, desc);
            }
            cout << "Loaded from expshare.txt.\n";
        } else if(cmd == "quit") {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
