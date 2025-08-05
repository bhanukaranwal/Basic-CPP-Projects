#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

void printMenu() {
    cout << "\n--- Contact Management System ---\n";
    cout << "1. Add Contact\n";
    cout << "2. View All Contacts\n";
    cout << "3. Update Contact\n";
    cout << "4. Delete Contact\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void showContacts(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "No contacts found.\n";
        return;
    }
    cout << left << setw(20) << "Name"
         << setw(15) << "Phone"
         << setw(25) << "Email" << endl;
    cout << string(60, '-') << endl;
    for (const auto& c : contacts) {
        cout << left << setw(20) << c.name
             << setw(15) << c.phone
             << setw(25) << c.email << endl;
    }
}

int findContactIdx(const vector<Contact>& contacts, const string& name) {
    for (size_t i = 0; i < contacts.size(); ++i)
        if (contacts[i].name == name) return i;
    return -1;
}

int main() {
    vector<Contact> contacts;
    int choice;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Contact c;
            cout << "Enter name: "; getline(cin, c.name);
            cout << "Enter phone: "; getline(cin, c.phone);
            cout << "Enter email: "; getline(cin, c.email);
            if (findContactIdx(contacts, c.name) == -1) {
                contacts.push_back(c);
                cout << "Contact added!\n";
            } else {
                cout << "This contact already exists.\n";
            }
        }
        else if (choice == 2) {
            showContacts(contacts);
        }
        else if (choice == 3) {
            string name;
            cout << "Enter name of contact to update: "; getline(cin, name);
            int idx = findContactIdx(contacts, name);
            if (idx == -1) { cout << "Contact not found.\n"; continue; }
            cout << "Update phone (current: " << contacts[idx].phone << "): ";
            getline(cin, contacts[idx].phone);
            cout << "Update email (current: " << contacts[idx].email << "): ";
            getline(cin, contacts[idx].email);
            cout << "Updated!\n";
        }
        else if (choice == 4) {
            string name;
            cout << "Enter name of contact to delete: "; getline(cin, name);
            int idx = findContactIdx(contacts, name);
            if (idx != -1) {
                contacts.erase(contacts.begin() + idx);
                cout << "Deleted.\n";
            } else {
                cout << "Contact not found.\n";
            }
        }
        else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
