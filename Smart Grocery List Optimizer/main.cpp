/*
  Smart Grocery List Optimizer (main.cpp)
  -------------------------------------------
  - Add, edit, and review grocery items, with quantity, frequency (recurring/one-time), and cost.
  - Auto-builds master list for the next shopping run, factoring in recurring items.
  - Tracks historical spending and lets you mark items as “bought.”
  - Offers smart suggestions for healthy alternatives.
  - All C++17, file-based persistence in "grocery_list.txt", no dependencies.

  Concepts: file I/O, map, list filtering, summaries, smart prompts
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <sstream>
#include <ctime>
using namespace std;

struct Item {
    string name;
    int qty;
    string freq; // "always", "weekly", "monthly", "once"
    double price;
    bool bought;
    Item(const string& n, int q, const string& f, double p, bool b = false)
        : name(n), qty(q), freq(f), price(p), bought(b) {}
};

vector<Item> loadList(const string& fname) {
    vector<Item> db;
    ifstream fin(fname);
    string line;
    while (getline(fin, line)) {
        istringstream ss(line);
        string name, freq, boughts;
        int qty; double price; bool bought;
        ss >> quoted(name) >> qty >> freq >> price >> boughts;
        bought = (boughts == "bought");
        db.emplace_back(name, qty, freq, price, bought);
    }
    return db;
}
void saveList(const string& fname, const vector<Item>& db) {
    ofstream fout(fname, ios::trunc);
    for (auto& it : db)
        fout << quoted(it.name) << " " << it.qty << " "
             << it.freq << " " << it.price << " "
             << (it.bought ? "bought" : "not") << "\n";
}

map<string, string> healthySwap = {
    {"chips", "Roasted chickpeas, fresh fruit"},
    {"cola", "Sparkling water, unsweetened juice"},
    {"white bread", "Whole-grain bread"},
    {"sugar", "Honey, stevia"},
    {"butter", "Olive oil, avocado"},
    {"chocolate", "Dark chocolate, dried fruits"},
    {"cream", "Greek yogurt"},
    {"regular milk", "Almond/oat milk"}
};

void printItem(const Item& it, int idx = -1) {
    cout << setw(2) << (idx >= 0 ? to_string(idx + 1) : "") << " "
         << setw(18) << left << it.name
         << "x" << it.qty << ", " << setw(8) << it.freq
         << " @ " << fixed << setprecision(2) << it.price
         << (it.bought ? " [BOUGHT] " : "") << "\n";
}

void showList(const vector<Item>& db, bool all = true) {
    cout << "\n--- Grocery List ---\n";
    int idx = 0, count = 0;
    for (const auto& it : db) {
        if (all || !it.bought)
            printItem(it, idx);
        ++idx; ++count;
    }
    cout << "Total items: " << count << "\n";
}

void buildNextList(vector<Item>& db, vector<Item>& out) {
    out.clear();
    for (auto& it : db)
        if (!it.bought && (it.freq == "weekly" || it.freq == "always" || it.freq == "once"))
            out.push_back(it);
}

void markBought(vector<Item>& db) {
    showList(db, false);
    cout << "Mark which item # as bought (0 to skip, -1 to finish): ";
    int idx;
    while (cin >> idx && idx != -1) {
        if (idx == 0) break;
        if (idx > 0 && idx <= db.size() && !db[idx - 1].bought) {
            db[idx - 1].bought = true;
            cout << db[idx - 1].name << " marked as bought.\n";
        }
        cout << "Next #: ";
    }
}

void showSpent(const vector<Item>& db) {
    double spent = 0.0, planned = 0.0;
    int boughtct = 0, allct = 0;
    for (const auto& it : db) {
        planned += it.price * it.qty;
        if (it.bought) {
            spent += it.price * it.qty;
            ++boughtct;
        }
        ++allct;
    }
    cout << fixed << setprecision(2)
         << "\nSpent this run: $" << spent
         << " (Bought " << boughtct << " of " << allct << " items).\n"
         << "Total planned: $" << planned << "\n";
}

void suggestHealthier(const vector<Item>& db) {
    cout << "\nHealthy alternatives:\n";
    for (const auto& it : db) {
        auto f = healthySwap.find(it.name);
        if (f != healthySwap.end() && !it.bought)
            cout << "For \"" << it.name << "\": try " << f->second << "\n";
    }
}

int main() {
    cout << "=== Smart Grocery List Optimizer ===\n";
    string fname = "grocery_list.txt";
    vector<Item> db = loadList(fname);

    while (true) {
        cout << "\nCommands: add, list, build, bought, spent, suggest, reset, quit\n> ";
        string cmd; cin >> cmd;
        if (cmd == "add") {
            cout << "Item name: "; string name; cin.ignore(); getline(cin, name);
            cout << "Quantity: "; int qty; cin >> qty;
            cout << "Frequency (always/weekly/monthly/once): "; string freq; cin >> freq;
            cout << "Price per unit: $"; double prc; cin >> prc;
            db.emplace_back(name, qty, freq, prc, false);
            saveList(fname, db);
            cout << name << " added.\n";
        } else if (cmd == "list") {
            showList(db);
        } else if (cmd == "build") {
            vector<Item> tobuy;
            buildNextList(db, tobuy);
            cout << "\nGrocery master list for next trip:\n";
            double total = 0.0;
            for (size_t i = 0; i < tobuy.size(); ++i) {
                printItem(tobuy[i], int(i));
                total += tobuy[i].price * tobuy[i].qty;
            }
            cout << "Projected cost: $" << total << "\n";
        } else if (cmd == "bought") {
            markBought(db);
            saveList(fname, db);
        } else if (cmd == "spent") {
            showSpent(db);
        } else if (cmd == "suggest") {
            suggestHealthier(db);
        } else if (cmd == "reset") {
            for (auto& it : db)
                if (it.freq != "once")
                    it.bought = false;
            db.erase(remove_if(db.begin(), db.end(),
                               [](const Item& i) { return i.freq == "once"; }),
                     db.end());
            saveList(fname, db);
            cout << "Reset done (one-time items cleared, others reset to unbought).\n";
        } else if (cmd == "quit") {
            saveList(fname, db);
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
