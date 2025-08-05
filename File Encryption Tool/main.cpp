#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// XORs file contents with the key derived from password
void xorFile(const string& infile, const string& outfile, const string& password) {
    ifstream fin(infile, ios::binary);
    ofstream fout(outfile, ios::binary);
    if (!fin || !fout) {
        cout << "File operation failed.\n";
        return;
    }
    size_t passlen = password.size();
    if (passlen == 0) {
        cout << "Password must not be empty.\n";
        return;
    }
    char ch;
    size_t idx = 0;
    while (fin.get(ch)) {
        ch ^= password[idx % passlen];
        fout.put(ch);
        ++idx;
    }
    cout << "Operation complete. Output file: " << outfile << endl;
}

void printMenu() {
    cout << "\n--- File Encryption/Decryption Tool ---\n";
    cout << "1. Encrypt File\n";
    cout << "2. Decrypt File\n";
    cout << "3. Exit\n";
    cout << "Choose: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();
        if (choice == 1 || choice == 2) {
            string infile, outfile, password;
            cout << "Enter input file name: "; getline(cin, infile);
            cout << "Enter output file name: "; getline(cin, outfile);
            cout << "Enter password/key: "; getline(cin, password);
            xorFile(infile, outfile, password);
        } else if (choice == 3) {
            cout << "Goodbye!\n"; break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
