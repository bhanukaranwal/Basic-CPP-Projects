#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Task {
    string description;
    bool completed = false;
};

void printMenu() {
    cout << "\n--- To-Do List Manager ---\n";
    cout << "1. Add Task\n";
    cout << "2. View Tasks\n";
    cout << "3. Mark Task as Completed\n";
    cout << "4. Delete Task\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void showTasks(const vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << "No tasks found.\n";
        return;
    }
    cout << left << setw(4) << "No." << setw(40) << "Task Description" << "Status\n";
    cout << string(55, '-') << endl;
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << setw(4) << i+1
             << setw(40) << tasks[i].description
             << (tasks[i].completed ? "Completed" : "Pending") << endl;
    }
}

int main() {
    vector<Task> tasks;
    int choice;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) { // Add
            Task t;
            cout << "Enter task description: ";
            getline(cin, t.description);
            tasks.push_back(t);
            cout << "Task added!\n";
        }
        else if (choice == 2) { // View
            showTasks(tasks);
        }
        else if (choice == 3) { // Complete
            showTasks(tasks);
            if (tasks.empty()) continue;
            cout << "Enter task number to mark as completed: ";
            int n; cin >> n; cin.ignore();
            if (n >= 1 && n <= (int)tasks.size()) {
                tasks[n-1].completed = true;
                cout << "Task marked as completed.\n";
            } else {
                cout << "Invalid.\n";
            }
        }
        else if (choice == 4) { // Delete
            showTasks(tasks);
            if (tasks.empty()) continue;
            cout << "Enter task number to delete: ";
            int n; cin >> n; cin.ignore();
            if (n >= 1 && n <= (int)tasks.size()) {
                tasks.erase(tasks.begin() + n - 1);
                cout << "Task deleted.\n";
            } else {
                cout << "Invalid.\n";
            }
        }
        else if (choice == 5) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid choice. Enter 1-5.\n";
        }
    }
    return 0;
}
