#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
using namespace std;

struct Event {
    string desc;
};

void printCalendar(int month, int year, const map<string, vector<Event>>& events) {
    static const string months[] = {
        "", "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    static const int daysInMonth[] = {
        0,31,28,31,30,31,30,31,31,30,31,30,31
    };
    cout << "\n    " << months[month] << " " << year << endl;
    cout << "Su Mo Tu We Th Fr Sa\n";
    int days = daysInMonth[month];
    // Leap year
    if (month == 2 && ((year%4==0&&year%100!=0)||year%400==0)) days = 29;
    // Zeller's Congruence for first weekday
    int y = year - (month < 3);
    int first = (y + y/4 - y/100 + y/400 + "602531504624"[month] - '0') % 7;
    int d = 1;
    for(int i=0; i<first; ++i) cout << "   ";
    while (d <= days) {
        ostringstream oss; oss << setw(4) << setfill('0') << year << "-"
                               << setw(2) << setfill('0') << month << "-"
                               << setw(2) << setfill('0') << d;
        string key = oss.str();
        cout << setw(2) << d;
        if (events.count(key)) cout << "* "; else cout << "  ";
        if ((first + d)%7 == 0) cout << endl;
        ++d;
    }
    cout << endl << "(Days marked with * have events. Format: YYYY-MM-DD)" << endl;
}

void addEvent(map<string, vector<Event>>& events) {
    string date, desc;
    cout << "Add event on (YYYY-MM-DD): ";
    getline(cin, date);
    cout << "Event description: ";
    getline(cin, desc);
    events[date].push_back({desc});
    cout << "Event added on " << date << "!\n";
}

void showEvents(const map<string, vector<Event>>& events) {
    if (events.empty()) {
        cout << "No events scheduled.\n";
        return;
    }
    cout << "-- Scheduled Events --\n";
    for (const auto& e : events)
        for (size_t i = 0; i < e.second.size(); ++i) {
            cout << e.first << " [" << (i+1) << "]: "
                 << e.second[i].desc << endl;
        }
}

void removeEvent(map<string, vector<Event>>& events) {
    string date;
    cout << "Date of event to remove (YYYY-MM-DD): ";
    getline(cin, date);
    if (!events.count(date)) { cout << "No events on that day.\n"; return; }
    const auto& elist = events[date];
    for (size_t i=0; i<elist.size(); ++i)
        cout << (i+1) << ". " << elist[i].desc << endl;
    cout << "Enter number to remove: ";
    int num; cin >> num; cin.ignore();
    if (num < 1 || num > (int)elist.size()) { cout << "Invalid.\n"; return; }
    auto& vec = events[date];
    vec.erase(vec.begin() + (num-1));
    if (vec.empty()) events.erase(date);
    cout << "Event removed.\n";
}

int main() {
    map<string, vector<Event>> events;
    int choice;
    while (true) {
        cout << "\n--- Calendar & Event Scheduler ---\n";
        cout << "1. Show Calendar\n";
        cout << "2. Add Event\n";
        cout << "3. Show All Events\n";
        cout << "4. Remove Event\n";
        cout << "5. Exit\n";
        cout << "Choose: "; cin >> choice; cin.ignore();

        if (choice == 1) {
            int m, y;
            cout << "Enter month (1-12): "; cin >> m;
            cout << "Enter year (e.g., 2025): "; cin >> y; cin.ignore();
            if (m<1 || m>12 || y < 1900) { cout << "Invalid.\n"; continue; }
            printCalendar(m, y, events);
        }
        else if (choice == 2) {
            addEvent(events);
        }
        else if (choice == 3) {
            showEvents(events);
        }
        else if (choice == 4) {
            removeEvent(events);
        }
        else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
