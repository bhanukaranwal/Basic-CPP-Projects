#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

struct Alarm {
    int hour;
    int minute;
    string label;
};

void printMenu() {
    cout << "\n--- Alarm Clock ---\n";
    cout << "1. Set New Alarm\n";
    cout << "2. View All Alarms\n";
    cout << "3. Remove Alarm\n";
    cout << "4. Exit\n";
    cout << "Choose: ";
}

void showAlarms(const vector<Alarm>& alarms) {
    if (alarms.empty()) {
        cout << "No alarms set.\n";
        return;
    }
    cout << "\n--- Active Alarms ---\n";
    for (size_t i = 0; i < alarms.size(); ++i) {
        cout << i+1 << ". ["
             << setw(2) << setfill('0') << alarms[i].hour << ":"
             << setw(2) << setfill('0') << alarms[i].minute << "] ";
        cout << alarms[i].label << "\n";
    }
    cout << setfill(' ');
}

bool checkAlarmHit(const Alarm& alarm, int h, int m) {
    return alarm.hour == h && alarm.minute == m;
}

int main() {
    vector<Alarm> alarms;
    int choice;

    cout << "--- Simple Alarm Clock ---\n";
    cout << "Alarm runs as long as program is running. Make sure time is correct on your system!\n";

    // We'll run a "background" alarm check in main loop, with brief sleep between user interactions.
    while (true) {
        printMenu();

        // Check current time
        time_t now = time(0);
        tm *ltm = localtime(&now);

        // Ring any matching alarms
        for (auto it = alarms.begin(); it != alarms.end(); ) {
            if (checkAlarmHit(*it, ltm->tm_hour, ltm->tm_min)) {
                cout << "\n*** ALARM! (" 
                     << setw(2) << setfill('0') << it->hour << ":"
                     << setw(2) << setfill('0') << it->minute << ") *** "
                     << it->label << " ***\n\a"; // Beep, if terminal supports it
                it = alarms.erase(it); // Remove after ringing (one-shot alarm)
            } else {
                ++it;
            }
        }
        cout << setfill(' ');

        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            // Set new alarm
            Alarm a;
            cout << "Set hour (0-23): "; cin >> a.hour; cin.ignore();
            cout << "Set minute (0-59): "; cin >> a.minute; cin.ignore();
            if (a.hour < 0 || a.hour > 23 || a.minute < 0 || a.minute > 59) {
                cout << "Invalid time!\n";
                continue;
            }
            cout << "Label for alarm (optional): ";
            getline(cin, a.label);
            alarms.push_back(a);
            cout << "Alarm set for "
                 << setw(2) << setfill('0') << a.hour << ":"
                 << setw(2) << setfill('0') << a.minute << " (" << a.label << ")\n";
        }
        else if (choice == 2) {
            showAlarms(alarms);
        }
        else if (choice == 3) {
            showAlarms(alarms);
            if (alarms.empty()) continue;
            cout << "Enter alarm number to remove: ";
            int n; cin >> n; cin.ignore();
            if (n >= 1 && n <= (int)alarms.size()) {
                alarms.erase(alarms.begin() + (n-1));
                cout << "Alarm removed.\n";
            } else {
                cout << "Invalid number.\n";
            }
        }
        else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-4.\n";
        }
        // Give some time for system time to update, so alarms can ring at the right minute.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
