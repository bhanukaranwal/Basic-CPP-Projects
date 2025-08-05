// main.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET int
#endif

using namespace std;

// Initialize sockets (for Windows)
void initSockets() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
}

// Cleanup sockets (for Windows)
void cleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void handle_client(SOCKET client) {
    char buffer[1024] = {0};
    int recvlen = recv(client, buffer, sizeof(buffer)-1, 0);
    if (recvlen <= 0) {
#ifdef _WIN32
        closesocket(client);
#else
        close(client);
#endif
        return;
    }
    buffer[recvlen] = 0;
    string req(buffer);
    istringstream iss(req);
    string method, path, proto;
    iss >> method >> path >> proto;
    if (method != "GET") {
        string resp = "HTTP/1.0 501 Not Implemented\r\n\r\n";
        send(client, resp.c_str(), resp.size(), 0);
#ifdef _WIN32
        closesocket(client);
#else
        close(client);
#endif
        return;
    }
    // Remove leading slash
    string filename = path == "/" ? "index.html" : path.substr(1);
    ifstream file(filename);
    if (!file) {
        string resp = "HTTP/1.0 404 Not Found\r\n\r\n<h1>404 Not Found</h1>";
        send(client, resp.c_str(), resp.size(), 0);
#ifdef _WIN32
        closesocket(client);
#else
        close(client);
#endif
        return;
    }
    ostringstream body;
    body << file.rdbuf();
    string header = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
    string resp = header + body.str();
    send(client, resp.c_str(), resp.size(), 0);
#ifdef _WIN32
    closesocket(client);
#else
    close(client);
#endif
}

int main() {
    initSockets();
    int port = 8080;
    cout << "--- Simple HTTP Web Server ---\n";
    cout << "Serving files from the current directory on http://localhost:" << port << "/\n";
#ifdef _WIN32
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
#else
    int server = socket(AF_INET, SOCK_STREAM, 0);
#endif
    if (server == INVALID_SOCKET) {
        cerr << "Could not create socket\n";
        cleanupSockets();
        return 1;
    }
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(server, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Bind failed\n";
        cleanupSockets();
        return 1;
    }
    listen(server, 5);
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        SOCKET client = accept(server, (sockaddr*)&client_addr, &client_len);
        if (client == INVALID_SOCKET) continue;
        thread(handle_client, client).detach();
    }
    cleanupSockets();
    return 0;
}
