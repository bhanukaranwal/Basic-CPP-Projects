#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Entry {
    string date;   // e.g. "2025-08-06"
    string text;
};

void printMenu() {
    cout << "\n--- Personal Diary ---\n";
    cout << "1. Add New Entry\n";
    cout << "2. View All Entries\n";
    cout << "3. Search by Date\n";
    cout << "4. Search by Keyword\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void showEntries(const vector<Entry>& entries) {
    if (entries.empty()) {
        cout << "No entries yet.\n";
        return;
    }
    cout << "\n--- Diary Entries ---\n";
    for (const auto& e : entries) {
        cout << "[" << e.date << "]\n" << e.text << "\n---\n";
    }
}

int main() {
    vector<Entry> diary;
    int choice;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();
        if (choice == 1) { // Add entry
            Entry e;
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, e.date);
            cout << "Write your diary entry:\n";
            getline(cin, e.text);
            diary.push_back(e);
            cout << "Entry saved!\n";
        }
        else if (choice == 2) { // View all
            showEntries(diary);
        }
        else if (choice == 3) { // Search by date
            cout << "Enter date (YYYY-MM-DD): ";
            string d; getline(cin, d);
            bool found = false;
            for (const auto& e : diary) {
                if (e.date == d) {
                    cout << "[" << e.date << "]\n" << e.text << "\n---\n";
                    found = true;
                }
            }
            if (!found) cout << "No entry on that date.\n";
        }
        else if (choice == 4) { // Search by keyword
            cout << "Enter keyword to search: ";
            string key; getline(cin, key);
            bool found = false;
            for (const auto& e : diary) {
                if (e.text.find(key) != string::npos) {
                    cout << "[" << e.date << "]\n" << e.text << "\n---\n";
                    found = true;
                }
            }
            if (!found) cout << "No entry containing that keyword.\n";
        }
        else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-5.\n";
        }
    }
    return 0;
}
