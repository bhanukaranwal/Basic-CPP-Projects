#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Event {
    int year, month, day;
    string description;
};

bool sameDate(const Event& e, int y, int m, int d) {
    return e.year == y && e.month == m && e.day == d;
}

void printMenu() {
    cout << "\n--- Calendar & Reminder System ---\n";
    cout << "1. Add Event\n";
    cout << "2. View All Events\n";
    cout << "3. View Events for a Date\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void showEvents(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events recorded yet.\n";
        return;
    }
    cout << "\nDate        | Event\n";
    cout << "------------------------------\n";
    for (const auto& e : events) {
        cout << setw(4) << setfill('0') << e.year << '-'
             << setw(2) << setfill('0') << e.month << '-'
             << setw(2) << setfill('0') << e.day << " | "
             << e.description << "\n";
    }
    cout << setfill(' ');
}

int main() {
    vector<Event> events;
    int choice;

    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore();

        if (choice == 1) { // Add Event
            Event e;
            cout << "Enter year (e.g., 2025): ";
            cin >> e.year;
            cout << "Enter month (1-12): ";
            cin >> e.month;
            cout << "Enter day (1-31): ";
            cin >> e.day;
            cin.ignore();
            cout << "Enter event description: ";
            getline(cin, e.description);
            events.push_back(e);
            cout << "Event added!\n";
        }
        else if (choice == 2) { // View all
            showEvents(events);
        }
        else if (choice == 3) { // View by date
            int y, m, d;
            cout << "Enter year (e.g., 2025): ";
            cin >> y;
            cout << "Enter month (1-12): ";
            cin >> m;
            cout << "Enter day (1-31): ";
            cin >> d;
            bool found = false;
            cout << "\nEvents on " << setw(4) << setfill('0') << y << '-'
                 << setw(2) << setfill('0') << m << '-'
                 << setw(2) << setfill('0') << d << ":\n";
            cout << setfill(' ');
            for (const auto& e : events) {
                if (sameDate(e, y, m, d)) {
                    cout << "- " << e.description << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No events found.\n";
        }
        else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please enter 1-4.\n";
        }
    }
    return 0;
}
