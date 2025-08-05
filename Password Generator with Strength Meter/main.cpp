#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

string generatePassword(int length, bool useLower, bool useUpper, bool useDigit, bool useSymbol) {
    string lower = "abcdefghijklmnopqrstuvwxyz";
    string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string digits = "0123456789";
    string symbols = "!@#$%^&*()-_=+[]{}/?<>";

    string pool = "";
    if (useLower) pool += lower;
    if (useUpper) pool += upper;
    if (useDigit) pool += digits;
    if (useSymbol) pool += symbols;

    if (pool.empty() || length <= 0) return "";

    string password;
    for (int i = 0; i < length; ++i) {
        password += pool[rand() % pool.size()];
    }
    return password;
}

int getStrength(const string& pwd) {
    int score = 0;
    if (pwd.length() >= 8) score++;
    if (any_of(pwd.begin(), pwd.end(), ::isdigit)) score++;
    if (any_of(pwd.begin(), pwd.end(), ::islower)) score++;
    if (any_of(pwd.begin(), pwd.end(), ::isupper)) score++;
    if (any_of(pwd.begin(), pwd.end(), [](char c) { return string("!@#$%^&*()-_=+[]{}/?<>").find(c) != string::npos; })) score++;
    return score;
}

string strengthText(int score) {
    if (score <= 2) return "Weak";
    if (score == 3) return "Moderate";
    if (score == 4) return "Strong";
    return "Very Strong";
}

int main() {
    srand(time(0));
    cout << "--- Password Generator with Strength Meter ---\n";
    int length;
    char y;

    cout << "Password length: ";
    cin >> length;
    cout << "Include lowercase letters? (y/n): "; cin >> y;
    bool useLower = (y == 'y' || y == 'Y');
    cout << "Include uppercase letters? (y/n): "; cin >> y;
    bool useUpper = (y == 'y' || y == 'Y');
    cout << "Include digits? (y/n): "; cin >> y;
    bool useDigit = (y == 'y' || y == 'Y');
    cout << "Include symbols? (y/n): "; cin >> y;
    bool useSymbol = (y == 'y' || y == 'Y');

    string pwd = generatePassword(length, useLower, useUpper, useDigit, useSymbol);

    if (pwd.empty()) {
        cout << "Invalid settings, no characters to generate password.\n";
        return 1;
    }

    cout << "\nGenerated Password: " << pwd << endl;
    int score = getStrength(pwd);
    cout << "Password Strength: " << strengthText(score) << " (" << score << "/5)\n";

    return 0;
}
