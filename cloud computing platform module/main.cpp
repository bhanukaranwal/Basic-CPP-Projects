/*
  Cloud Microservice Simulation: High-Concurrency Task Queue (main.cpp)
  -----------------------------------------------------
  - Multi-threaded task queue microservice console demo
  - Clients post tasks (strings) to a shared queue
  - Worker threads process tasks concurrently (simulated "work")
  - Shows basic REST-like interface, status display, and graceful shutdown
  - C++17 standard, fully self-contained (no external dependencies)

  Concepts: thread safety (mutex, lock_guard), producer-consumer queue, atomic ops, condition_variable
*/

#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <string>
using namespace std;

class TaskQueue {
    queue<string> q;
    mutex mtx;
    condition_variable cv;
    bool shutdownFlag = false;
public:
    void push(const string& task) {
        lock_guard<mutex> lock(mtx);
        q.push(task);
        cv.notify_one();
    }
    bool pop(string& task) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]{ return !q.empty() || shutdownFlag; });
        if (!q.empty()) {
            task = q.front(); q.pop();
            return true;
        }
        return false;
    }
    void shutdown() {
        lock_guard<mutex> lock(mtx);
        shutdownFlag = true; // unblock all waits
        cv.notify_all();
    }
    bool empty() {
        lock_guard<mutex> lock(mtx);
        return q.empty();
    }
};

// Simulated "worker" function
void worker(TaskQueue& tq, atomic<int>& processed, int id) {
    string task;
    while (tq.pop(task)) {
        cout << "[Worker " << id << "] Processing task: " << task << "\n";
        this_thread::sleep_for(chrono::milliseconds(500 + 70*(task.length()%3))); // Simulate work
        processed++;
    }
    cout << "[Worker " << id << "] Exiting (no more tasks)\n";
}

void printStatus(const atomic<int>& processed, const TaskQueue& tq, int totalPosted, int numThreads) {
    cout << "\n--- Task Queue Microservice Status ---\n"
         << "  Total tasks posted: " << totalPosted << "\n"
         << "  Total tasks processed: " << processed.load() << "\n"
         << "  Tasks in queue: " << (tq.empty() ? 0 : 1) << " (see note below)\n"
         << "  Worker threads: " << numThreads << "\n"
         << "(*) Queue depth not exact (demo), see code for custom monitoring.\n"
         << endl;
}

int main() {
    cout << "=== Cloud Task-Queue Microservice Simulation (C++) ===\n";
    cout << "Commands: POST <task>, STATUS, EXIT, HELP\n";
    TaskQueue tq;
    atomic<int> processed = 0;
    int totalPosted = 0;
    const int NUM_WORKERS = 4;
    vector<thread> pool;

    // Spin up worker pool
    for (int i=0; i<NUM_WORKERS; ++i)
        pool.emplace_back(worker, ref(tq), ref(processed), i+1);

    string line, cmd;
    cout << ">> ";
    while (getline(cin, line)) {
        if (line.empty()) { cout << ">> "; continue; }
        istringstream iss(line);
        iss >> cmd;
        if (cmd == "POST") {
            string task;
            getline(iss, task);
            if (task.empty()) { cout << "No task specified.\n"; }
            else {
                tq.push(task.substr(1)); // skip initial whitespace
                ++totalPosted;
                cout << "[Client] Posted new task: " << task.substr(1) << endl;
            }
        } else if (cmd == "STATUS") {
            printStatus(processed, tq, totalPosted, NUM_WORKERS);
        } else if (cmd == "HELP") {
            cout << "Commands:\n  POST <task> : add task to queue\n  STATUS : print stats\n  EXIT : shutdown server\n";
        } else if (cmd == "EXIT") {
            cout << "Initiating graceful shutdown...\n";
            break;
        } else {
            cout << "Unknown command. Type HELP for options.\n";
        }
        cout << ">> ";
    }
    // Shut down
    tq.shutdown();
    for (auto& t : pool) t.join();
    printStatus(processed, tq, totalPosted, NUM_WORKERS);
    cout << "Simulation complete. All tasks processed. Goodbye!\n";
    return 0;
}
