#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <fstream>
#include<string>
using namespace std;

const int PORT = 8080;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 1);

    cout << "Server is running on port " << PORT << std::endl;

    SOCKET client_socket = accept(server_socket, nullptr, nullptr);

    char buffer[1024] = { 0 };
    recv(client_socket, buffer, 1024, 0);

    ifstream file("index.html");
    string response;

    if (file.is_open()) {
        response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        std::string line;
        while (std::getline(file, line)) {
            response += line + "\n";
        }
        file.close();
    }
    else {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n"
            "<html><body><h1>404 Not Found</h1></body></html>";
    }

    send(client_socket, response.c_str(), response.size(), 0);

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
