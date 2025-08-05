#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Luhn algorithm for card validation
bool isValidCreditCard(const string& num) {
    int sum = 0;
    bool alt = false;
    for (int i = num.length() - 1; i >= 0; --i) {
        if (!isdigit(num[i])) return false; // Only digits allowed
        int digit = num[i] - '0';
        if (alt) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
        alt = !alt;
    }
    return (sum % 10 == 0);
}

int main() {
    cout << "--- Credit Card Validator ---\n";
    string cardNum;
    cout << "Enter credit card number: ";
    getline(cin, cardNum);
    // Remove spaces
    cardNum.erase(remove(cardNum.begin(), cardNum.end(), ' '), cardNum.end());
    if (isValidCreditCard(cardNum))
        cout << "Valid credit card number.\n";
    else
        cout << "Invalid credit card number.\n";
    return 0;
}
