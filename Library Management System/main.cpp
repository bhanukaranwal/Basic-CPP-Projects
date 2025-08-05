#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool issued = false;
};

void printMenu() {
    cout << "\n--- Library Management System ---\n";
    cout << "1. Add Book\n";
    cout << "2. View All Books\n";
    cout << "3. Search Books\n";
    cout << "4. Issue Book\n";
    cout << "5. Return Book\n";
    cout << "6. Remove Book\n";
    cout << "7. Exit\n";
    cout << "Choose: ";
}

void showBooks(const vector<Book>& books) {
    if (books.empty()) {
        cout << "No books in the catalog.\n";
        return;
    }
    cout << left << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << "Status\n";
    cout << string(65, '-') << endl;
    for (const auto& b : books) {
        cout << setw(5) << b.id
             << setw(30) << b.title
             << setw(20) << b.author
             << (b.issued ? "Issued" : "Available") << endl;
    }
}

int findBookIdx(const vector<Book>& books, int bookid) {
    for (size_t i = 0; i < books.size(); ++i)
        if (books[i].id == bookid) return i;
    return -1;
}

int main() {
    vector<Book> books;
    int choice;
    int idCounter = 1;

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) { // Add Book
            Book book;
            book.id = idCounter++;
            cout << "Enter book title: "; getline(cin, book.title);
            cout << "Enter author: "; getline(cin, book.author);
            books.push_back(book);
            cout << "Book added!\n";
        }
        else if (choice == 2) { // View All
            showBooks(books);
        }
        else if (choice == 3) { // Search
            cout << "Search by (1) Title or (2) Author? ";
            int s; cin >> s; cin.ignore();
            string key;
            if (s == 1) {
                cout << "Enter title keyword: "; getline(cin, key);
                for (const auto& b : books)
                    if (b.title.find(key) != string::npos)
                        cout << "[" << b.id << "] " << b.title << " by " << b.author << (b.issued ? " (Issued)" : "") << endl;
            } else if (s == 2) {
                cout << "Enter author keyword: "; getline(cin, key);
                for (const auto& b : books)
                    if (b.author.find(key) != string::npos)
                        cout << "[" << b.id << "] " << b.title << " by " << b.author << (b.issued ? " (Issued)" : "") << endl;
            } else {
                cout << "Invalid option.\n";
            }
        }
        else if (choice == 4) { // Issue
            showBooks(books);
            cout << "Enter book ID to issue: ";
            int bid; cin >> bid; cin.ignore();
            int idx = findBookIdx(books, bid);
            if (idx == -1) { cout << "Book not found.\n"; continue; }
            if (books[idx].issued) {
                cout << "Book is already issued.\n";
            } else {
                books[idx].issued = true;
                cout << "Book issued.\n";
            }
        }
        else if (choice == 5) { // Return
            showBooks(books);
            cout << "Enter book ID to return: ";
            int bid; cin >> bid; cin.ignore();
            int idx = findBookIdx(books, bid);
            if (idx == -1) { cout << "Book not found.\n"; continue; }
            if (!books[idx].issued) {
                cout << "Book is not issued.\n";
            } else {
                books[idx].issued = false;
                cout << "Book returned.\n";
            }
        }
        else if (choice == 6) { // Remove
            showBooks(books);
            cout << "Enter book ID to remove: ";
            int bid; cin >> bid; cin.ignore();
            int idx = findBookIdx(books, bid);
            if (idx != -1) {
                books.erase(books.begin() + idx);
                cout << "Book removed.\n";
            } else {
                cout << "Book not found.\n";
            }
        }
        else if (choice == 7) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-7.\n";
        }
    }
    return 0;
}
