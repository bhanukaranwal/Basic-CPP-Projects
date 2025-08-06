/*
  Habit Reminder & Nudge Tool (main.cpp)
  ----------------------------------------
  - Add, list, and configure habits/reminders (custom intervals)
  - Sends console "nudge" notifications at each interval (for each habit)
  - Tracks completion/checks and streaks per habit
  - Shows daily and weekly summaries
  - Data persisted to "habits.txt", pure C++17

  Concepts: scheduling, cross-interval reminders, file I/O, summary stats
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

// Habit record (configurable interval in min, streaks, logs)
struct Habit {
    string name;
    int intervalMin;
    int streak;
    vector<string> checkedDates; // "YYYY-MM-DD"

    Habit(const string& n, int iv = 60) : name(n), intervalMin(iv), streak(0) {}

    bool isChecked(const string& date) {
        return find(checkedDates.begin(), checkedDates.end(), date) != checkedDates.end();
    }
    void check(const string& date) {
        if (!isChecked(date)) checkedDates.push_back(date);
    }
    void forget(const string& date) {
        checkedDates.erase(remove(checkedDates.begin(), checkedDates.end(), date), checkedDates.end());
    }
};

string todayDate() {
    time_t now = time(nullptr);
    tm* lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

vector<Habit> loadHabits(const string& fname) {
    vector<Habit> habits;
    ifstream fin(fname);
    string line;
    while (getline(fin, line)) {
        istringstream ss(line);
        string name, dates;
        int interval, streak;
        getline(ss, name, '|'); ss >> interval >> streak; ss.ignore();
        vector<string> cks;
        while (getline(ss, dates, ',')) if (!dates.empty()) cks.push_back(dates);
        Habit h(name, interval); h.streak = streak; h.checkedDates = cks;
        habits.push_back(h);
    }
    return habits;
}
void saveHabits(const string& fname, const vector<Habit>& habits) {
    ofstream fout(fname, ios::trunc);
    for (const auto& h : habits) {
        fout << h.name << "|" << h.intervalMin << " " << h.streak << " ";
        for (const auto& d : h.checkedDates) fout << d << ",";
        fout << "\n";
    }
}

void showHabits(const vector<Habit>& habits) {
    cout << "\nYour habits:\n";
    for (size_t i = 0; i < habits.size(); ++i) {
        cout << i+1 << ". " << setw(20) << habits[i].name
             << " | every " << habits[i].intervalMin << " min"
             << " | Streak: " << habits[i].streak << "\n";
    }
}

void showSummary(const vector<Habit>& habits, int days = 7) {
    cout << "\nHabit Streak Summary (" << days << " days):\n";
    string today = todayDate();
    for (const auto& h : habits) {
        int count = 0;
        for (const auto& d : h.checkedDates)
            if (d >= today.substr(0, 8) + to_string(max(1, stoi(today.substr(8,2)) - days + 1)))
                ++count;
        cout << "  " << setw(20) << h.name << ": " << count << "/" << days << " days\n";
    }
}

int main() {
    cout << "=== Habit Reminder & Nudge Tool ===\n";
    cout << "Store and nudge any set of daily habits with interval-based reminders.\n";
    string fname = "habits.txt";
    vector<Habit> habits = loadHabits(fname);

    while (true) {
        cout << "\nCommands: add, list, remove, start, check, streaks, quit\n> ";
        string cmd; cin >> cmd;
        if (cmd == "add") {
            string name; int iv;
            cout << "Habit name: "; cin.ignore(); getline(cin, name);
            cout << "Interval (min): "; cin >> iv;
            habits.emplace_back(name, iv);
            saveHabits(fname, habits);
            cout << "Added.\n";
        } else if (cmd == "list") {
            showHabits(habits);
        } else if (cmd == "remove") {
            showHabits(habits);
            cout << "Remove which #? "; int idx; cin >> idx;
            if (idx > 0 && idx <= habits.size()) {
                habits.erase(habits.begin() + idx - 1);
                saveHabits(fname, habits);
                cout << "Removed.\n";
            }
        } else if (cmd == "start") {
            if (habits.empty()) { cout << "No habits!\n"; continue; }
            vector<chrono::steady_clock::time_point> next(habits.size());
            auto start = chrono::steady_clock::now();
            for (size_t i = 0; i < habits.size(); ++i)
                next[i] = start + chrono::minutes(habits[i].intervalMin);
            cout << "[Habit nudger started. Ctrl+C to stop. Answer reminders as prompted.]\n";
            while (true) {
                this_thread::sleep_for(chrono::seconds(10));
                auto now = chrono::steady_clock::now();
                string today = todayDate();
                for (size_t i = 0; i < habits.size(); ++i) {
                    if (now >= next[i] && !habits[i].isChecked(today)) {
                        cout << "\a\n[Reminder] Time for: " << habits[i].name << " (" << habits[i].intervalMin << " min)\n";
                        cout << "Completed? (y/n): ";
                        string ans; cin >> ans;
                        if (ans == "y" || ans == "Y") {
                            habits[i].check(today);
                            habits[i].streak++;
                            saveHabits(fname, habits);
                            cout << "Great! Streak: " << habits[i].streak << "\n";
                        } else {
                            habits[i].streak = 0;
                        }
                        next[i] = now + chrono::minutes(habits[i].intervalMin);
                    }
                }
            }
        } else if (cmd == "check") {
            string today = todayDate();
            showHabits(habits);
            cout << "Which # to mark complete (or 0 to skip): ";
            int idx; cin >> idx;
            if (idx > 0 && idx <= habits.size()) {
                habits[idx-1].check(today);
                habits[idx-1].streak++;
                saveHabits(fname, habits);
                cout << "Done.\n";
            }
        } else if (cmd == "streaks") {
            showSummary(habits);
        } else if (cmd == "quit") {
            saveHabits(fname, habits);
            cout << "All data saved. Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
