#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    cout << "--- File Word/Line/Byte Counter (wc clone) ---\n";
    cout << "Enter filename: ";
    string filename;
    getline(cin, filename);

    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Could not open file.\n";
        return 1;
    }

    int lines = 0, words = 0, bytes = 0;
    bool in_word = false;
    char ch;
    while (fin.get(ch)) {
        bytes++;
        if (ch == '\n') lines++;
        if (isspace(ch)) {
            in_word = false;
        } else if (!in_word) {
            words++;
            in_word = true;
        }
    }

    cout << "Lines:  " << lines << endl;
    cout << "Words:  " << words << endl;
    cout << "Bytes:  " << bytes << endl;
    fin.close();
    return 0;
}
