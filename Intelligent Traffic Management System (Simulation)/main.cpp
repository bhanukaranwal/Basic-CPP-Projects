/*
  Intelligent Traffic Management System Simulation
  -------------------------------------------------
  Simulates an intersection with 4 directions (N/E/S/W).
  - Uses random traffic pattern generation per direction.
  - Continuously "optimizes" green light timing based on real-time queue lengths.
  - Visualizes the intersection state, queue sizes, and signal state.
  - Logs summary stats at simulation end.
  (All C++17 standard library, fully portable)
*/
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

// GLOBALS
constexpr int NUM_DIRECTIONS = 4;
constexpr int SIM_STEPS = 30;                // Number of light cycles
constexpr int BASE_GREEN_TIME = 5;           // Minimum green light seconds
constexpr int MAX_GREEN_TIME = 15;           // Maximum green light seconds
constexpr int ARRIVAL_CHANCE = 70;           // Out of 100 (per direction, per sec)
const char* const DIR_NAMES[4] = {"North", "East", "South", "West"};
const char SIGNAL_COLOR[][6] = {"RED  ", "GREEN"};

struct Direction {
    int queueSize = 0;             // Current queue
    int totalArrived = 0;          // Stats
    int totalPassed = 0;
    int peakQueue = 0;
};

// Helper: random number generator
int randomInt(int min, int max) {
    static std::mt19937 rng(static_cast<unsigned int>(
        chrono::system_clock::now().time_since_epoch().count()));
    return uniform_int_distribution<int>(min, max)(rng);
}

// Simulates car arrivals for all directions each second
void addArrivals(vector<Direction>& dirs) {
    for (int d=0; d<NUM_DIRECTIONS; ++d) {
        if (randomInt(1, 100) <= ARRIVAL_CHANCE) {
            int cars = randomInt(1, 3); // 1 to 3 cars may arrive
            dirs[d].queueSize += cars;
            dirs[d].totalArrived += cars;
            dirs[d].peakQueue = max(dirs[d].peakQueue, dirs[d].queueSize);
        }
    }
}

// Decides next green light direction and how long,
// based on who has the largest queue currently.
pair<int, int> decideNextGreen(const vector<Direction>& dirs) {
    int maxQ = -1, dir = 0;
    for (int d=0; d<NUM_DIRECTIONS; ++d) {
        if (dirs[d].queueSize > maxQ) {
            maxQ = dirs[d].queueSize;
            dir = d;
        }
    }
    // Green time scales with queue, but always in limits
    int greenTime = BASE_GREEN_TIME + dirs[dir].queueSize;
    greenTime = min(MAX_GREEN_TIME, max(BASE_GREEN_TIME, greenTime));
    return make_pair(dir, greenTime);
}

// Processes the green phase for the selected direction
void processGreen(vector<Direction>& dirs, int dir, int greenSecs) {
    // Assume up to 2 cars can leave every second
    for (int s=0; s<greenSecs; ++s) {
        addArrivals(dirs);
        int out = min({dirs[dir].queueSize, randomInt(1,2)});
        dirs[dir].queueSize -= out;
        dirs[dir].totalPassed += out;
        for (int d=0; d<NUM_DIRECTIONS; ++d)
            dirs[d].peakQueue = max(dirs[d].peakQueue, dirs[d].queueSize);
        // Show intersection state
        cout << "\nTime+s " << setw(2) << s+1 << "/" << greenSecs
             << " | GREEN: " << DIR_NAMES[dir] << "\n";
        for (int d=0; d<NUM_DIRECTIONS; ++d) {
            cout << "  [" << DIR_NAMES[d][0] << "] "
                 << "Signal: " << (d==dir ? SIGNAL_COLOR[1] : SIGNAL_COLOR[0])
                 << " | Q: " << setw(2) << dirs[d].queueSize << "\n";
        }
        std::this_thread::sleep_for(chrono::milliseconds(400));
    }
}

// Displays a summary of the simulation
void printSummary(const vector<Direction>& dirs, int totalTime) {
    cout << "\n\n===== Simulation Summary =====\n";
    cout << setw(12) << "Direction"
         << setw(10) << "Arrived"
         << setw(10) << "Passed"
         << setw(10) << "PeakQ"
         << setw(10) << "EndQ" << "\n";
    for (int d=0; d<NUM_DIRECTIONS; ++d) {
        cout << setw(12) << DIR_NAMES[d]
             << setw(10) << dirs[d].totalArrived
             << setw(10) << dirs[d].totalPassed
             << setw(10) << dirs[d].peakQueue
             << setw(10) << dirs[d].queueSize << "\n";
    }
    cout << "\nTotal simulation time: " << totalTime << "s\n";
    cout << "Avg throughput: "
         << fixed << setprecision(2)
         << (float) accumulate(dirs.begin(), dirs.end(), 0,
                               [](int sum, const Direction& d){return sum + d.totalPassed;}) / totalTime
         << " veh/sec\n";
}

int main() {
    cout << "===== Intelligent Traffic Signal System Simulation =====\n\n";
    vector<Direction> dirs(NUM_DIRECTIONS);
    int simTime = 0;

    for (int t=1; t<=SIM_STEPS; ++t) {
        addArrivals(dirs);
        auto [greenDir, greenSecs] = decideNextGreen(dirs);

        // Print intersection state before green
        cout << "\nCycle #" << t << " | ";
        cout << "Next GREEN: " << DIR_NAMES[greenDir]
             << " for " << greenSecs << "s (Queue=" << dirs[greenDir].queueSize << ")\n";
        for (int d=0; d<NUM_DIRECTIONS; ++d) {
            cout << "  [" << DIR_NAMES[d][0] << "] Q: " << setw(2) << dirs[d].queueSize;
            if (d == greenDir) cout << " <--";
            cout << "\n";
        }

        processGreen(dirs, greenDir, greenSecs);
        simTime += greenSecs;
        std::this_thread::sleep_for(chrono::milliseconds(350));
    }

    printSummary(dirs, simTime);
    cout << "\nSimulation complete. Thank you for using the Traffic Mgmt Simulator!\n";
    return 0;
}
