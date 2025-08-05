#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
using namespace std;
using namespace chrono;

void printMenu(bool running) {
    cout << "\n--- Stopwatch / Timer ---\n";
    cout << "1. Start\n";
    cout << "2. Pause\n";
    cout << "3. Reset\n";
    cout << "4. Show elapsed time\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void printElapsed(duration<double> elapsed) {
    int hours   = static_cast<int>(elapsed.count()) / 3600;
    int minutes = (static_cast<int>(elapsed.count()) % 3600) / 60;
    int seconds = static_cast<int>(elapsed.count()) % 60;
    int millis  = static_cast<int>((elapsed.count() - static_cast<int>(elapsed.count())) * 1000);
    cout << "Elapsed: "
         << setfill('0') << setw(2) << hours   << ":"
         << setfill('0') << setw(2) << minutes << ":"
         << setfill('0') << setw(2) << seconds << "."
         << setfill('0') << setw(3) << millis  << "\n"
         << setfill(' ');
}

int main() {
    steady_clock::time_point start;
    duration<double> elapsed = seconds(0);
    bool running = false;

    int choice;
    while (true) {
        printMenu(running);
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        if (choice == 1) { // Start
            if (!running) {
                start = steady_clock::now();
                running = true;
                cout << "Stopwatch started.\n";
            } else {
                cout << "Already running.\n";
            }
        }
        else if (choice == 2) { // Pause
            if (running) {
                elapsed += steady_clock::now() - start;
                running = false;
                cout << "Paused.\n";
            } else {
                cout << "Not running.\n";
            }
        }
        else if (choice == 3) { // Reset
            elapsed = seconds(0);
            running = false;
            cout << "Reset to zero.\n";
        }
        else if (choice == 4) { // Show elapsed time
            if (running)
                printElapsed(elapsed + (steady_clock::now() - start));
            else
                printElapsed(elapsed);
        }
        else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
