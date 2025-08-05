#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Convert a single line of markdown to HTML
string md2html(const string& md) {
    // Headers
    if (md.substr(0,3) == "###")
        return "<h3>" + md.substr(3) + "</h3>";
    if (md.substr(0,2) == "##")
        return "<h2>" + md.substr(2) + "</h2>";
    if (md.substr(0,1) == "#")
        return "<h1>" + md.substr(1) + "</h1>";
    // Unordered list
    if (md.substr(0,2) == "- ") 
        return "<li>" + md.substr(2) + "</li>";

    // Inline bold (**bold**)
    string res = md;
    res = regex_replace(res, regex(R"(\*\*(.*?)\*\*)"), R"(<b>$1</b>)");
    // Inline italics (*italics*)
    res = regex_replace(res, regex(R"(\*(.*?)\*)"), R"(<i>$1</i>)");
    return "<p>" + res + "</p>";
}

int main() {
    cout << "--- Markdown to HTML Converter ---\n";
    cout << "Type markdown lines (blank line to finish):\n";

    vector<string> input_lines, html_lines;
    string line;
    bool inUL = false;

    while (getline(cin, line)) {
        if (line.empty()) break;
        input_lines.push_back(line);
    }

    for (size_t i = 0; i < input_lines.size(); ++i) {
        if (input_lines[i].substr(0,2) == "- ") {
            if (!inUL) {
                html_lines.push_back("<ul>");
                inUL = true;
            }
            html_lines.push_back(md2html(input_lines[i]));
            // Peek next or end
            if (i+1 == input_lines.size() || input_lines[i+1].substr(0,2) != "- ") {
                html_lines.push_back("</ul>");
                inUL = false;
            }
        } else {
            html_lines.push_back(md2html(input_lines[i]));
        }
    }

    cout << "\n--- HTML Output ---\n";
    for (const auto& h : html_lines)
        cout << h << endl;
    return 0;
}
