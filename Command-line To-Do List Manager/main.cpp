#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Task {
    string description;
    bool done;
    int priority; // 1 (high) to 5 (low)
};

void printMenu() {
    cout << "\n--- To-Do List Manager ---\n";
    cout << "1. Add Task\n";
    cout << "2. View All Tasks\n";
    cout << "3. Mark Task as Done\n";
    cout << "4. Remove Task\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void showTasks(const vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return;
    }
    cout << "\n#  [Done]  Priority  Description\n";
    cout << "-------------------------------------------\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << i + 1 << "    "
             << (tasks[i].done ? "[X]" : "[ ]") << "        "
             << tasks[i].priority << "      "
             << tasks[i].description << endl;
    }
}

int main() {
    vector<Task> tasks;
    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(); // Clear input buffer

        if (choice == 1) { // Add
            Task t;
            cout << "Enter task description: ";
            getline(cin, t.description);
            cout << "Enter priority (1: High ... 5: Low): ";
            cin >> t.priority;
            if (t.priority < 1 || t.priority > 5) t.priority = 3;
            t.done = false;
            tasks.push_back(t);
            cout << "Task added!\n";
        } 
        else if (choice == 2) { // View
            showTasks(tasks);
        } 
        else if (choice == 3) { // Mark done
            showTasks(tasks);
            cout << "Enter task number to mark as done: ";
            size_t num;
            cin >> num;
            if (!cin || num < 1 || num > tasks.size()) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Invalid number.\n";
                continue;
            }
            tasks[num - 1].done = true;
            cout << "Task marked as done!\n";
        } 
        else if (choice == 4) { // Remove
            showTasks(tasks);
            cout << "Enter task number to remove: ";
            size_t num;
            cin >> num;
            if (!cin || num < 1 || num > tasks.size()) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Invalid number.\n";
                continue;
            }
            tasks.erase(tasks.begin() + (num - 1));
            cout << "Task removed!\n";
        } 
        else if (choice == 5) { // Exit
            cout << "Goodbye! Happy task managing!\n";
            break;
        } 
        else {
            cout << "Invalid choice. Please select between 1-5.\n";
        }
    }
    return 0;
}
