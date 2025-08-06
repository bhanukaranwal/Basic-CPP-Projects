/*
  Local Weather Logger (main.cpp)
  -----------------------------------
  - Lets you log daily weather: temperature (°C), humidity (%), sky condition
  - Stores data in "weatherlog.txt" for persistence and long-term tracking
  - Supports edit/review, weekly/monthly trend graphs (ASCII bar style)
  - Fully C++17, single file, extensible for sensors or online use

  Concepts: file I/O, CLI UX, moving averages, ASCII data visualization
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cmath>
using namespace std;

struct WeatherEntry {
    string date; // YYYY-MM-DD
    double tempC;
    int humidity;
    string sky;
    WeatherEntry(const string& d, double t, int h, const string& s)
      : date(d), tempC(t), humidity(h), sky(s) {}
};

vector<WeatherEntry> loadLog(const string& fname) {
    vector<WeatherEntry> db;
    ifstream fin(fname);
    string line;
    while(getline(fin, line)) {
        istringstream ss(line);
        string date, sky;
        double t; int h;
        ss >> date >> t >> h; getline(ss, sky); if (!sky.empty() && sky[0]==' ') sky = sky.substr(1);
        db.emplace_back(date, t, h, sky);
    }
    return db;
}

void saveLog(const string& fname, const vector<WeatherEntry>& db) {
    ofstream fout(fname, ios::trunc);
    for(const auto& e : db)
        fout << e.date << " " << fixed << setprecision(1) << e.tempC << " " << e.humidity << " " << e.sky << "\n";
}

string todayDate() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

void plotTrend(const vector<WeatherEntry>& db, int days, const string& which) {
    cout << "\n--- " << which << " Trend (last " << days << " days) ---\n";
    double minv = 1e9, maxv = -1e9;
    vector<double> vals;
    int n = 0;
    for(auto it=db.rbegin(); it!=db.rend() && n<days; ++it,++n) {
        double v = which=="Temperature" ? it->tempC : it->humidity;
        vals.push_back(v);
        minv = min(minv, v);
        maxv = max(maxv, v);
    }
    if(vals.empty()) { cout << "(No recent data)\n"; return; }
    reverse(vals.begin(), vals.end());
    for(size_t i=0;i<vals.size();++i) {
        cout << setw(2) << i+1 << " ";
        int bars = (int)round((vals[i]-minv)/(maxv-minv+0.01)*35);
        for(int b=0;b<bars;++b) cout << (which == "Temperature" ? '#' : '=');
        cout << " " << fixed << setprecision(1) << vals[i]
             << (which=="Temperature"?"°C":"%");
        cout << "\n";
    }
    cout << "Min: " << minv << ", Max: " << maxv << "\n";
}

void showSummary(const vector<WeatherEntry>& db, int N = 7) {
    if(db.empty()) { cout << "(No data)\n"; return; }
    double st=0, sh=0; int n=0;
    map<string,int> skyhist;
    for(auto it=db.rbegin(); it!=db.rend()&&n<N;++it,++n) {
        st += it->tempC; sh += it->humidity; skyhist[it->sky]++;
    }
    cout << "\n--- Recent " << N << "-Day Weather Stats ---\n";
    cout << "Avg temp: " << fixed << setprecision(1) << (n?st/n:0) << "°C\n";
    cout << "Avg humidity: " << fixed << setprecision(1) << (n?sh/n:0) << "%\n";
    cout << "Sky condition counts: ";
    for(auto& kv : skyhist)
        cout << kv.first << ":" << kv.second << " ";
    cout << "\n";
    plotTrend(db, N, "Temperature");
    plotTrend(db, N, "Humidity");
}

int main() {
    cout << "=== Local Weather Logger ===\n";
    cout << "Logs temperature (°C), humidity (%), and sky for each day.\n";
    cout << "File: weatherlog.txt (editable)\n";
    vector<WeatherEntry> db = loadLog("weatherlog.txt");

    while(true) {
        cout << "\nCommands: log, view, edit, weekly, monthly, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd=="log") {
            string date = todayDate();
            // Check for duplicate entry
            auto it = find_if(db.begin(),db.end(),[&](const WeatherEntry& e){return e.date==date;});
            if(it!=db.end()) { cout << "Already logged for today. Use edit.\n"; continue; }
            double t; int h; string sky;
            cout << "Temperature (°C): "; cin >> t;  cout << "Humidity (%): "; cin >> h;
            cout << "Sky (e.g. Clear, Cloudy, Rain): "; cin.ignore(); getline(cin, sky);
            db.emplace_back(date, t, h, sky);
            saveLog("weatherlog.txt", db);
            cout << "Logged.\n";
        } else if(cmd=="view") {
            if(db.empty()) { cout << "(No data)\n"; continue; }
            cout << "\nLast Entry: " << db.back().date
                 << "  " << db.back().tempC << "°C  " << db.back().humidity << "%  "
                 << db.back().sky << "\n";
        } else if(cmd=="edit") {
            cout << "Date to edit (YYYY-MM-DD): "; string date; cin >> date;
            auto it = find_if(db.begin(),db.end(),[&](const WeatherEntry& e){return e.date==date;});
            if(it==db.end()) { cout << "Not found.\n"; continue; }
            double t; int h; string sky;
            cout << "New temp (°C): "; cin >> t; cout << "New humidity: "; cin >> h;
            cout << "New sky: "; cin.ignore(); getline(cin, sky);
            it->tempC = t; it->humidity = h; it->sky = sky;
            saveLog("weatherlog.txt", db);
            cout << "Edited.\n";
        } else if(cmd=="weekly") {
            showSummary(db, 7);
        } else if(cmd=="monthly") {
            showSummary(db, 31);
        } else if(cmd=="quit") {
            saveLog("weatherlog.txt", db);
            cout << "All data saved. Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
