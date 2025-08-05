#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
using namespace std;

// Product representation
struct Product {
    int id;
    string name;
    double price;
};

// User representation
struct User {
    string username;
    string password;
    vector<int> cart; // stores product IDs
};

// Product seed data
vector<Product> products = {
    {1, "Laptop", 699.99},
    {2, "Headphones", 49.99},
    {3, "Smartphone", 499.99},
    {4, "Book", 14.99},
    {5, "Backpack", 29.99}
};
// In-memory user store
map<string, User> users;
User* currentUser = nullptr;

void showProducts() {
    cout << "\n--- Product Catalog ---\n";
    cout << left << setw(5) << "ID" << setw(20) << "Name" << "Price\n";
    cout << string(35, '-') << endl;
    for (const auto& p : products)
        cout << setw(5) << p.id << setw(20) << p.name << "$" << fixed << setprecision(2) << p.price << endl;
}

void registerUser() {
    string uname, pwd;
    cout << "Choose a username: "; cin >> uname;
    if (users.count(uname)) { cout << "Username already taken.\n"; return; }
    cout << "Choose a password: "; cin >> pwd;
    users[uname] = {uname, pwd, {}};
    cout << "Registration successful!\n";
}

bool loginUser() {
    string uname, pwd;
    cout << "Username: "; cin >> uname;
    cout << "Password: "; cin >> pwd;
    if (users.count(uname) && users[uname].password == pwd) {
        currentUser = &users[uname];
        cout << "Login successful. Welcome, " << uname << "!\n";
        return true;
    }
    cout << "Login failed.\n";
    return false;
}

void addToCart() {
    showProducts();
    cout << "Enter product ID to add to your cart: ";
    int id; cin >> id;
    bool found = false;
    for (const auto& p : products)
        if (p.id == id) found = true;
    if (!found) { cout << "Invalid product ID.\n"; return; }
    currentUser->cart.push_back(id);
    cout << "Added to cart!\n";
}

void viewCart() {
    if (currentUser->cart.empty()) { cout << "Your cart is empty.\n"; return; }
    cout << "\n--- Your Cart ---\n";
    double total = 0;
    for (int pid : currentUser->cart)
        for (const auto& p : products)
            if (p.id == pid) {
                cout << p.name << " - $" << fixed << setprecision(2) << p.price << '\n';
                total += p.price;
            }
    cout << "Total: $" << fixed << setprecision(2) << total << '\n';
}

void checkout() {
    if (currentUser->cart.empty()) { cout << "Nothing to checkout.\n"; return; }
    viewCart();
    cout << "Checkout complete! Thank you for your purchase.\n";
    currentUser->cart.clear();
}

int main() {
    cout << "--- Simple E-commerce Platform (CLI) ---\n";
    int choice;
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nChoose: ";
        cin >> choice;
        if (choice == 1) registerUser();
        else if (choice == 2 && loginUser()) break;
        else if (choice == 3) { cout << "Bye!\n"; return 0; }
        else cout << "Invalid option.\n";
    }
    // Main shopping loop
    while (true) {
        cout << "\n1. Show Products\n2. Add to Cart\n3. View Cart\n4. Checkout\n5. Logout\nChoose: ";
        cin >> choice;
        if (choice == 1) showProducts();
        else if (choice == 2) addToCart();
        else if (choice == 3) viewCart();
        else if (choice == 4) checkout();
        else if (choice == 5) { cout << "Logged out.\n"; currentUser = nullptr; break; }
        else cout << "Invalid.\n";
    }
    return 0;
}
