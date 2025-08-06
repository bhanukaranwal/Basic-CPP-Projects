/*
  Recipe Calorie Calculator (main.cpp)
  ----------------------------------------
  - Lets user input a recipe: name, servings, and list of ingredients with gram/portion amounts
  - Calculates total calories and per-serving calories using an internal (editable) database
  - Supports saving/loading recipes from file for future lookup/reuse
  - All C++17, single-file, easy to extend with more foods, macros, or CSV input/output!

  Concepts: file I/O, map lookup, calculations, saving state, CLI/UX
*/

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Mini food calorie DB: per 100g unless otherwise noted
map<string, double> calorieDB = {
    {"rice", 130}, {"chicken breast", 165}, {"potato", 77}, {"egg", 155},
    {"bread", 265}, {"cheese", 402}, {"butter", 720}, {"olive oil", 884},
    {"milk", 42}, {"almond", 579}, {"sugar", 387}, {"banana", 89},
    {"apple", 52}, {"carrot", 41}, {"beef", 250}, {"salmon", 208},
    {"oats", 372}, {"yogurt", 60}, {"tofu", 76}, {"avocado", 160}
};

// Ingredient for a recipe
struct Ingredient {
    string name;
    double grams;
    Ingredient(const string& n, double g) : name(n), grams(g) {}
};

// Recipe struct
struct Recipe {
    string name;
    int servings;
    vector<Ingredient> items;
    double totalCal = 0;

    void calculateCalories() {
        totalCal = 0;
        for (const auto& ing : items) {
            auto it = calorieDB.find(ing.name);
            if (it == calorieDB.end()) {
                cout << "No data for '" << ing.name << "'. Skipping.\n";
                continue;
            }
            totalCal += (ing.grams / 100.0) * it->second;
        }
    }

    void print() const {
        cout << "\n--- Recipe: " << name << " ---\n";
        cout << "Servings: " << servings << "\n";
        cout << "Ingredients:\n";
        for (const auto& ing : items)
            cout << "  " << setw(14) << left << ing.name << ": "
                 << fixed << setprecision(1) << ing.grams << "g\n";
        cout << fixed << setprecision(2)
             << "\nTotal Calories: " << totalCal << "\n";
        cout << "Calories per serving: "
             << (servings > 0 ? totalCal / servings : totalCal) << "\n";
    }
};

// --- File I/O for recipes ----
vector<Recipe> loadRecipes(const string& fname) {
    vector<Recipe> db;
    ifstream fin(fname);
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string nm; int serv; ss >> quoted(nm) >> serv;
        if (!serv) continue;
        Recipe r; r.name = nm; r.servings = serv;
        string iname; double grams;
        while (ss >> quoted(iname) >> grams)
            r.items.emplace_back(iname, grams);
        r.calculateCalories();
        db.push_back(r);
    }
    return db;
}

void saveRecipes(const string& fname, const vector<Recipe>& db) {
    ofstream fout(fname, ios::trunc);
    for (const auto& r : db) {
        fout << quoted(r.name) << " " << r.servings;
        for (const auto& i : r.items)
            fout << " " << quoted(i.name) << " " << i.grams;
        fout << "\n";
    }
}

// --- Main UI Logic ----
void addRecipe(vector<Recipe>& db) {
    cout << "Recipe name: "; string name; cin.ignore(); getline(cin, name);
    cout << "Number of servings: "; int s; cin >> s;
    vector<Ingredient> ing;
    cout << "Enter ingredients; one per line as: <name> <grams>\n";
    cout << "Type '.' and ENTER to finish.\n";
    while (true) {
        cout << "> ";
        string n; double g;
        cin >> ws;
        getline(cin, n, ' ');
        if (n == ".") break;
        if (!(cin >> g)) { cin.clear(); string dummy; getline(cin,dummy); cout << "Bad input.\n"; continue; }
        ing.emplace_back(n, g);
        cin.ignore(); // handle remainder of line if any
    }
    Recipe r{ name, s, ing }; r.calculateCalories();
    r.print();
    db.push_back(r);
}

void listCalcDB() {
    cout << "\n--- Food Calorie DB (per 100g) ---\n";
    for (const auto& kv : calorieDB)
        cout << setw(18) << left << kv.first << " : " << kv.second << " kcal\n";
}

int main() {
    cout << "=== Recipe Calorie Calculator ===\n";
    string fname = "recipes.txt";
    vector<Recipe> db = loadRecipes(fname);

    while (true) {
        cout << "\nCommands: add, list, foods, show, save, load, quit\n> ";
        string cmd; cin >> cmd;
        if (cmd == "add") {
            addRecipe(db);
            saveRecipes(fname, db);
        } else if (cmd == "list") {
            cout << "\n--- Saved Recipes ---\n";
            for (size_t i = 0; i < db.size(); ++i)
                cout << setw(2) << i+1 << ". " << db[i].name << "\n";
        } else if (cmd == "show") {
            cout << "Which recipe #? ";
            int idx; cin >> idx;
            if (idx > 0 && idx <= db.size())
                db[idx-1].print();
            else
                cout << "Out of range.\n";
        } else if (cmd == "foods") {
            listCalcDB();
        } else if (cmd == "save") {
            saveRecipes(fname, db);
            cout << "Saved.\n";
        } else if (cmd == "load") {
            db = loadRecipes(fname);
            cout << "Loaded.\n";
        } else if (cmd == "quit") {
            saveRecipes(fname, db);
            cout << "All data saved. Goodbye!\n";
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
