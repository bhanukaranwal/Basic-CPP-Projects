#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Expense {
    string date;      // e.g., "2025-08-06"
    string category;  // e.g., "Food", "Transport"
    double amount;
};

void printMenu() {
    cout << "\n--- Personal Expense Tracker ---\n";
    cout << "1. Add Expense\n";
    cout << "2. View All Expenses\n";
    cout << "3. View Total by Category\n";
    cout << "4. View Total Spent\n";
    cout << "5. Search by Category\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

void showExpenses(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        return;
    }
    cout << left << setw(12) << "Date"
         << setw(14) << "Category"
         << setw(10) << "Amount" << endl;
    cout << string(36, '-') << endl;
    for (const auto& e : expenses)
        cout << setw(12) << e.date
             << setw(14) << e.category
             << "$" << fixed << setprecision(2) << e.amount << endl;
}

void viewByCategory(const vector<Expense>& expenses) {
    if (expenses.empty()) { cout << "No expenses.\n"; return; }
    vector<string> cats;
    for (const auto& e : expenses) cats.push_back(e.category);
    sort(cats.begin(), cats.end());
    cats.erase(unique(cats.begin(), cats.end()), cats.end());
    for (const string& cat : cats) {
        double sum = 0;
        for (const auto& e : expenses)
            if (e.category == cat) sum += e.amount;
        cout << setw(14) << cat << ": $" << fixed << setprecision(2) << sum << endl;
    }
}

void searchByCategory(const vector<Expense>& expenses) {
    string cat;
    cout << "Enter category to search: ";
    getline(cin, cat);
    vector<Expense> found;
    for (const auto& e : expenses)
        if (e.category == cat)
            found.push_back(e);
    showExpenses(found);
}

int main() {
    vector<Expense> expenses;
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            Expense e;
            cout << "Enter date (YYYY-MM-DD): "; getline(cin, e.date);
            cout << "Enter category: "; getline(cin, e.category);
            cout << "Amount: $"; cin >> e.amount; cin.ignore();
            expenses.push_back(e);
            cout << "Expense added!\n";
        }
        else if (choice == 2) {
            showExpenses(expenses);
        }
        else if (choice == 3) {
            viewByCategory(expenses);
        }
        else if (choice == 4) {
            double sum = 0;
            for (const auto& e : expenses) sum += e.amount;
            cout << "Total spent: $" << fixed << setprecision(2) << sum << endl;
        }
        else if (choice == 5) {
            searchByCategory(expenses);
        }
        else if (choice == 6) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid choice. Enter 1-6.\n";
        }
    }
    return 0;
}
