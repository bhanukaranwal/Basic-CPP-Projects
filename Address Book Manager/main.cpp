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
    cout << "\n--- Address Book Manager ---\n";
    cout << "1. Add Contact\n";
    cout << "2. View All Contacts\n";
    cout << "3. Search Contact\n";
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
         << setw(30) << "Email\n";
    cout << string(65, '-') << endl;
    for (const auto& c : contacts)
        cout << setw(20) << c.name
             << setw(15) << c.phone
             << setw(30) << c.email << endl;
}

void searchContacts(const vector<Contact>& contacts) {
    string keyword;
    cout << "Enter name/phone/email to search: ";
    getline(cin, keyword);
    vector<Contact> found;
    for (const auto& c : contacts) {
        if (c.name.find(keyword) != string::npos ||
            c.phone.find(keyword) != string::npos ||
            c.email.find(keyword) != string::npos)
            found.push_back(c);
    }
    showContacts(found);
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
            contacts.push_back(c);
            cout << "Contact added!\n";
        }
        else if (choice == 2) {
            showContacts(contacts);
        }
        else if (choice == 3) {
            searchContacts(contacts);
        }
        else if (choice == 4) {
            showContacts(contacts);
            if (contacts.empty()) continue;
            cout << "Enter name of contact to delete: ";
            string name;
            getline(cin, name);
            auto it = remove_if(contacts.begin(), contacts.end(),
                                [&](const Contact& c) { return c.name == name; });
            if (it != contacts.end()) {
                contacts.erase(it, contacts.end());
                cout << "Contact deleted.\n";
            } else {
                cout << "No contact found with that name.\n";
            }
        }
        else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid option. Enter 1-5.\n";
        }
    }
    return 0;
}
