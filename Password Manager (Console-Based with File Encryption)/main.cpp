#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Entry {
    string website;
    string username;
    string password;
};

const string VAULT_FILE = "vault.dat";

// Simple XOR encryption for demonstration
string xorEncrypt(const string& data, const string& key) {
    string enc = data;
    for (size_t i = 0; i < data.size(); ++i)
        enc[i] ^= key[i % key.size()];
    return enc;
}

void saveVault(const vector<Entry>& entries, const string& key) {
    ofstream fout(VAULT_FILE, ios::binary);
    for (const auto& e : entries) {
        string rec = e.website + "\t" + e.username + "\t" + e.password + "\n";
        string encrypted = xorEncrypt(rec, key);
        fout << encrypted;
    }
    fout.close();
}

void loadVault(vector<Entry>& entries, const string& key) {
    entries.clear();
    ifstream fin(VAULT_FILE, ios::binary);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        string dec = xorEncrypt(line, key);
        size_t t1 = dec.find('\t'), t2 = dec.find('\t', t1+1);
        if (t1 == string::npos || t2 == string::npos) continue;
        Entry e;
        e.website = dec.substr(0, t1);
        e.username = dec.substr(t1+1, t2-t1-1);
        e.password = dec.substr(t2+1);
        entries.push_back(e);
    }
    fin.close();
}

void printMenu() {
    cout << "\n--- Password Manager ---\n";
    cout << "1. Add Entry\n";
    cout << "2. View All Passwords\n";
    cout << "3. Search Entry\n";
    cout << "4. Delete Entry\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void showEntries(const vector<Entry>& entries) {
    if (entries.empty()) {
        cout << "No entries found.\n";
        return;
    }
    cout << left << setw(18) << "Website"
         << setw(18) << "Username"
         << setw(18) << "Password\n";
    cout << string(54, '-') << '\n';
    for (const auto& e : entries)
        cout << setw(18) << e.website << setw(18) << e.username << setw(18) << e.password << '\n';
}

void searchEntry(const vector<Entry>& entries) {
    string key;
    cout << "Enter website or username: "; getline(cin, key);
    vector<Entry> found;
    for (const auto& e : entries)
        if (e.website.find(key) != string::npos ||
            e.username.find(key) != string::npos)
            found.push_back(e);
    showEntries(found);
}

int main() {
    vector<Entry> entries;
    string masterKey;
    cout << "Enter vault password: ";
    getline(cin, masterKey);

    loadVault(entries, masterKey);
    int choice;
    while (true) {
        printMenu();
        cin >> choice; cin.ignore();
        if (choice == 1) {
            Entry e;
            cout << "Website: "; getline(cin, e.website);
            cout << "Username: "; getline(cin, e.username);
            cout << "Password: "; getline(cin, e.password);
            entries.push_back(e);
            saveVault(entries, masterKey);
            cout << "Entry added!\n";
        } else if (choice == 2) {
            showEntries(entries);
        } else if (choice == 3) {
            searchEntry(entries);
        } else if (choice == 4) {
            showEntries(entries);
            if (entries.empty()) continue;
            cout << "Enter website to delete: ";
            string w;
            getline(cin, w);
            auto it = remove_if(entries.begin(), entries.end(),
                                [&](const Entry& e) { return e.website == w; });
            if (it != entries.end()) {
                entries.erase(it, entries.end());
                saveVault(entries, masterKey);
                cout << "Entry deleted.\n";
            } else {
                cout << "No entry found for that website.\n";
            }
        } else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        } else {
            cout << "Invalid option. Enter 1-5.\n";
        }
    }
    return 0;
}
