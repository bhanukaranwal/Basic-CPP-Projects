#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Message {
    string user;
    string text;
};

void printMenu() {
    cout << "\n--- Chat Room Simulator ---\n";
    cout << "1. Send Message\n";
    cout << "2. Show Chat History\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<Message> chat;
    int choice;

    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            Message msg;
            cout << "Enter your name: ";
            getline(cin, msg.user);
            if (msg.user.empty()) msg.user = "Anonymous";
            cout << "Enter your message: ";
            getline(cin, msg.text);

            chat.push_back(msg);
            cout << "Message sent!\n";
        }
        else if (choice == 2) {
            if (chat.empty()) {
                cout << "No messages yet.\n";
                continue;
            }
            cout << "\n--- Chat History ---\n";
            for (const auto& m : chat) {
                cout << left << setw(15) << m.user << ": " << m.text << endl;
            }
        }
        else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please enter 1-3.\n";
        }
    }
    return 0;
}
