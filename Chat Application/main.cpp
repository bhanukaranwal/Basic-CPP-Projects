#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET int
#endif

using namespace std;

vector<SOCKET> clients;
mutex clients_mutex;

void broadcast(const string& msg, SOCKET sender) {
    lock_guard<mutex> lock(clients_mutex);
    for (SOCKET client : clients) {
        if (client != sender) {
            send(client, msg.c_str(), msg.size(), 0);
        }
    }
}

void handle_client(SOCKET client) {
    char buffer[512];
    while (true) {
        int len = recv(client, buffer, sizeof(buffer)-1, 0);
        if (len <= 0) break;
        buffer[len] = 0;
        string msg(buffer);
        broadcast(msg, client);
    }
    // Remove client
    {
        lock_guard<mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client), clients.end());
    }
#ifdef _WIN32
    closesocket(client);
#else
    close(client);
#endif
}

int main() {
#ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
#endif
    int port = 9009;
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    cout << "--- Chat Server Started on port " << port << " ---" << endl;
    while (true) {
        sockaddr_in caddr;
        socklen_t clen = sizeof(caddr);
        SOCKET client = accept(server, (sockaddr*)&caddr, &clen);
        {
            lock_guard<mutex> lock(clients_mutex);
            clients.push_back(client);
        }
        thread(handle_client, client).detach();
    }
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
