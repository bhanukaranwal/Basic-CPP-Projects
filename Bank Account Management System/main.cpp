#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Account {
    int number;
    string name;
    double balance;
};

void printMenu() {
    cout << "\n--- Bank Account Management System ---\n";
    cout << "1. Create Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Display All Accounts\n";
    cout << "5. Delete Account\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

int findAccountIdx(const vector<Account>& accounts, int number) {
    for (size_t i = 0; i < accounts.size(); ++i)
        if (accounts[i].number == number) return i;
    return -1;
}

void showAccounts(const vector<Account>& accounts) {
    if (accounts.empty()) {
        cout << "No accounts found.\n";
        return;
    }
    cout << left << setw(10) << "Number" << setw(25) << "Name" << "Balance\n";
    cout << string(47, '-') << endl;
    for (const auto& acc : accounts)
        cout << setw(10) << acc.number << setw(25) << acc.name << "$" << fixed << setprecision(2) << acc.balance << endl;
}

int main() {
    vector<Account> accounts;
    int choice;
    int nextNumber = 1001;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) { // Create account
            Account acc;
            acc.number = nextNumber++;
            cout << "Enter account holder name: ";
            getline(cin, acc.name);
            cout << "Initial deposit: $";
            cin >> acc.balance; cin.ignore();
            accounts.push_back(acc);
            cout << "Account created! Number: " << acc.number << endl;
        }
        else if (choice == 2) { // Deposit
            showAccounts(accounts);
            cout << "Enter account number to deposit into: ";
            int num; cin >> num; cin.ignore();
            int idx = findAccountIdx(accounts, num);
            if (idx == -1) { cout << "Account not found.\n"; continue; }
            cout << "Deposit amount: $";
            double amt; cin >> amt; cin.ignore();
            if (amt <= 0) { cout << "Amount must be positive.\n"; continue; }
            accounts[idx].balance += amt;
            cout << "Deposit successful. New balance: $" << accounts[idx].balance << endl;
        }
        else if (choice == 3) { // Withdraw
            showAccounts(accounts);
            cout << "Enter account number to withdraw from: ";
            int num; cin >> num; cin.ignore();
            int idx = findAccountIdx(accounts, num);
            if (idx == -1) { cout << "Account not found.\n"; continue; }
            cout << "Withdrawal amount: $";
            double amt; cin >> amt; cin.ignore();
            if (amt <= 0 || amt > accounts[idx].balance) {
                cout << "Invalid amount.\n";
                continue;
            }
            accounts[idx].balance -= amt;
            cout << "Withdrawal successful. New balance: $" << accounts[idx].balance << endl;
        }
        else if (choice == 4) { // Display all
            showAccounts(accounts);
        }
        else if (choice == 5) { // Delete
            showAccounts(accounts);
            cout << "Enter account number to delete: ";
            int num; cin >> num; cin.ignore();
            int idx = findAccountIdx(accounts, num);
            if (idx != -1) {
                accounts.erase(accounts.begin() + idx);
                cout << "Account deleted.\n";
            } else {
                cout << "Account not found.\n";
            }
        }
        else if (choice == 6) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid option. Enter 1-6.\n";
        }
    }
    return 0;
}
