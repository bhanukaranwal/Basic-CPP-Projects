/*
  Personal Data Analyzer (main.cpp)
  ------------------------------------
  - Lets users enter daily metrics (steps, sleep, study hours)
  - Stores data in a local file ("personal_data.txt") for persistence
  - Displays weekly and monthly summaries and ASCII bar graphs
  - Pure C++17, single file, extensible for any metric type

  Metrics:
    * Steps walked
    * Sleep hours
    * Study hours

  Concepts: file I/O, structuring, console visualization, rolling averages
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
using namespace std;

struct DayData {
    string date; // format: YYYY-MM-DD
    int steps;
    double sleep;
    double study;
    DayData(const string& d, int s, double sl, double st)
      : date(d), steps(s), sleep(sl), study(st) {}
};

vector<DayData> loadData(const string& fname) {
    vector<DayData> data;
    ifstream fin(fname);
    string line;
    while(getline(fin,line)) {
        istringstream ss(line);
        string date; int steps; double sleep, study;
        if(ss >> date >> steps >> sleep >> study)
            data.emplace_back(date, steps, sleep, study);
    }
    return data;
}

void saveData(const string& fname, const vector<DayData>& data) {
    ofstream fout(fname, ios::trunc);
    for(const auto& d : data)
        fout << d.date << " " << d.steps << " "
             << d.sleep << " " << d.study << "\n";
}

string todayDate() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

// Simple ASCII bar
void printBar(const string& label, double val, double scale, const string& unit) {
    int bars = val / scale;
    cout << setw(10) << label << " |";
    for(int i=0;i<bars;++i) cout << char(219);
    cout << " " << val << " " << unit << "\n";
}

// Weekly or monthly summary
void showSummary(const vector<DayData>& data, int days) {
    if(data.empty()) { cout << "(No data)\n"; return; }
    cout << "\nSummary (last " << days << " days):\n";
    int sumSteps=0; double sumSleep=0,sumStudy=0;
    int n=0;
    for(auto it=data.rbegin(); it!=data.rend()&&n<days;++it,++n) {
        sumSteps+=it->steps; sumSleep+=it->sleep; sumStudy+=it->study;
    }
    cout << "  Avg Steps: " << (n?sumSteps/n:0) << "\n";
    cout << "  Avg Sleep: " << fixed << setprecision(2) << (n?sumSleep/n:0) << " hrs\n";
    cout << "  Avg Study: " << fixed << setprecision(2) << (n?sumStudy/n:0) << " hrs\n";
    // Show bar charts
    cout << "\n  Steps bar chart (per day):\n";
    n=0;
    for(auto it=data.rbegin(); it!=data.rend()&&n<days;++it,++n)
        printBar(it->date.substr(5), it->steps, 500, "steps");
    cout << "  Sleep bar chart (per day):\n";
    n=0;
    for(auto it=data.rbegin(); it!=data.rend()&&n<days;++it,++n)
        printBar(it->date.substr(5), it->sleep, 0.5, "hr");
    cout << "  Study bar chart (per day):\n";
    n=0;
    for(auto it=data.rbegin(); it!=data.rend()&&n<days;++it,++n)
        printBar(it->date.substr(5), it->study, 0.5, "hr");
}

int main() {
    cout << "=== Personal Data Analyzer ===\n";
    cout << "Tracks your daily Steps, Sleep Hours, Study Hours.\n";
    cout << "Data stored in: personal_data.txt (editable)\n";
    string fname = "personal_data.txt";
    vector<DayData> db = loadData(fname);

    while(true) {
        cout << "\nCommands: log, recent, weekly, monthly, edit, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd=="log") {
            string date = todayDate();
            // Prevent duplicate
            auto it = find_if(db.begin(), db.end(), [&](const DayData& d){return d.date==date;});
            if(it != db.end()) { cout << "Data for today already entered. Use edit.\n"; continue; }
            int steps; double sleep, study;
            cout << "Enter steps walked: "; cin >> steps;
            cout << "Enter sleep hours: "; cin >> sleep;
            cout << "Enter study hours: "; cin >> study;
            db.emplace_back(date,steps,sleep,study);
            cout << "Logged for " << date << ".\n";
            saveData(fname, db);
        } else if (cmd=="recent") {
            if(db.empty()) { cout << "(No data)\n"; continue; }
            const auto& d = db.back();
            cout << "Last Entry (" << d.date << "): "
                 << d.steps << " steps, "
                 << d.sleep << "hr sleep, "
                 << d.study << "hr study\n";
        } else if (cmd=="weekly") {
            showSummary(db,7);
        } else if (cmd=="monthly") {
            showSummary(db,31);
        } else if (cmd=="edit") {
            cout << "Enter date to edit (YYYY-MM-DD): ";
            string date; cin >> date;
            auto it = find_if(db.begin(), db.end(), [&](const DayData& d){return d.date==date;});
            if(it == db.end()) { cout << "No entry for that date.\n"; continue; }
            int steps; double sleep, study;
            cout << "New steps: "; cin >> steps;
            cout << "New sleep: "; cin >> sleep;
            cout << "New study: "; cin >> study;
            it->steps = steps; it->sleep = sleep; it->study = study;
            saveData(fname, db);
            cout << "Edited.\n";
        } else if (cmd=="quit") break;
        else cout << "Unknown command. Try log, recent, weekly, monthly, edit, quit.\n";
    }
    cout << "Data saved. Goodbye!\n";
    return 0;
}
