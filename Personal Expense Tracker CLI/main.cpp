#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
using namespace std;

struct Expense {
    string category;
    double amount;
};

void printMenu() {
    cout << "\n--- Personal Expense Tracker ---\n";
    cout << "1. Add Expense\n";
    cout << "2. Show All Expenses\n";
    cout << "3. Show Expenses by Category\n";
    cout << "4. Show Summary\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<Expense> expenses;
    map<string, double> categoryTotals;
    int choice;

    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {
            cin.ignore();  // Clear newline from input buffer
            Expense e;
            cout << "Enter category (e.g. Food, Transport, Bills): ";
            getline(cin, e.category);
            cout << "Enter amount spent: ";
            if (!(cin >> e.amount) || e.amount < 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid amount. Expense not added.\n";
                continue;
            }
            expenses.push_back(e);
            categoryTotals[e.category] += e.amount;
            cout << "Expense added!\n";
        } 
        else if (choice == 2) {
            if (expenses.empty()) {
                cout << "No expenses recorded yet.\n";
                continue;
            }
            cout << "\nAll Expenses:\n";
            cout << left << setw(20) << "Category" << right << setw(10) << "Amount\n";
            cout << "---------------------------------\n";
            for (const auto& e : expenses) {
                cout << left << setw(20) << e.category << right << setw(10) << fixed << setprecision(2) << e.amount << "\n";
            }
        } 
        else if (choice == 3) {
            cin.ignore();
            cout << "Enter category to show expenses: ";
            string category;
            getline(cin, category);

            double sum = 0;
            bool found = false;
            cout << "\nExpenses in category '" << category << "':\n";
            cout << right << setw(10) << "Amount\n";
            cout << "----------------\n";
            for (const auto& e : expenses) {
                if (e.category == category) {
                    cout << right << setw(10) << fixed << setprecision(2) << e.amount << "\n";
                    sum += e.amount;
                    found = true;
                }
            }
            if (!found) {
                cout << "No expenses found in this category.\n";
            } else {
                cout << "Total: " << fixed << setprecision(2) << sum << "\n";
            }
        } 
        else if (choice == 4) {
            cout << "\nExpense Summary:\n";
            cout << left << setw(20) << "Category" << right << setw(10) << "Total\n";
            cout << "---------------------------------\n";
            double grandTotal = 0;
            for (const auto& pair : categoryTotals) {
                cout << left << setw(20) << pair.first << right << setw(10) << fixed << setprecision(2) << pair.second << "\n";
                grandTotal += pair.second;
            }
            cout << "---------------------------------\n";
            cout << left << setw(20) << "Grand Total" << right << setw(10) << fixed << setprecision(2) << grandTotal << "\n";
        } 
        else if (choice == 5) {
            cout << "Thank you for using Expense Tracker. Goodbye!\n";
            break;
        } 
        else {
            cout << "Invalid choice. Please select between 1-5.\n";
        }
    }

    return 0;
}
