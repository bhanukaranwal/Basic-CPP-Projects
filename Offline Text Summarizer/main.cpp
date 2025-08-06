/*
  Offline Text Summarizer (main.cpp)
  --------------------------------------
  - User pastes or types text
  - Summarizes using basic term frequency + scoring
  - Shows sentence ranking, top keyword highlights
  - All C++17, single file, no dependencies
  - Extendable via NLP libraries (if desired)

  Concepts: string processing, term counting, heuristic scoring, I/O
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <locale>
using namespace std;

// -- Utility: lowercased, alpha-only word
string normWord(const string& w) {
    string nw;
    for (auto c : w)
        if (isalpha((unsigned char)c)) nw += tolower(c);
    return nw;
}

// Tokenize string to sentences and words
vector<string> splitSentences(const string& txt) {
    vector<string> out;
    istringstream ss(txt); string sent, token;
    while (getline(ss, sent, '.')) {
        while (!sent.empty() && isspace(sent.back())) sent.pop_back();
        if (sent.size() > 10) // exclude empty
            out.push_back(sent + ".");
    }
    return out;
}
vector<string> splitWords(const string& sent) {
    vector<string> out;
    istringstream ws(sent); string w;
    while (ws >> w) {
        w = normWord(w);
        if (w.size() > 2) out.push_back(w); // ignore short/stop
    }
    return out;
}

// Score sentences by word frequency (TF)
vector<pair<int, string>> scoreSentences(const vector<string>& sents, map<string,int>& wordFreq) {
    vector<pair<int, string>> scored;
    for (const auto& s : sents) {
        int score = 0;
        for (const auto& w : splitWords(s))
            score += wordFreq[w];
        scored.emplace_back(score, s);
    }
    sort(scored.rbegin(), scored.rend()); // highest first
    return scored;
}

int main() {
    cout << "=== Offline Text Summarizer ===\n";
    cout << "(Paste or type your text, END with a line containing only 'END')\n";

    // Read text
    string txt, line;
    while (getline(cin, line)) {
        if (line == "END") break;
        txt += line + "\n";
    }

    // Tokenize
    vector<string> sents = splitSentences(txt);
    map<string, int> wordFreq;
    for (const auto& s : sents)
        for (const auto& w : splitWords(s))
            ++wordFreq[w];
    // Remove common stopwords for keyword highlight
    set<string> stopwords = {"the","and","for","with","that","this","but","not","are","was","you","all","can","has","have","had","from","your","also","their","they","will","who","out","use","any","one","two","its"};
    vector<pair<int, string>> keywords;
    for (const auto& kv : wordFreq)
        if (kv.second > 1 && stopwords.find(kv.first) == stopwords.end())
            keywords.emplace_back(kv.second, kv.first);
    sort(keywords.rbegin(), keywords.rend());

    // Score sentences
    auto scored = scoreSentences(sents, wordFreq);

    // -- Output --
    cout << "\nText summary:\n";
    size_t summaryLen = min(size_t(4), scored.size());
    for (size_t i = 0; i < summaryLen; ++i)
        cout << "- " << scored[i].second << "\n";
    cout << "\nKey terms: ";
    for (size_t i = 0; i < min(size_t(8), keywords.size()); ++i)
        cout << keywords[i].second << (i+1<min(size_t(8),keywords.size())?", ":"");
    cout << "\n\nSentences ranked by importance:\n";
    for (size_t i = 0; i < min(size_t(8),scored.size()); ++i)
        cout << setw(2) << i+1 << ". " << scored[i].second << "\n";

    cout << "\nDone. Edit/extend for advanced NLP as needed!\n";
    return 0;
}
