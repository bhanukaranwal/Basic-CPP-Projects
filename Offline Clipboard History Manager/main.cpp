/*
  Clipboard History Manager (main.cpp)
  --------------------------------------
  - Records everything copied to clipboard during program session
  - Supports saving/loading history ("clipboard_history.txt") for reuse between sessions
  - Lets user view, search, and select previous entries to "re-copy" (simulate paste)
  - Super handy for coders, writers, researchers, and daily desktop use!
  - 100% C++17, OS-independent (simulated clipboard), easily extensible to OS clipboard APIs

  Concepts: circular queue, search/filter, file I/O, simple CLI
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

struct ClipHistory {
    vector<string> history;
    unsigned int maxSize = 30;

    void add(const string& entry) {
        if(entry.empty()) return;
        if(!history.empty() && history.back() == entry) return;
        history.push_back(entry);
        if(history.size() > maxSize) history.erase(history.begin());
    }

    void printAll() const {
        cout << "\n--- Clipboard History (latest last) ---\n";
        int idx = 1;
        for(const auto& h : history)
            cout << idx++ << ") " << h << "\n";
    }

    void save(const string& fname) const {
        ofstream fout(fname, ios::trunc);
        for(const auto& h : history)
            fout << h << "\n---\n";
    }

    void load(const string& fname) {
        history.clear();
        ifstream fin(fname);
        string line, block;
        while(getline(fin,line)) {
            if(line == "---") {
                if(!block.empty()) { add(block); block.clear(); }
            } else {
                if(!block.empty()) block += "\n";
                block += line;
            }
        }
        if(!block.empty()) add(block);
    }

    vector<pair<int,string>> search(const string& term) const {
        vector<pair<int,string>> found;
        int idx = 1;
        for(const auto& h : history) {
            if(h.find(term) != string::npos)
                found.emplace_back(idx, h);
            ++idx;
        }
        return found;
    }

    string get(size_t n) const {
        if(n>=1 && n<=history.size())
            return history[n-1];
        return "";
    }
};

int main() {
    cout << "=== Clipboard History Manager (offline) ===\n";
    cout << "Stores up to 30 entries. Data saved as clipboard_history.txt.\n";
    ClipHistory ch;
    ch.load("clipboard_history.txt");

    string clipboard = ""; // Simulated current clipboard
    while(true) {
        cout << "\nCommands: copy, paste, show, search, select, save, load, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd=="copy") {
            cout << "Enter text to copy: ";
            cin.ignore();
            string tocopy; getline(cin, tocopy);
            clipboard = tocopy;
            ch.add(clipboard);
            cout << "[Copied]\n";
        } else if(cmd=="paste") {
            cout << "Clipboard: " << clipboard << "\n";
        } else if(cmd=="show") {
            ch.printAll();
        } else if(cmd=="search") {
            cout << "Enter search term: ";
            cin.ignore();
            string term; getline(cin, term);
            auto results = ch.search(term);
            if(results.empty()) cout << "No match.\n";
            else for(auto& r:results) cout << r.first << ") " << r.second << "\n";
        } else if(cmd=="select") {
            cout << "Which history # to move to clipboard (show first)? ";
            int idx; cin >> idx;
            string selected = ch.get(idx);
            if(selected.empty()) cout << "Invalid index.\n";
            else { clipboard=selected; cout << "Clipboard set to: " << clipboard << "\n"; }
        } else if(cmd=="save") {
            ch.save("clipboard_history.txt");
            cout << "History saved.\n";
        } else if(cmd=="load") {
            ch.load("clipboard_history.txt");
            cout << "Loaded past history.\n";
        } else if(cmd=="quit") {
            ch.save("clipboard_history.txt");
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
