#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

// Check if a number is prime
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i*i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

// Generate all primes up to n (Sieve)
vector<int> generatePrimes(int n) {
    vector<bool> sieve(n+1, true);
    vector<int> primes;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i <= n; ++i) {
        if (sieve[i]) {
            primes.push_back(i);
            for (int j = i*i; j <= n; j += i)
                sieve[j] = false;
        }
    }
    return primes;
}

// Prime factorization
vector<int> primeFactors(int n) {
    vector<int> factors;
    for (int i = 2; i*i <= n; ++i) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

void printMenu() {
    cout << "\n--- Prime Number Utility ---\n";
    cout << "1. Check if number is prime\n";
    cout << "2. Generate primes up to N\n";
    cout << "3. Prime factorization\n";
    cout << "4. Exit\n";
    cout << "Choose: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        if (choice == 1) {
            int x;
            cout << "Enter number: "; cin >> x;
            if (isPrime(x))
                cout << x << " is a prime number.\n";
            else
                cout << x << " is not a prime number.\n";
        }
        else if (choice == 2) {
            int n;
            cout << "Generate primes up to: "; cin >> n;
            auto primes = generatePrimes(n);
            cout << "Primes up to " << n << ":\n";
            for (size_t i = 0; i < primes.size(); ++i) {
                cout << setw(5) << primes[i];
                if ((i+1)%12 == 0) cout << '\n';
            }
            cout << endl;
        }
        else if (choice == 3) {
            int n;
            cout << "Enter number to factor: "; cin >> n;
            auto facs = primeFactors(n);
            cout << "Prime factors: ";
            for (size_t i = 0; i < facs.size(); ++i) {
                cout << facs[i];
                if (i+1 < facs.size()) cout << " x ";
            }
            if (facs.empty()) cout << "None (check input)";
            cout << endl;
        }
        else if (choice == 4) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid choice. Enter 1-4.\n";
        }
    }
    return 0;
}
