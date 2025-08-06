/*
  Screen Time Tracker (main.cpp)
  ------------------------------
  - Logs time spent per session, with break/time-up alerts.
  - Auto-starts timer when run, reminders configurable.
  - Shows today's and weekly summaries.
  - Saves all data in screentime_log.txt.
  - All C++17, portable, easily extensible.

  Concepts: chrono, file I/O, time formatting, minimal UX reminders
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

// Utility: Get current date as YYYY-MM-DD
string todayDate() {
    time_t now = time(nullptr);
    tm* lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

struct LogEntry {
    string date;
    int seconds;
    LogEntry(const string& d, int s): date(d), seconds(s) {}
};

// Load logs
vector<LogEntry> loadLog(const string& fname) {
    vector<LogEntry> log;
    ifstream fin(fname);
    string date; int secs;
    while (fin >> date >> secs)
        log.emplace_back(date, secs);
    return log;
}

// Save log
void saveLog(const string& fname, vector<LogEntry>& log) {
    ofstream fout(fname, ios::trunc);
    for (auto& e : log)
        fout << e.date << " " << e.seconds << "\n";
}

// Nicely format seconds as h:mm:ss
string formatSecs(int sec) {
    ostringstream oss;
    int h = sec/3600, m = (sec%3600)/60, s = sec%60;
    oss << h << ":" << setw(2) << setfill('0') << m << ":"
        << setw(2) << setfill('0') << s;
    return oss.str();
}

// Show daily or weekly summary
void printSummary(const vector<LogEntry>& log, int days = 7) {
    cout << "\nScreen Time Summary (" << days << " days):\n";
    int total = 0, n = 0;
    for (auto it = log.rbegin(); it != log.rend() && n < days; ++it, ++n) {
        cout << "  " << setw(12) << it->date << " : " << formatSecs(it->seconds) << "\n";
        total += it->seconds;
    }
    cout << "  (avg: " << formatSecs(n ? total/n : 0) << " per day)\n";
}

int main() {
    cout << "=== Screen Time Tracker ===\n";
    cout << "Tracks your session and daily totals. Data: screentime_log.txt\n";
    string fname = "screentime_log.txt";
    vector<LogEntry> log = loadLog(fname);

    // Settings
    int remindMins = 60;        // Remind every X minutes
    int hardLimitMins = 180;    // Optional: hard daily limit (0 = off)

    cout << "Session (run timer). Enter to start, Ctrl+C to end early.\n";
    cout << "How many minutes before reminders? (60=default): ";
    string input; getline(cin, input);
    if (!input.empty()) remindMins = stoi(input);
    cout << "Hard session limit in minutes (0=off, 180=default): ";
    getline(cin, input);
    if (!input.empty()) hardLimitMins = stoi(input);

    cout << "[Timer started] Minimize or background this window.\n";
    cout << "You will receive reminders every " << remindMins << " minutes.\n";

    auto start = chrono::steady_clock::now();
    int lastAlarm = 0;
    bool overLimit = false;

    // Main loop: check time, remind, stop if hard limit hit
    while(true) {
        this_thread::sleep_for(chrono::seconds(10));
        int elapsed = chrono::duration_cast<chrono::seconds>(
                          chrono::steady_clock::now()-start).count();
        if (remindMins > 0 && elapsed/60 > lastAlarm && elapsed/60 % remindMins == 0) {
            cout << "\a\n[Break Reminder] You've been at it for " << formatSecs(elapsed)
                 << ". Time to stretch or rest your eyes!\n";
            lastAlarm = elapsed/60;
        }
        if (hardLimitMins > 0 && elapsed/60 >= hardLimitMins) {
            cout << "\n=== Hard session limit reached! Logging you out. ===\n";
            overLimit = true;
            break;
        }
        // Non-block: let user Ctrl+C to end, or add other user-interrupt logic if desired
    }

    auto end = chrono::steady_clock::now();
    int sessionSecs = chrono::duration_cast<chrono::seconds>(end-start).count();
    string today = todayDate();

    // Find or append today's total
    auto it = find_if(log.begin(), log.end(), [&](const LogEntry& e){ return e.date == today; });
    if (it == log.end()) log.emplace_back(today, 0), it = log.end()-1;
    it->seconds += sessionSecs;

    saveLog(fname, log);

    cout << "\nSession duration: " << formatSecs(sessionSecs) << "\n";
    cout << "Updated today's total: " << formatSecs(it->seconds) << "\n";
    printSummary(log, 7);

    cout << "Goodbye!\n";
    return 0;
}
