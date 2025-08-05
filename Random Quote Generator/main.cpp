#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<string> quotes = {
        "The only way to do great work is to love what you do. – Steve Jobs",
        "Life is what happens when you're busy making other plans. – John Lennon",
        "Success is not final, failure is not fatal: It is the courage to continue that counts. – Winston Churchill",
        "You miss 100% of the shots you don’t take. – Wayne Gretzky",
        "Be yourself; everyone else is already taken. – Oscar Wilde",
        "Strive not to be a success, but rather to be of value. – Albert Einstein",
        "The best way to predict the future is to invent it. – Alan Kay",
        "To live is the rarest thing in the world. Most people exist, that is all. – Oscar Wilde",
        "If you’re going through hell, keep going. – Winston Churchill",
        "Happiness is not by chance, but by choice. – Jim Rohn"
    };

    cout << "--- Random Quote Generator ---\n";
    char ans;
    do {
        int idx = rand() % quotes.size();
        cout << "\n" << quotes[idx] << "\n";
        cout << "\nGet another quote? (y/n): ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');
    cout << "Goodbye!\n";
    return 0;
}
