#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET int
#endif

using namespace std;

void recvThread(SOCKET sock) {
    char buffer[512];
    while (true) {
        int len = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (len <= 0) break;
        buffer[len] = 0;
        cout << buffer << endl;
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
#endif
    string server_ip = "127.0.0.1";
    int port = 9009;
    cout << "Enter server IP (default 127.0.0.1): ";
    getline(cin, server_ip);
    if (server_ip.empty()) server_ip = "127.0.0.1";
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    addr.sin_port = htons(port);
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Connection failed\n";
        return 1;
    }
    cout << "Connected. You can start typing messages.\n";

    thread t(recvThread, sock);
    string msg;
    while (getline(cin, msg)) {
        send(sock, msg.c_str(), msg.size(), 0);
    }
#ifdef _WIN32
    closesocket(sock); WSACleanup();
#else
    close(sock);
#endif
    t.join();
    return 0;
}
