#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

void printStack(const vector<int>& stk) {
    cout << "\nStack [top -> bottom]:\n";
    if (stk.empty()) {
        cout << "(empty)\n";
        return;
    }
    for (int i = (int)stk.size() - 1; i >= 0; --i)
        cout << "| " << setw(3) << stk[i] << " |\n";
    cout << "-----\n";
}

void printQueue(const queue<int>& q) {
    cout << "\nQueue [front -> back]:\n";
    if (q.empty()) {
        cout << "(empty)\n";
        return;
    }
    queue<int> temp = q;
    while (!temp.empty()) {
        cout << "[ " << temp.front() << " ] ";
        temp.pop();
    }
    cout << "<- back\n";
}

void stackMenu(vector<int>& stk) {
    while (true) {
        cout << "\n-- Stack Operations --\n";
        cout << "1. Push\n2. Pop\n3. Show\n4. Back\nChoose: ";
        int c; cin >> c;
        if (c == 1) {
            int val;
            cout << "Enter value to push: "; cin >> val;
            stk.push_back(val);
            printStack(stk);
        } else if (c == 2) {
            if (stk.empty()) cout << "Stack is empty.\n";
            else {
                cout << "Popped: " << stk.back() << "\n";
                stk.pop_back();
            }
            printStack(stk);
        } else if (c == 3) {
            printStack(stk);
        } else if (c == 4) break;
        else cout << "Invalid option.\n";
    }
}

void queueMenu(queue<int>& que) {
    while (true) {
        cout << "\n-- Queue Operations --\n";
        cout << "1. Enqueue\n2. Dequeue\n3. Show\n4. Back\nChoose: ";
        int c; cin >> c;
        if (c == 1) {
            int val;
            cout << "Enter value to enqueue: "; cin >> val;
            que.push(val);
            printQueue(que);
        } else if (c == 2) {
            if (que.empty()) cout << "Queue is empty.\n";
            else {
                cout << "Dequeued: " << que.front() << "\n";
                que.pop();
            }
            printQueue(que);
        } else if (c == 3) {
            printQueue(que);
        } else if (c == 4) break;
        else cout << "Invalid option.\n";
    }
}

int main() {
    vector<int> stk;
    queue<int> que;
    while (true) {
        cout << "\n=== Stack & Queue Visualizer ===\n";
        cout << "1. Stack\n2. Queue\n3. Exit\nChoose: ";
        int choice; cin >> choice;
        if (choice == 1) stackMenu(stk);
        else if (choice == 2) queueMenu(que);
        else if (choice == 3) { cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
