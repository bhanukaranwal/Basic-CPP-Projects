#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

struct Weather {
    int day;
    float temperature; // Celsius
    float humidity;    // Percentage
    string condition;  // Sunny, Rainy, Cloudy, etc.
};

string getRandomCondition() {
    static const string conditions[] = {"Sunny", "Rainy", "Cloudy", "Windy", "Thunderstorm", "Snowy"};
    return conditions[rand() % 6];
}

void printMenu() {
    cout << "\n--- Weather Data Simulator ---\n";
    cout << "1. Simulate Weather for N Days\n";
    cout << "2. Show All Recorded Weather Data\n";
    cout << "3. Show Summary (Avg Temp & Humidity)\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(time(0));
    vector<Weather> data;
    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        if (choice == 1) {
            int n;
            cout << "Simulate for how many days? ";
            cin >> n;
            if (n <= 0) {
                cout << "Please enter a positive number.\n";
                continue;
            }
            for (int i = 1; i <= n; ++i) {
                Weather w;
                w.day = data.size() + 1;
                w.temperature = (rand() % 3500) / 100.0f - 10.0f; // -10.0 to 25.0 °C
                w.humidity = (rand() % 101); // 0% to 100%
                w.condition = getRandomCondition();
                data.push_back(w);
            }
            cout << n << " days simulated and recorded!\n";
        }
        else if (choice == 2) {
            if (data.empty()) { cout << "No weather data yet.\n"; continue; }
            cout << "\nDay  | Temp (C) | Humidity (%) | Condition\n";
            cout << "---------------------------------------------\n";
            for (const auto& w : data)
                cout << setw(4) << w.day << " | "
                     << setw(8) << fixed << setprecision(2) << w.temperature << " | "
                     << setw(12) << w.humidity << " | "
                     << w.condition << '\n';
        }
        else if (choice == 3) {
            if (data.empty()) { cout << "No weather data to summarize.\n"; continue; }
            float t_sum = 0, h_sum = 0;
            for (const auto& w : data) {
                t_sum += w.temperature;
                h_sum += w.humidity;
            }
            cout << "Average Temperature: " 
                 << fixed << setprecision(2) << t_sum / data.size() << " °C\n";
            cout << "Average Humidity:    " 
                 << fixed << setprecision(2) << h_sum / data.size() << " %\n";
        }
        else if (choice == 4) {
            cout << "Goodbye!\n"; break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
