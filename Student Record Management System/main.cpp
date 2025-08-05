#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;

struct Student {
    int roll;
    string name;
    double grade;
};

// File to save/load students
const string STUDENT_FILE = "students.txt";

// Save all students to file, one per line
void saveStudents(const vector<Student>& students) {
    ofstream fout(STUDENT_FILE);
    for (const auto& s : students)
        fout << s.roll << '\t' << s.name << '\t' << s.grade << '\n';
    fout.close();
}

// Load all students from file at start
void loadStudents(vector<Student>& students) {
    students.clear();
    ifstream fin(STUDENT_FILE);
    if (!fin) return; // No file, fine on first run
    int roll; string name; double grade;
    while (fin >> roll) {
        fin.ignore(); // skip tab
        getline(fin, name, '\t');
        fin >> grade;
        fin.ignore(); // skip newline
        students.push_back({roll, name, grade});
    }
    fin.close();
}

void printMenu() {
    cout << "\n--- Student Record Management ---\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

void showStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }
    cout << left << setw(6) << "Roll" << setw(20) << "Name" << setw(8) << "Grade\n";
    cout << string(34, '-') << '\n';
    for (const auto& s : students)
        cout << setw(6) << s.roll << setw(20) << s.name << setw(8) << s.grade << '\n';
}

void searchStudent(const vector<Student>& students) {
    int roll;
    cout << "Enter roll number: "; cin >> roll; cin.ignore();
    auto it = find_if(students.begin(), students.end(),
                      [&](const Student& s){return s.roll == roll;});
    if (it != students.end()) {
        cout << "Found: Roll " << it->roll << ", Name: " << it->name << ", Grade: " << it->grade << '\n';
    } else {
        cout << "No student found with that roll number.\n";
    }
}

void updateStudent(vector<Student>& students) {
    int roll;
    cout << "Enter roll number to update: "; cin >> roll; cin.ignore();
    for (auto& s : students) {
        if (s.roll == roll) {
            cout << "Enter new name: "; getline(cin, s.name);
            cout << "Enter new grade: "; cin >> s.grade; cin.ignore();
            saveStudents(students);
            cout << "Record updated.\n";
            return;
        }
    }
    cout << "No student found with that roll number.\n";
}

void deleteStudent(vector<Student>& students) {
    int roll;
    cout << "Enter roll number to delete: "; cin >> roll; cin.ignore();
    auto it = remove_if(students.begin(), students.end(),
                        [&](const Student& s) { return s.roll == roll; });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveStudents(students);
        cout << "Student deleted.\n";
    } else {
        cout << "No student found with that roll number.\n";
    }
}

int main() {
    vector<Student> students;
    loadStudents(students);
    int choice;
    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            Student s;
            cout << "Enter roll number: "; cin >> s.roll; cin.ignore();
            cout << "Enter name: "; getline(cin, s.name);
            cout << "Enter grade: "; cin >> s.grade; cin.ignore();
            students.push_back(s);
            saveStudents(students);
            cout << "Student added!\n";
        }
        else if (choice == 2) {
            showStudents(students);
        }
        else if (choice == 3) {
            searchStudent(students);
        }
        else if (choice == 4) {
            updateStudent(students);
        }
        else if (choice == 5) {
            deleteStudent(students);
        }
        else if (choice == 6) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid option. Enter 1-6.\n";
        }
    }
    return 0;
}
