/*
  Media Metadata Batch Editor (main.cpp)
  ------------------------------------------
  - Scans chosen directory for media files (MP3, JPG, PNG, MP4, MOV)
  - Reads/writes metadata sidecar files: "<file>.meta"
  - Lets user display, batch-edit, and mass-tag files from the console
  - Changes are logged to "meta_edit_log.txt"
  - 100% C++17 (requires <filesystem>), extensible for binary metadata editing

  Concepts: file I/O, filtering, batch editing, directory traversal
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <ctime>
using namespace std;
namespace fs = std::filesystem;

const vector<string> MEDIA_EXTS = {".mp3",".wav",".flac",".jpg",".jpeg",".png",".mp4",".mov",".avi"};

struct Meta {
    string title, artist, date, tags;
    string tostr() const {
        ostringstream ss;
        ss << "Title:  " << title << "\n"
           << "Artist: " << artist << "\n"
           << "Date:   " << date << "\n"
           << "Tags:   " << tags << "\n";
        return ss.str();
    }
};

// Read .meta sidecar file, or make a blank one
Meta readMeta(const fs::path& pf) {
    Meta m;
    ifstream fin(pf.string() + ".meta");
    string line;
    while(getline(fin,line)) {
        if(line.find("Title:") == 0)   m.title = line.substr(6);
        if(line.find("Artist:") == 0)  m.artist = line.substr(7);
        if(line.find("Date:") == 0)    m.date = line.substr(5);
        if(line.find("Tags:") == 0)    m.tags = line.substr(5);
    }
    return m;
}
void writeMeta(const fs::path& pf, const Meta& m) {
    ofstream fout(pf.string() + ".meta", ios::trunc);
    fout << "Title:  " << m.title << "\n"
         << "Artist: " << m.artist << "\n"
         << "Date:   " << m.date << "\n"
         << "Tags:   " << m.tags << "\n";
}

vector<fs::path> scanMedia(const fs::path& dir) {
    vector<fs::path> files;
    for(auto& entry : fs::directory_iterator(dir))
        if(entry.is_regular_file())
            if(find(MEDIA_EXTS.begin(), MEDIA_EXTS.end(), entry.path().extension().string()) != MEDIA_EXTS.end())
                files.push_back(entry.path());
    return files;
}

string todayDate() {
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", lt);
    return string(buf);
}

void logEdit(const string& entry) {
    ofstream fout("meta_edit_log.txt", ios::app);
    fout << todayDate() << " - " << entry << "\n";
}

int main() {
    cout << "=== Media Metadata Batch Editor ===\n";
    cout << "Scans and updates metadata .meta files for audio, image, video files.\n";
    cout << "Enter directory to scan (blank for .): ";
    string dirname; getline(cin, dirname);
    fs::path dir = dirname.empty() ? fs::current_path() : dirname;
    if(!fs::is_directory(dir)) { cout << "Invalid directory.\n"; return 1; }

    vector<fs::path> files = scanMedia(dir);
    if(files.empty()) { cout << "No media files found.\n"; return 0; }

    cout << "Found " << files.size() << " media files.\n";
    while(true) {
        cout << "\nCommands: list, view, edit, tag, log, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd == "list") {
            for(size_t i=0;i<files.size();++i)
                cout << setw(2) << i+1 << ". " << files[i].filename().string() << "\n";
        } else if(cmd == "view") {
            cout << "Which file # (list for indices)? ";
            int idx; cin >> idx;
            if(idx<1||idx>(int)files.size()) { cout << "Out of range.\n"; continue; }
            Meta m = readMeta(files[idx-1]);
            cout << files[idx-1].filename().string() << " Metadata:\n" << m.tostr();
        } else if(cmd == "edit") {
            cout << "Which file # (or 0 for ALL)? ";
            int idx; cin >> idx;
            vector<int> inds;
            if(idx==0) { for(int i=0;i<files.size();++i) inds.push_back(i); }
            else if(idx>0 && idx<=files.size()) inds.push_back(idx-1);
            else { cout << "Invalid.\n"; continue; }
            for(int i : inds) {
                Meta m = readMeta(files[i]);
                cout << "Editing: " << files[i].filename().string() << "\n";
                cout << "(blank to skip, old in brackets)\n";
                cout << "Title [" << m.title << "]: "; cin.ignore(); string t; getline(cin,t);
                cout << "Artist [" << m.artist << "]: "; string ar; getline(cin,ar);
                cout << "Date [" << m.date << "]: "; string d; getline(cin,d);
                cout << "Tags [" << m.tags << "]: "; string tag; getline(cin,tag);
                if(!t.empty()) m.title = t;
                if(!ar.empty()) m.artist = ar;
                if(!d.empty()) m.date = d;
                if(!tag.empty()) m.tags = tag;
                writeMeta(files[i], m);
                logEdit("Edited: " + files[i].filename().string());
                cout << "Updated.\n";
            }
        } else if(cmd == "tag") {
            cout << "Global tags to add: "; cin.ignore(); string t; getline(cin, t);
            for(auto& f : files) {
                Meta m = readMeta(f);
                if(m.tags.find(t)==string::npos) { m.tags+=(m.tags.empty()?"":", ")+t; writeMeta(f,m); logEdit("Tagged "+f.filename().string()); }
            }
            cout << "All files tagged with '"<<t<<"'.\n";
        } else if(cmd == "log") {
            ifstream fin("meta_edit_log.txt");
            cout << "\n--- Edit Log ---\n";
            string l; while(getline(fin,l)) cout << l << "\n";
        } else if(cmd == "quit") {
            cout << "All changes saved. Goodbye!\n";
            break;
        } else cout << "Unknown command.\n";
    }
    return 0;
}
