/*
  Meeting/Class Attendance Tracker (main.cpp)
  ---------------------------------------------
  - Enables easy tracking of members' attendance across sessions.
  - CLI: add/remove participants, log attendance, annotate absences.
  - Shows session and member summaries.
  - Data is persisted in "attendance.txt".
  - Pure C++17, single file, extensible for any group.

  Concepts: file I/O, data parsing, summary reporting, CLI design.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <map>
using namespace std;

struct Session {
    std::string date; // YYYY-MM-DD
    std::map<std::string, std::string> status; // name -> "Present"/"Absent:reason"
};

vector<std::string> loadNames(const string& fname) {
    vector<string> names;
    ifstream fin(fname + "_names.txt");
    string n;
    while(getline(fin,n)) if(!n.empty()) names.push_back(n);
    return names;
}
void saveNames(const string& fname, const vector<string>& names) {
    ofstream fout(fname + "_names.txt", ios::trunc);
    for(const auto& n : names) fout << n << "\n";
}

vector<Session> loadAttendance(const string& fname) {
    vector<Session> data;
    ifstream fin(fname + ".txt");
    string line;
    while(getline(fin,line)) {
        if(line.find("DATE") == 0) {
            Session s;
            stringstream ss(line);
            string _; ss >> _ >> s.date;
            while(getline(fin,line) && !line.empty()) {
                string name,status; // name status (reason optional)
                stringstream lss(line);
                lss >> std::ws;
                getline(lss, name, '|');
                lss >> std::ws;
                getline(lss, status);
                if(!name.empty())
                    s.status[name]=status;
            }
            data.push_back(s);
        }
    }
    return data;
}
void saveAttendance(const string& fname, const vector<Session>& data) {
    ofstream fout(fname + ".txt", ios::trunc);
    for(const auto& s : data) {
        fout << "DATE " << s.date << "\n";
        for(const auto& kv : s.status)
            fout << kv.first << "|" << kv.second << "\n";
        fout << "\n";
    }
}

string todayDate() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

void printSummary(const vector<string>& names, const vector<Session>& data) {
    if(data.empty()) { cout << "(No sessions yet)\n"; return; }
    cout << "\nAttendance Summary Table:\n";
    cout << setw(15) << "Name";
    for(const auto& s : data) cout << "| " << s.date;
    cout << " | Total Present\n";
    for(const auto& n : names) {
        cout << setw(15) << n;
        int present=0;
        for(const auto& s : data) {
            auto it = s.status.find(n);
            if(it != s.status.end())
                if(it->second.find("Present") == 0) { cout << "|   P"; ++present; }
                else                                { cout << "|   A"; }
            else cout << "|    ";
        }
        cout << " |    " << present << "\n";
    }
    cout << endl;
}

int main() {
    cout << "=== Meeting/Class Attendance Tracker ===\n";
    cout << "Group and session records are persistent (attendance.txt, attendance_names.txt)\n";

    string fname = "attendance";
    vector<string> names = loadNames(fname);
    vector<Session> data = loadAttendance(fname);

    while(true) {
        cout << "\nCommands: add, remove, list, new, annotate, summary, sessions, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd=="add") {
            cout << "Participant name: ";
            string name; cin.ignore(); getline(cin, name);
            if(find(names.begin(), names.end(), name) != names.end())
                cout << "Already exists.\n";
            else {
                names.push_back(name);
                saveNames(fname, names);
                cout << name << " added.\n";
            }
        } else if(cmd=="remove") {
            cout << "Name to remove: ";
            string name; cin.ignore(); getline(cin, name);
            auto it=find(names.begin(),names.end(),name);
            if(it == names.end()) cout << "Not found.\n";
            else {
                names.erase(it);
                saveNames(fname, names);
                cout << "Removed.\n";
            }
        } else if(cmd=="list") {
            cout << "Participants:\n";
            for(auto& n:names) cout << "  "<<n<<"\n";
        } else if(cmd=="new") {
            string date = todayDate();
            cout << "Session date (default: "<<date<<")? Enter to accept: ";
            string input; cin.ignore(); getline(cin, input);
            if(!input.empty()) date = input;
            Session s; s.date = date;
            for(const auto& n: names) {
                cout << n << " (P=present, A=absent): ";
                string resp; getline(cin, resp);
                if(resp=="P"||resp=="p") s.status[n]="Present";
                else if(resp=="A"||resp=="a") {
                    cout << "Reason for absence (ENTER to skip): ";
                    string reason; getline(cin,reason);
                    s.status[n]="Absent"+(reason.empty()?"":(": "+reason));
                }
            }
            data.push_back(s);
            saveAttendance(fname, data);
            cout << "Session recorded for "<<date<<".\n";
        } else if(cmd=="annotate") {
            cout << "Session date to annotate: ";
            string date; cin.ignore(); getline(cin, date);
            auto it = find_if(data.begin(), data.end(), [&](const Session& s){return s.date==date;});
            if(it == data.end()) { cout << "Session not found.\n"; continue; }
            cout << "Name and new status (e.g., 'Jane Present' or 'Paul Absent: sick'): ";
            string name, newstat;
            getline(cin, name, ' '); getline(cin, newstat);
            if(it->status.find(name)==it->status.end()) { cout << "Not a participant.\n"; continue; }
            it->status[name]=newstat;
            saveAttendance(fname, data);
            cout << "Status updated.\n";
        } else if(cmd=="summary") {
            printSummary(names, data);
        } else if(cmd=="sessions") {
            cout << "Sessions recorded:\n";
            for(const auto& s:data) cout << "  " << s.date << "\n";
        } else if(cmd=="quit") break;
        else cout << "Unknown command.\n";
    }
    cout << "All data saved. Goodbye!\n";
    return 0;
}
