#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Song {
    string title;
    string artist;
};

void printMenu() {
    cout << "\n--- Music Playlist Manager ---\n";
    cout << "1. Add Song\n";
    cout << "2. View Playlist\n";
    cout << "3. Play Next Song\n";
    cout << "4. Play Previous Song\n";
    cout << "5. Remove Song\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

void showPlaylist(const vector<Song>& playlist, int current) {
    if (playlist.empty()) {
        cout << "Playlist is empty.\n";
        return;
    }
    cout << "\n--- Playlist ---\n";
    for (size_t i = 0; i < playlist.size(); ++i) {
        cout << (i+1) << ". " 
             << (i == current ? ">> " : "   ")
             << "\"" << playlist[i].title << "\" by " << playlist[i].artist << "\n";
    }
}

int main() {
    vector<Song> playlist;
    int current = -1; // -1 means nothing is currently "playing"
    int choice;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) { // Add song
            Song s;
            cout << "Enter song title: "; getline(cin, s.title);
            cout << "Enter artist: "; getline(cin, s.artist);
            playlist.push_back(s);
            if (current == -1) current = 0; // Start from first song
            cout << "Song added!\n";
        }
        else if (choice == 2) { // View
            showPlaylist(playlist, current);
        }
        else if (choice == 3) { // Next song
            if (playlist.empty()) { cout << "Playlist is empty.\n"; continue; }
            current = (current + 1) % playlist.size();
            cout << "Now playing: \"" << playlist[current].title << "\" by " << playlist[current].artist << "\n";
        }
        else if (choice == 4) { // Previous song
            if (playlist.empty()) { cout << "Playlist is empty.\n"; continue; }
            current = (current - 1 + playlist.size()) % playlist.size();
            cout << "Now playing: \"" << playlist[current].title << "\" by " << playlist[current].artist << "\n";
        }
        else if (choice == 5) { // Remove song
            showPlaylist(playlist, current);
            if (playlist.empty()) continue;
            cout << "Enter song number to remove: ";
            int n; cin >> n; cin.ignore();
            if (n >= 1 && n <= (int)playlist.size()) {
                playlist.erase(playlist.begin() + (n-1));
                if (playlist.empty())
                    current = -1;
                else if (current >= (int)playlist.size())
                    current = 0;
                cout << "Removed.\n";
            } else {
                cout << "Invalid number.\n";
            }
        }
        else if (choice == 6) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-6.\n";
        }
    }
    return 0;
}
