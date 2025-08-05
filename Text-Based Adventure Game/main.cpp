#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

struct Room {
    string name;
    string description;
    map<string, string> exits; // direction -> room name
    bool hasTreasure = false;
};

void printStatus(const Room& room, bool hasTreasure) {
    cout << "\nYou are in the " << room.name << ".\n";
    cout << room.description << "\n";
    if (!room.exits.empty()) {
        cout << "Exits: ";
        for (const auto& ex : room.exits)
            cout << ex.first << " ";
        cout << "\n";
    }
    if (room.hasTreasure && !hasTreasure) {
        cout << "You see something shiny here!\n";
    }
}

int main() {
    // Define rooms
    map<string, Room> rooms = {
        {"Entrance", {"Entrance", "A cold stone entryway. Light filters in from the north.", {{"north","Hallway"}}, false}},
        {"Hallway", {"Hallway", "A torch-lit corridor. There are doors east and west.", {{"south","Entrance"}, {"east","Armory"}, {"west","Library"}, {"north","Throne"}}, false}},
        {"Armory", {"Armory", "A dusty room filled with old weapons.", {{"west","Hallway"}}, false}},
        {"Library", {"Library", "Tall bookshelves line the walls, filled with ancient tomes.", {{"east","Hallway"}}, true}},
        {"Throne", {"Throne Room", "A grand hall with a golden throne.", {{"south","Hallway"}}, true}}
    };

    string current = "Entrance";
    bool gotTreasure = false;
    set<string> inventory;

    cout << "--- Welcome to the Adventure Game ---\n";
    cout << "Explore the dungeon, collect treasures, and try to find your way out!\nCommands: go [direction], take, inventory, help, quit\n";

    while (true) {
        printStatus(rooms[current], gotTreasure);

        cout << "\n> ";
        string cmd;
        getline(cin, cmd);

        if (cmd.find("go ") == 0) {
            string dir = cmd.substr(3);
            if (rooms[current].exits.count(dir)) {
                current = rooms[current].exits[dir];
                cout << "You move " << dir << ".\n";
            } else {
                cout << "You can't go that way.\n";
            }
        } else if (cmd == "take") {
            if (rooms[current].hasTreasure && !gotTreasure) {
                cout << "You picked up a mysterious treasure!\n";
                gotTreasure = true;
                inventory.insert("treasure");
            } else {
                cout << "There's nothing to take here.\n";
            }
        } else if (cmd == "inventory") {
            cout << "You have: ";
            if (inventory.empty()) cout << "nothing.\n";
            else for (auto& it : inventory) cout << it << " ";
            cout << "\n";
        } else if (cmd == "help") {
            cout << "Commands: go [direction], take, inventory, help, quit\n";
        } else if (cmd == "quit") {
            cout << "Thanks for playing!\n";
            break;
        } else {
            cout << "I don't understand that command.\n";
        }
    }
    return 0;
}
