#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

// Simple hash (for demo only, don't use in real security applications)
string simple_hash(const string &password) {
    unsigned int hash = 0;
    for (char c : password) hash = (hash * 131) + c;
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// Load user database from file
void load_users(map<string, string> &users) {
    users.clear();
    ifstream fin("users.dat");
    string line;
    while (getline(fin, line)) {
        size_t pos = line.find(',');
        if (pos == string::npos) continue;
        string user = line.substr(0, pos);
        string pass = line.substr(pos + 1);
        users[user] = pass;
    }
}

// Save user database to file
void save_users(const map<string, string> &users) {
    ofstream fout("users.dat");
    for (auto &kv : users)
        fout << kv.first << "," << kv.second << endl;
}

// Register new user
void register_user(map<string, string> &users) {
    string user, pass;
    cout << "Choose username: "; cin >> user;
    if (users.count(user)) {
        cout << "Username already exists.\n";
        return;
    }
    cout << "Choose password: "; cin >> pass;
    users[user] = simple_hash(pass);
    save_users(users);
    cout << "Registration successful.\n";
}

// Login
bool login(const map<string, string> &users) {
    string user, pass;
    cout << "Username: "; cin >> user;
    cout << "Password: "; cin >> pass;
    auto it = users.find(user);
    if (it != users.end() && it->second == simple_hash(pass)) {
        cout << "Login successful. Welcome, " << user << "!\n";
        return true;
    }
    cout << "Login failed.\n";
    return false;
}

// Change password
void change_password(map<string, string> &users) {
    string user, old_pass, new_pass;
    cout << "Username: "; cin >> user;
    cout << "Current password: "; cin >> old_pass;
    if (users.count(user) && users[user] == simple_hash(old_pass)) {
        cout << "New password: "; cin >> new_pass;
        users[user] = simple_hash(new_pass);
        save_users(users);
        cout << "Password updated.\n";
    } else {
        cout << "Invalid credentials.\n";
    }
}

void printMenu() {
    cout << "\n-- Security System --\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Change Password\n";
    cout << "4. Exit\n";
    cout << "Choose: ";
}

int main() {
    map<string, string> users;
    load_users(users);

    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        if (choice == 1) register_user(users);
        else if (choice == 2) login(users);
        else if (choice == 3) change_password(users);
        else if (choice == 4) { cout << "Thank you! Program ended.\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
