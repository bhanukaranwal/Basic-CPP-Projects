#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Run-Length Encode: e.g. "aaabbc" -> "a3b2c1"
string rle_compress(const string& input) {
    ostringstream out;
    int n = input.size();
    for (int i = 0; i < n; ) {
        char c = input[i];
        int count = 1;
        while (i + count < n && input[i + count] == c) ++count;
        out << c << count;
        i += count;
    }
    return out.str();
}

// Run-Length Decode: e.g. "a3b2c1" -> "aaabbc"
string rle_decompress(const string& compressed) {
    ostringstream out;
    for (size_t i = 0; i < compressed.size(); ) {
        char c = compressed[i++];
        string num;
        while (i < compressed.size() && isdigit(compressed[i]))
            num += compressed[i++];
        int count = stoi(num);
        out << string(count, c);
    }
    return out.str();
}

void printMenu() {
    cout << "\n--- File Compression Simulator (RLE) ---\n";
    cout << "1. Compress input\n";
    cout << "2. Decompress input\n";
    cout << "3. Exit\n";
    cout << "Your choice: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            cout << "Enter text to compress: ";
            string input;
            getline(cin, input);
            string compressed = rle_compress(input);
            cout << "Compressed: " << compressed << endl;
        } else if (choice == 2) {
            cout << "Enter text to decompress (must be valid RLE format): ";
            string input;
            getline(cin, input);
            try {
                string decompressed = rle_decompress(input);
                cout << "Decompressed: " << decompressed << endl;
            } catch (...) {
                cout << "Input is not in valid RLE format.\n";
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
