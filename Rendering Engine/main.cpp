#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <chrono>
#include <thread>
using namespace std;

// Simple 3D vector
struct Vec3 {
    double x, y, z;
};

// Projection parameters
const int WIDTH = 40;
const int HEIGHT = 20;
const double FOV = 90.0 * M_PI / 180.0; // 90 degrees field of view
const double VIEW_DIST = 3.0;

// Cube vertices
vector<Vec3> vertices = {
    {-1, -1, -1}, {1, -1, -1}, {1,  1, -1}, {-1,  1, -1},
    {-1, -1,  1}, {1, -1,  1}, {1,  1,  1}, {-1,  1,  1}
};
// Cube edges (pairs of indices into the 'vertices' vector)
vector<pair<int,int>> edges = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

void clearScreen() {
    cout << "\033[2J\033[H";
}

// Rotate point (x, y, z) about the X, Y, and Z axes
Vec3 rotate(Vec3 v, double ax, double ay, double az) {
    // Rotate around x
    double cosy = cos(ay), siny = sin(ay);
    double cosx = cos(ax), sinx = sin(ax);
    double cosz = cos(az), sinz = sin(az);
    // X axis
    double y1 = v.y * cosx - v.z * sinx;
    double z1 = v.y * sinx + v.z * cosx;
    // Y axis
    double x2 = v.x * cosy + z1 * siny;
    double z2 = -v.x * siny + z1 * cosy;
    // Z axis
    double x3 = x2 * cosz - y1 * sinz;
    double y3 = x2 * sinz + y1 * cosz;
    return {x3, y3, z2};
}

// Project 3D to 2D
void project(Vec3 v, int& sx, int& sy) {
    double factor = FOV / (VIEW_DIST + v.z);
    sx = int(WIDTH/2 + v.x * factor * WIDTH/2);
    sy = int(HEIGHT/2 - v.y * factor * HEIGHT/2);
}

void drawFrame(const vector<Vec3>& verts, const vector<pair<int,int>>& edges) {
    char screen[HEIGHT][WIDTH+1];
    for (int i=0; i<HEIGHT; ++i) {
        for (int j=0; j<WIDTH; ++j) screen[i][j] = ' ';
        screen[i][WIDTH] = 0;
    }
    // Draw edges
    for (auto& e : edges) {
        int x0, y0, x1, y1;
        project(verts[e.first], x0, y0);
        project(verts[e.second], x1, y1);
        int dx = abs(x1-x0), dy = abs(y1-y0);
        int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
        int err = dx-dy, e2;
        while (true) {
            if (x0>=0&&x0<WIDTH&&y0>=0&&y0<HEIGHT)
                screen[y0][x0] = '#';
            if (x0==x1&&y0==y1) break;
            e2 = 2*err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    }
    // Output frame
    for (int i=0; i<HEIGHT; ++i)
        cout << screen[i] << endl;
}

int main() {
    double ax=0, ay=0, az=0;
    cout << "--- Simple 3D Rendering Engine (Cube Wireframe) ---\n";
    cout << "Press Ctrl+C to stop.\n";
    while (true) {
        vector<Vec3> verts;
        for (const auto& v : vertices)
            verts.push_back(rotate(v, ax, ay, az));
        clearScreen();
        drawFrame(verts, edges);
        this_thread::sleep_for(std::chrono::milliseconds(80));
        ax += 0.04; ay += 0.025; az += 0.02;
    }
    return 0;
}
