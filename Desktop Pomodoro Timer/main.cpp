/*
  Desktop Pomodoro Timer (main.cpp)
  --------------------------------------
  - Implements the Pomodoro productivity technique
  - Customizable work/break intervals, auto-logging to "pomodoro_log.txt"
  - Provides daily and weekly summary stats
  - Core timer, alerts, logging all in C++17, single file

  Concepts: system clock, simple timer, file I/O, CLI, data summaries
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>
#include <algorithm>
using namespace std;

// Utility: Current date string (YYYY-MM-DD)
string todayDate() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

// Pomodoro log entry
struct LogEntry {
    std::string date;
    int sessions;

    LogEntry(const string& d, int s): date(d), sessions(s) {}
};

// Load existing daily log
vector<LogEntry> loadLog(const string& fname) {
    vector<LogEntry> log;
    ifstream fin(fname);
    string date; int count;
    while (fin >> date >> count)
        log.emplace_back(date, count);
    return log;
}

// Save log
void saveLog(const string& fname, vector<LogEntry>& log) {
    ofstream fout(fname, ios::trunc);
    for (auto& e : log)
        fout << e.date << " " << e.sessions << "\n";
}

void beep() {
    cout << "\a";
    cout.flush();
}

void printSummary(const vector<LogEntry>& log, int days = 7) {
    cout << "\nPomodoro Summary (" << days << " days):\n";
    int total = 0;
    int n = 0;
    for (auto it = log.rbegin(); it != log.rend() && n < days; ++it, ++n) {
        cout << "  " << setw(12) << it->date << " : " << it->sessions << " sessions\n";
        total += it->sessions;
    }
    cout << "  (avg: ";
    cout << fixed << setprecision(2)
         << (n ? float(total) / n : 0) << " per day)";
    cout << "\n";
}

void runPomodoro(int workMin, int breakMin, int rounds, vector<LogEntry>& log, const string& fname) {
    cout << "\n--- Pomodoro Focus Session ---\n";
    int todayIdx = -1;
    string today = todayDate();
    for (size_t i = 0; i < log.size(); ++i)
        if (log[i].date == today) todayIdx = i;

    if (todayIdx == -1) { log.emplace_back(today, 0); todayIdx = log.size() - 1; }

    for (int i = 1; i <= rounds; ++i) {
        cout << "[Work] Block " << i << "/" << rounds << " (" << workMin << " min)...\n";
        for (int sec = 0; sec < workMin * 60; ++sec) {
            if (sec % 60 == 0 && sec) cout << '.';
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        beep();
        cout << "\n[Break] Block " << i << "/" << rounds << " (" << breakMin << " min)...\n";
        for (int sec = 0; sec < breakMin * 60; ++sec) {
            if (sec % 60 == 0 && sec) cout << '.';
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        beep();
        cout << "\n";
        log[todayIdx].sessions++;
        saveLog(fname, log);
    }
    cout << "\nPomodoro session complete! " << rounds << " work intervals logged.\n";
}

int main() {
    cout << "=== Desktop Pomodoro Timer ===\n";
    cout << "Auto-logs your daily sessions to pomodoro_log.txt\n";

    string fname = "pomodoro_log.txt";
    vector<LogEntry> log = loadLog(fname);

    while (true) {
        cout << "\nCommands: start, today, week, log, quit\n> ";
        string cmd; cin >> cmd;
        if (cmd == "start") {
            int wmin = 25, bmin = 5, rounds = 4;
            cout << "Work min (default 25): "; cin >> wmin;
            cout << "Break min (default 5): "; cin >> bmin;
            cout << "Rounds (default 4): "; cin >> rounds;
            cin.ignore();
            cout << "[Press ENTER to begin]"; cin.get();
            runPomodoro(wmin, bmin, rounds, log, fname);
        } else if (cmd == "today") {
            string today = todayDate();
            auto it = std::find_if(log.begin(), log.end(),
                                   [&](const LogEntry& e) { return e.date == today; });
            int n = (it != log.end() ? it->sessions : 0);
            cout << "Today's Pomodoro sessions: " << n << "\n";
        } else if (cmd == "week") {
            printSummary(log, 7);
        } else if (cmd == "log") {
            printSummary(log, 30);
        } else if (cmd == "quit") {
            saveLog(fname, log);
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
