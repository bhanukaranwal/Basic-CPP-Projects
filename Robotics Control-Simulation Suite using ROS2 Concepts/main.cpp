/*
  Robotics Fleet Simulation Suite (main.cpp)
  -------------------------------------------------
  - Simulates multiple robots moving in a grid world.
  - Each robot plans a path using A* to a random destination.
  - "Sensors" detect obstacles (randomized).
  - Console visualization: world map, robot/goal positions, and step-by-step movement.
  - Demonstrates coordination, path planning, and modular robotics logic in pure C++17.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <utility>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iomanip>
#include <set>
using namespace std;

// --- Grid setup ---
constexpr int GRID_W = 16, GRID_H = 9, NUM_ROBOTS = 3, OBSTACLE_COUNT = 16;

// Directions: N, E, S, W
const int DX[4] = {-1, 0, 1, 0}, DY[4] = {0, 1, 0, -1};

struct Pose { int r, c; };

// For pathfinding
struct Node {
    int r, c;
    int gCost, hCost;
    Node* parent;
    int f() const { return gCost + hCost; }
};
bool operator<(const Node& a, const Node& b) { return a.f() > b.f(); }

// Robot agent
struct Robot {
    int id;
    Pose pos;
    Pose goal;
    vector<Pose> plannedPath;
    char symbol;
    bool planning = false;
    Robot(int id_, Pose pos_, Pose goal_, char sym): id(id_), pos(pos_), goal(goal_), symbol(sym) {}
};

// Random obstacles
vector<Pose> genObstacles(mt19937& rng) {
    set<pair<int,int>> obs;
    uniform_int_distribution<int> wr(0, GRID_H-1), wc(0, GRID_W-1);
    while ((int)obs.size() < OBSTACLE_COUNT)
        obs.emplace(wr(rng), wc(rng));
    vector<Pose> vec;
    for (const auto& p : obs) vec.push_back({p.first, p.second});
    return vec;
}
bool isFree(const Pose& p, const vector<Pose>& obs) {
    for (auto& o : obs) if (o.r == p.r && o.c == p.c) return false;
    return p.r >= 0 && p.r < GRID_H && p.c >= 0 && p.c < GRID_W;
}
bool sameCell(const Pose& a, const Pose& b) { return a.r==b.r && a.c==b.c; }

// --- A* Path Planning ---
vector<Pose> astar(const Pose& src, const Pose& dst, const vector<Pose>& obs) {
    vector<vector<bool>> closed(GRID_H, vector<bool>(GRID_W,false));
    vector<vector<Node*>> nodes(GRID_H, vector<Node*>(GRID_W,nullptr));
    auto h = [&](int r, int c){ return abs(r-dst.r)+abs(c-dst.c); };
    priority_queue<Node> q;

    Node* start = new Node{src.r,src.c,0,h(src.r,src.c),nullptr};
    q.push(*start); nodes[src.r][src.c]=start;
    while (!q.empty()) {
        Node curr = q.top(); q.pop();
        if (closed[curr.r][curr.c]) continue;
        closed[curr.r][curr.c]=true;
        if (curr.r==dst.r && curr.c==dst.c) {
            // Reconstruct path
            vector<Pose> path;
            Node* p = nodes[curr.r][curr.c];
            while (p) { path.push_back({p->r,p->c}); p=p->parent; }
            reverse(path.begin(),path.end());
            // Clean up
            for (auto& v : nodes) for (auto& n : v) delete n;
            return path;
        }
        for (int d=0;d<4;++d) {
            int nr=curr.r+DX[d], nc=curr.c+DY[d];
            if (!isFree({nr,nc},obs)) continue;
            int g=curr.gCost+1;
            if (!nodes[nr][nc] || g < nodes[nr][nc]->gCost) {
                Node* newN = new Node{nr,nc,g,h(nr,nc),nodes[curr.r][curr.c]};
                nodes[nr][nc]=newN;
                q.push(*newN);
            }
        }
    }
    // No path
    for (auto& v : nodes) for (auto& n : v) delete n;
    return {};
}

// --- Visualization ---
void printWorld(const vector<Robot>& robots, const vector<Pose>& obs) {
    vector<vector<char>> grid(GRID_H,vector<char>(GRID_W,'.'));
    for (auto& o : obs) grid[o.r][o.c]='#';
    for (auto& r : robots) grid[r.goal.r][r.goal.c]='G';
    for (auto& r : robots) grid[r.pos.r][r.pos.c]=r.symbol;
    cout << "\n  ";
    for (int c=0;c<GRID_W;++c) cout << (c/10?'0'+c/10:' ') << (c%10);
    cout << "\n";
    for (int r=0;r<GRID_H;++r){
        cout << setw(2) << r << " ";
        for (int c=0;c<GRID_W;++c)
            cout << grid[r][c] << ' ';
        cout << "\n";
    }
    cout << "Key: .=Empty  #=Obstacle  A-C=Robot  G=Goal\n";
}

// --- Main Simulation ---
int main() {
    mt19937 rng(time(nullptr));
    // Place obstacles
    auto obs = genObstacles(rng);

    // Place robots at random starts/goals (avoid collisions)
    vector<Robot> fleet;
    char syms[] = {'A','B','C','D','E'};
    set<pair<int,int>> used;
    for(int i=0;i<NUM_ROBOTS;++i) {
        Pose pos,goal;
        do { pos = {rng()%GRID_H, rng()%GRID_W}; } while (!isFree(pos,obs)||used.count({pos.r,pos.c}));
        used.emplace(pos.r,pos.c);
        do { goal = {rng()%GRID_H, rng()%GRID_W}; } while ((!isFree(goal,obs) || used.count({goal.r,goal.c})) && !(goal.r==pos.r && goal.c==pos.c));
        used.emplace(goal.r,goal.c);
        fleet.emplace_back(i, pos, goal, syms[i]);
    }

    // Path planning: each robot plans to goal
    for (auto& r : fleet) {
        cout << "Robot " << r.symbol << " planning path...\n";
        r.plannedPath = astar(r.pos, r.goal, obs);
        if (r.plannedPath.empty()) cout << "  No path found!\n";
        else cout << "  Path length: " << r.plannedPath.size()-1 << "\n";
        r.planning = !r.plannedPath.empty();
        this_thread::sleep_for(chrono::milliseconds(600));
    }

    // Main loop: robots move 1 step per turn
    int step=1;
    printWorld(fleet, obs);
    while (true) {
        bool allArrived=true;
        for (auto& r : fleet) {
            if (!r.planning || r.plannedPath.empty() || sameCell(r.pos, r.goal)) continue;
            allArrived=false;
            // Move along path (skip if already at goal)
            auto it = find_if(r.plannedPath.begin(),r.plannedPath.end(),
                              [&](const Pose& p){return sameCell(p,r.pos);});
            if (it!=r.plannedPath.end()&&it+1!=r.plannedPath.end()) r.pos=*(it+1);
        }
        cout << "\n--- Step " << step++ << " ---";
        printWorld(fleet, obs);
        if (allArrived) break;
        this_thread::sleep_for(chrono::milliseconds(800));
    }
    cout << "\nSimulation complete! All robots reached their goals.\n";
    return 0;
}
