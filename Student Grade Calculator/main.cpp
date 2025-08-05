#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    vector<double> marks;
    double average;
    char grade;
};

char getGrade(double avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

void printMenu() {
    cout << "\n--- Student Grade Calculator ---\n";
    cout << "1. Add Student\n";
    cout << "2. Show All Students with Grades\n";
    cout << "3. Show Class Rank\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

bool compareAverage(const Student &a, const Student &b) {
    return a.average > b.average;
}

int main() {
    vector<Student> students;
    int nSubjects;
    cout << "Enter number of subjects: ";
    cin >> nSubjects;
    if (nSubjects <= 0) nSubjects = 1; // minimum 1 subject

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(); // Clear newline

        if (choice == 1) { // Add student
            Student s;
            cout << "Enter student name: ";
            getline(cin, s.name);
            cout << "Enter marks for " << nSubjects << " subject(s):\n";
            s.marks.resize(nSubjects);
            double sum = 0;
            for (int i = 0; i < nSubjects; ++i) {
                cout << "  Subject " << (i+1) << ": ";
                cin >> s.marks[i];
                if (s.marks[i] < 0) s.marks[i] = 0;
                if (s.marks[i] > 100) s.marks[i] = 100;
                sum += s.marks[i];
            }
            cin.ignore();
            s.average = sum / nSubjects;
            s.grade = getGrade(s.average);
            students.push_back(s);
            cout << "Student added!\n";
        }
        else if (choice == 2) { // Show all
            if (students.empty()) { cout << "No students added yet.\n"; continue; }
            cout << left << setw(15) << "Name"
                 << setw(12) << "Average"
                 << setw(8)  << "Grade" << endl;
            cout << "-------------------------------\n";
            for (const auto &s : students) {
                cout << left << setw(15) << s.name
                     << setw(12) << fixed << setprecision(2) << s.average
                     << setw(8)  << s.grade << endl;
            }
        }
        else if (choice == 3) { // Show rank
            if (students.empty()) { cout << "No students added yet.\n"; continue; }
            // Copy and sort
            vector<Student> ranklist = students;
            sort(ranklist.begin(), ranklist.end(), compareAverage);
            cout << left << setw(7) << "Rank"
                 << setw(15) << "Name"
                 << setw(12) << "Average"
                 << setw(8)  << "Grade" << endl;
            cout << "-------------------------------------\n";
            for (size_t i = 0; i < ranklist.size(); ++i) {
                cout << left << setw(7) << (i+1)
                     << setw(15) << ranklist[i].name
                     << setw(12) << fixed << setprecision(2) << ranklist[i].average
                     << setw(8)  << ranklist[i].grade << endl;
            }
        }
        else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-4.\n";
        }
    }
    return 0;
}
