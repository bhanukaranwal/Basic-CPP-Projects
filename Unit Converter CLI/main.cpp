#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

void printMenu() {
    cout << "\n--- Unit Converter CLI ---\n";
    cout << "1. Length (meters <-> feet)\n";
    cout << "2. Weight (kilograms <-> pounds)\n";
    cout << "3. Temperature (Celsius <-> Fahrenheit)\n";
    cout << "4. Currency (USD <-> INR)\n";
    cout << "5. Exit\n";
    cout << "Choose conversion: ";
}

void lengthConversion() {
    cout << "Convert:\n  1. Meters to Feet\n  2. Feet to Meters\nChoice: ";
    int choice; cin >> choice;
    double val;
    if (choice == 1) {
        cout << "Enter meters: "; cin >> val;
        cout << fixed << setprecision(4) << val << " meters = " << val * 3.28084 << " feet\n";
    } else if (choice == 2) {
        cout << "Enter feet: "; cin >> val;
        cout << fixed << setprecision(4) << val << " feet = " << val / 3.28084 << " meters\n";
    } else cout << "Invalid option.\n";
}

void weightConversion() {
    cout << "Convert:\n  1. Kilograms to Pounds\n  2. Pounds to Kilograms\nChoice: ";
    int choice; cin >> choice;
    double val;
    if (choice == 1) {
        cout << "Enter kilograms: "; cin >> val;
        cout << fixed << setprecision(4) << val << " kg = " << val * 2.20462 << " lbs\n";
    } else if (choice == 2) {
        cout << "Enter pounds: "; cin >> val;
        cout << fixed << setprecision(4) << val << " lbs = " << val / 2.20462 << " kg\n";
    } else cout << "Invalid option.\n";
}

void temperatureConversion() {
    cout << "Convert:\n  1. Celsius to Fahrenheit\n  2. Fahrenheit to Celsius\nChoice: ";
    int choice; cin >> choice;
    double val;
    if (choice == 1) {
        cout << "Enter Celsius: "; cin >> val;
        cout << fixed << setprecision(2) << val << " C = " << (val * 9 / 5) + 32 << " F\n";
    } else if (choice == 2) {
        cout << "Enter Fahrenheit: "; cin >> val;
        cout << fixed << setprecision(2) << val << " F = " << (val - 32) * 5 / 9 << " C\n";
    } else cout << "Invalid option.\n";
}

void currencyConversion() {
    cout << "(Note: For demo, fixed rates: 1 USD = 83 INR)\n";
    cout << "Convert:\n  1. USD to INR\n  2. INR to USD\nChoice: ";
    int choice; cin >> choice;
    double val;
    if (choice == 1) {
        cout << "Enter USD: "; cin >> val;
        cout << fixed << setprecision(2) << val << " USD = " << val * 83.0 << " INR\n";
    } else if (choice == 2) {
        cout << "Enter INR: "; cin >> val;
        cout << fixed << setprecision(2) << val << " INR = " << val / 83.0 << " USD\n";
    } else cout << "Invalid option.\n";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        if (choice == 1) lengthConversion();
        else if (choice == 2) weightConversion();
        else if (choice == 3) temperatureConversion();
        else if (choice == 4) currencyConversion();
        else if (choice == 5) { cout << "Goodbye!\n"; break; }
        else cout << "Invalid option. Try again.\n";
    }
    return 0;
}
