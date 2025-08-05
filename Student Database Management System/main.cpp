#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Student {
    int id;
    string name;
    vector<int> marks;
};

void printMenu() {
    cout << "\n--- Student Database Manager ---\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Update Student\n";
    cout << "4. Delete Student\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}

void showStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students in the database.\n";
        return;
    }
    cout << left << setw(5) << "ID" << setw(20) << "Name" << "Marks\n";
    cout << "------------------------------------------\n";
    for (const auto& s : students) {
        cout << left << setw(5) << s.id << setw(20) << s.name;
        for (int m : s.marks) cout << m << " ";
        cout << endl;
    }
}

int findStudentIdx(const vector<Student>& students, int id) {
    for (size_t i = 0; i < students.size(); ++i)
        if (students[i].id == id) return i;
    return -1;
}

int main() {
    vector<Student> students;
    int choice;
    int subjCount = 3; // Number of marks per student

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Student st;
            cout << "Enter student ID: "; cin >> st.id; cin.ignore();
            cout << "Enter name: "; getline(cin, st.name);
            st.marks.resize(subjCount);
            for (int i = 0; i < subjCount; ++i) {
                cout << "Enter mark for subject " << (i+1) << ": ";
                cin >> st.marks[i];
            }
            cin.ignore();
            if (findStudentIdx(students, st.id) == -1) {
                students.push_back(st);
                cout << "Student added!\n";
            } else {
                cout << "ID already exists.\n";
            }
        }
        else if (choice == 2) {
            showStudents(students);
        }
        else if (choice == 3) {
            int id, idx;
            cout << "Enter student ID to update: "; cin >> id; cin.ignore();
            idx = findStudentIdx(students, id);
            if (idx == -1) { cout << "Not found.\n"; continue; }
            cout << "Update name (current: " << students[idx].name << "): ";
            getline(cin, students[idx].name);
            for (int i = 0; i < subjCount; ++i) {
                cout << "Update mark for subject " << (i+1)
                     << " (current: " << students[idx].marks[i] << "): ";
                cin >> students[idx].marks[i];
            }
            cin.ignore();
            cout << "Updated!\n";
        }
        else if (choice == 4) {
            int id;
            cout << "Enter student ID to delete: "; cin >> id; cin.ignore();
            int idx = findStudentIdx(students, id);
            if (idx != -1) {
                students.erase(students.begin() + idx);
                cout << "Deleted.\n";
            } else {
                cout << "Not found.\n";
            }
        }
        else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
