/*
  Offline Dictionary & Vocabulary Builder (main.cpp)
  ---------------------------------------------------
  - Search for word meanings (built-in + custom additions)
  - Add new words/definitions
  - Tracks "word of the day", custom lists, and quizzes user
  - Data is persisted in "mydictionary.txt"
  - C++17, single file, modular for easy growth
*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
using namespace std;

// --------- Vocabulary Data ----------
struct Dictionary {
    map<string, string> words;
    vector<string> wotd_log; // historical words of the day

    void load(const string& fname) {
        ifstream fin(fname);
        words.clear();
        string line, word, def;
        while(getline(fin, line)) {
            if(line.empty()) continue;
            size_t sep = line.find('|');
            if(sep != string::npos) {
                word = line.substr(0, sep);
                def = line.substr(sep+1);
                words[word] = def;
            }
        }
        // Load word of the day log
        ifstream flog(fname + ".wotd");
        while(getline(flog, line))
            wotd_log.push_back(line);
    }

    void save(const string& fname) {
        ofstream fout(fname, ios::trunc);
        for(const auto& kv:words)
            fout << kv.first << "|" << kv.second << "\n";
        ofstream flog(fname + ".wotd", ios::trunc);
        for(const auto& w : wotd_log) flog << w << "\n";
    }

    bool exists(const string& word) {
        return words.find(word) != words.end();
    }

    void insert(const string& word, const string& def) {
        words[word]=def;
    }

    string getDef(const string& word) {
        auto it = words.find(word);
        return it != words.end() ? it->second : "(no definition found)";
    }

    // Pick a random word for WOTD
    string wordOfDay() {
        if(words.empty()) return "(no words yet)";
        static mt19937 rng(static_cast<unsigned>(time(0)));
        vector<string> keys;
        for(auto& kv: words) keys.push_back(kv.first);
        string pick = keys[rng()%keys.size()];
        wotd_log.push_back(pick + " : " + words[pick]);
        if(wotd_log.size() > 60) wotd_log.erase(wotd_log.begin());
        return pick;
    }

    void showWOTDLog(int n=10) {
        cout << "\n--- Recent Words of the Day ---\n";
        int start = wotd_log.size()>n ? wotd_log.size()-n : 0;
        for(size_t i=start; i<wotd_log.size(); ++i) cout << "  " << wotd_log[i] << "\n";
        cout << endl;
    }

    void quiz(int count=5) {
        if(words.empty()) { cout << "(No words to quiz!)\n"; return; }
        static mt19937 rng(static_cast<unsigned>(time(0)));
        vector<string> keys;
        for(auto& kv:words) keys.push_back(kv.first);
        shuffle(keys.begin(), keys.end(), rng);
        int correct=0;
        for(int i=0;i<count&&i<keys.size();++i) {
            cout << "\nDefine: " << keys[i] << "\n> ";
            string ans; getline(cin, ans);
            string def = words[keys[i]];
            cout << "Definition: " << def << "\n";
            if(ans.size() && def.find(ans)!=string::npos) ++correct;
        }
        cout << "\nQuiz done: " << correct << " of " << min(count,(int)keys.size()) << " correct.\n";
    }
};

// ----------- Built-in seed data -----------
void loadSeed(Dictionary& dict) {
    if(!dict.words.empty()) return;
    dict.words["elucidate"] = "make (something) clear; explain";
    dict.words["ephemeral"] = "lasting for a very short time";
    dict.words["ubiquitous"] = "present, appearing, or found everywhere";
    dict.words["serendipity"] = "the occurrence of fortunate events by chance";
    dict.words["plethora"] = "a large or excessive amount";
    dict.words["enigma"] = "a person or thing that is mysterious or difficult to understand";
    dict.words["mellifluous"] = "sweet or musical; pleasant to hear";
    dict.words["alacrity"] = "brisk and cheerful readiness";
}

// ------------------------------------------
int main() {
    cout << "=== Offline Dictionary & Vocabulary Builder ===\n";
    cout << "Words, quiz, and word-of-the-day saved in mydictionary.txt\n";
    Dictionary dict;
    dict.load("mydictionary.txt");
    loadSeed(dict);

    while(true) {
        cout << "\nCommands: lookup, add, wotd, wotdlog, quiz, all, quit\n> ";
        string cmd; cin >> cmd;
        if(cmd=="lookup") {
            cout << "Word to lookup: ";
            string w; cin.ignore(); getline(cin,w);
            transform(w.begin(), w.end(), w.begin(), ::tolower);
            cout << "Meaning: " << dict.getDef(w) << "\n";
        } else if(cmd=="add") {
            cout << "Word to add: ";
            string w; cin.ignore(); getline(cin,w);
            transform(w.begin(), w.end(), w.begin(), ::tolower);
            cout << "Definition: ";
            string d; getline(cin, d);
            dict.insert(w, d);
            cout << w << " added.\n";
            dict.save("mydictionary.txt");
        } else if(cmd=="all") {
            cout << "\n--- All Words ---\n";
            for(auto& kv : dict.words)
                cout << "  " << setw(15) << kv.first << " : " << kv.second << "\n";
        } else if(cmd=="wotd") {
            string w = dict.wordOfDay();
            cout << "\nWord of the Day: " << w << " = " << dict.words[w] << endl;
            dict.save("mydictionary.txt");
        } else if(cmd=="wotdlog") {
            dict.showWOTDLog();
        } else if(cmd=="quiz") {
            cin.ignore();
            dict.quiz();
        } else if(cmd=="quit") {
            dict.save("mydictionary.txt");
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
