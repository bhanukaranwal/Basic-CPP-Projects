#include <iostream>
#include <string>
using namespace std;

string caesarEncrypt(const string& text, int shift) {
    string result = text;
    for (char& c : result) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = base + (c - base + shift + 26) % 26;
        }
    }
    return result;
}

string caesarDecrypt(const string& text, int shift) {
    return caesarEncrypt(text, -shift);
}

string xorEncryptDecrypt(const string& text, char key) {
    string result = text;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

void printMenu() {
    cout << "\n--- Basic Encryption/Decryption Tool ---\n";
    cout << "1. Caesar Cipher Encrypt\n";
    cout << "2. Caesar Cipher Decrypt\n";
    cout << "3. XOR Cipher Encrypt/Decrypt\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore();
        if (choice == 1) {
            cout << "Enter message: ";
            string msg; getline(cin, msg);
            cout << "Enter shift (integer): ";
            int shift; cin >> shift; cin.ignore();
            cout << "Encrypted message: " << caesarEncrypt(msg, shift) << endl;
        } else if (choice == 2) {
            cout << "Enter message: ";
            string msg; getline(cin, msg);
            cout << "Enter shift (integer): ";
            int shift; cin >> shift; cin.ignore();
            cout << "Decrypted message: " << caesarDecrypt(msg, shift) << endl;
        } else if (choice == 3) {
            cout << "Enter message: ";
            string msg; getline(cin, msg);
            cout << "Enter single character key: ";
            char key; cin >> key; cin.ignore();
            string res = xorEncryptDecrypt(msg, key);
            cout << "Result: " << res << "\n";
            // Show you can reverse:
            cout << "Reversing XOR gives: " << xorEncryptDecrypt(res, key) << endl;
        } else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
