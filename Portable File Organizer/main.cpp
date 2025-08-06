/*
  Portable File Organizer (main.cpp)
  -------------------------------------
  - Scans a directory for files
  - Sorts and moves files into folders by extension, date, or keyword in filename
  - Suggests/creates destination folders automatically
  - Shows a summary before applying changes; user can preview or commit
  - All C++17 standard library, cross-platform (Windows/Linux/Mac with minor path changes)

  Concepts: directory traversal, string ops, file moving, user confirmation, extensibility

  Note: Requires C++17 for <filesystem>
*/

#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <set>
using namespace std;
namespace fs = std::filesystem;

// Lowercase file extension
string extLower(const fs::path& p) {
    string e = p.extension().string();
    transform(e.begin(), e.end(), e.begin(), ::tolower);
    return e;
}

// Extract keywords in filename (split by '_', '-', or ' ')
vector<string> extractKeywords(const fs::path& p) {
    string name = p.stem().string();
    for (auto& c : name) if (c == '-' || c == '_') c = ' ';
    istringstream iss(name);
    vector<string> keys;
    string w;
    while (iss >> w)
        if (w.size() > 2)
            keys.push_back(w);
    return keys;
}

void organizeByExt(const fs::path& dir, bool dryRun = true) {
    map<string, vector<fs::path>> groups;
    for (auto& entry : fs::directory_iterator(dir))
        if (entry.is_regular_file())
            groups[extLower(entry.path())].push_back(entry.path());
    cout << "\nOrganizing by file extension:\n";
    for (const auto& kv : groups)
        cout << "  " << setw(8) << (kv.first.empty() ? "[none]" : kv.first) << " : " << kv.second.size() << " files\n";

    if (dryRun) {
        cout << "\nThis is a preview. Proceed with changes? (yes/no): ";
        string resp; cin >> resp;
        if (resp != "yes") { cout << "Aborted!\n"; return; }
    }

    for (const auto& kv : groups) {
        string folder = kv.first.empty() ? "other" : kv.first.substr(1) + "_files";
        fs::create_directory(dir / folder);
        for (const auto& f : kv.second) {
            fs::path dest = dir / folder / f.filename();
            fs::rename(f, dest);
            cout << "  Moved: " << f.filename() << " → " << folder << "/\n";
        }
    }
    cout << "Organization complete.\n";
}

void organizeByDate(const fs::path& dir, bool dryRun = true) {
    map<string, vector<fs::path>> groups; // date string → files
    for (auto& entry : fs::directory_iterator(dir))
        if (entry.is_regular_file()) {
            auto ftime = fs::last_write_time(entry.path());
            std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
            tm* lt = localtime(&cftime);
            char buf[12];
            strftime(buf,12,"%Y-%m-%d",lt);
            groups[string(buf)].push_back(entry.path());
        }
    cout << "\nOrganizing by modification date:\n";
    for (const auto& kv : groups)
        cout << "  " << kv.first << " : " << kv.second.size() << " files\n";

    if (dryRun) {
        cout << "\nPreview. Proceed with changes? (yes/no): ";
        string resp; cin >> resp;
        if (resp != "yes") { cout << "Aborted!\n"; return; }
    }
    for (const auto& kv : groups) {
        string folder = "date_" + kv.first;
        fs::create_directory(dir / folder);
        for (const auto& f : kv.second) {
            fs::path dest = dir / folder / f.filename();
            fs::rename(f, dest);
            cout << "  Moved: " << f.filename() << " → " << folder << "/\n";
        }
    }
    cout << "Done. Files are now grouped by date.\n";
}

void organizeByKeyword(const fs::path& dir, bool dryRun = true) {
    map<string, vector<fs::path>> groups;
    set<string> allKeywords;
    for (auto& entry : fs::directory_iterator(dir))
        if (entry.is_regular_file()) {
            auto keys = extractKeywords(entry.path());
            for (auto& k : keys) {
                groups[k].push_back(entry.path());
                allKeywords.insert(k);
            }
        }
    cout << "Found keywords: ";
    for (const auto& k : allKeywords) cout << k << " "; cout << endl;
    if (groups.empty()) { cout << "No keywords found.\n"; return; }
    cout << "\nFile count by keyword:\n";
    for (const auto& kv : groups) cout << "  " << kv.first << " : " << kv.second.size() << " files\n";

    if (dryRun) {
        cout << "\nPreview. Proceed with changes? (yes/no): ";
        string resp; cin >> resp;
        if (resp != "yes") { cout << "Aborted!\n"; return; }
    }

    for (const auto& kv : groups) {
        string folder = "kw_" + kv.first;
        fs::create_directory(dir / folder);
        for (const auto& f : kv.second) {
            fs::path dest = dir / folder / f.filename();
            fs::rename(f, dest);
            cout << "  Moved: " << f.filename() << " → " << folder << "/\n";
        }
    }
    cout << "Done. Files are now grouped by keyword.\n";
}

int main() {
    cout << "=== Portable File Organizer ===\n";
    cout << "Enter the path to organize (blank for current directory): ";
    string dirstr;
    getline(cin, dirstr);
    fs::path dir = dirstr.empty() ? fs::current_path() : dirstr;
    cout << "Target: " << dir << "\n";
    if (!fs::is_directory(dir)) { cout << "Not a directory!\n"; return 1; }

    cout << "\nOrganization mode:\n";
    cout << "  1 = By extension  (e.g. pdf, docx)\n";
    cout << "  2 = By date       (last modified)\n";
    cout << "  3 = By keyword    (in filename)\n";
    cout << "> ";
    int mode; cin >> mode;

    switch(mode) {
        case 1: organizeByExt(dir); break;
        case 2: organizeByDate(dir); break;
        case 3: organizeByKeyword(dir); break;
        default: cout << "Unknown mode.\n";
    }

    cout << "All done! Tidy disk, tidy mind.\n";
    return 0;
}
