#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
namespace fs = std::filesystem;
using namespace std;

void listDirectory(const fs::path& path) {
    cout << "\nListing for: " << path << "\n\n";
    cout << left << setw(35) << "Name"
         << setw(10) << "Type"
         << setw(15) << "Size (bytes)"
         << setw(25) << "Last Modified"
         << endl;
    cout << string(90, '-') << endl;

    for (const auto& entry : fs::directory_iterator(path)) {
        string type = entry.is_directory() ? "Folder" : "File";
        auto ftime = entry.last_write_time();
        time_t cftime = decltype(ftime)::clock::to_time_t(ftime);

        cout << left << setw(35) << entry.path().filename().string()
             << setw(10) << type
             << setw(15) << (entry.is_directory() ? "-" : to_string(entry.file_size()))
             << setw(25) << put_time(localtime(&cftime), "%Y-%m-%d %H:%M:%S")
             << endl;
    }
}

int main() {
    string input;
    while (true) {
        cout << "\n--- Simple File System Explorer (C++17) ---\n";
        cout << "Enter directory path to explore (or blank for current, 'exit' to quit): ";
        getline(cin, input);
        if (input == "exit") break;
        if (input.empty()) input = ".";
        fs::path dir(input);
        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            cout << "Directory not found or invalid.\n";
            continue;
        }
        listDirectory(dir);
    }
    cout << "Goodbye!\n";
    return 0;
}
