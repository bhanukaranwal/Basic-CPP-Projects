#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Sieve of Eratosthenes
vector<int> generatePrimes(int limit) {
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= limit; j += i)
                isPrime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= limit; ++i)
        if (isPrime[i]) primes.push_back(i);
    return primes;
}

bool isPrimeNumber(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); ++i)
        if (n % i == 0) return false;
    return true;
}

void printMenu() {
    cout << "\n--- Prime Number Generator and Tester ---\n";
    cout << "1. Generate all primes up to N\n";
    cout << "2. Test if a number is prime\n";
    cout << "3. Exit\n";
    cout << "Your choice: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        if (choice == 1) {
            int n;
            cout << "Generate all primes up to: ";
            cin >> n;
            if (n < 2) { cout << "No primes available below 2.\n"; continue; }
            auto primes = generatePrimes(n);
            cout << "Primes up to " << n << ":\n";
            for (size_t i = 0; i < primes.size(); ++i) {
                cout << primes[i] << (i % 20 == 19 ? '\n' : ' ');
            }
            cout << endl;
        } else if (choice == 2) {
            int n;
            cout << "Enter number to test: ";
            cin >> n;
            if (isPrimeNumber(n)) {
                cout << n << " is a prime number.\n";
            } else {
                cout << n << " is NOT a prime number.\n";
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}
