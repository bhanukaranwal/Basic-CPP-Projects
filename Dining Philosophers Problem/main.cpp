#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
using namespace std;

const int N = 5; // Number of philosophers

mutex forks[N];

void philosopher(int id) {
    int left = id;                  // Left fork
    int right = (id + 1) % N;       // Right fork
    for (int times = 0; times < 3; ++times) {
        // Thinking
        cout << "Philosopher " << id + 1 << " is thinking..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500 + rand()%500));
        
        // Deadlock avoidance: pick up lower-numbered fork first
        int first = min(left, right);
        int second = max(left, right);
        {
            lock_guard<mutex> lock1(forks[first]);
            lock_guard<mutex> lock2(forks[second]);
            // Eating
            cout << "Philosopher " << id + 1 << " is eating (round " << times + 1 << ")..." << endl;
            this_thread::sleep_for(chrono::milliseconds(400 + rand()%400));
        }
        // Automatic release via lock_guard
        cout << "Philosopher " << id + 1 << " finished eating (round " << times + 1 << ")." << endl;
    }
    cout << "Philosopher " << id + 1 << " left the table." << endl;
}

int main() {
    srand(time(0));
    cout << "--- Dining Philosophers Problem Simulator ---\n";
    vector<thread> phils;
    for (int i = 0; i < N; ++i)
        phils.emplace_back(philosopher, i);
    for (auto &th : phils)
        th.join();
    cout << "All philosophers have finished.\n";
    return 0;
}
