#include <iostream>
#include <string>
#include <winsock.h>
#include <vector>
#pragma commnet*lib, "wsock32.lib")
#define SERVER_PORT 5000
#define BUF_SIZE 1024

const char* serverAddr = "10.66.132.207";
const int serverPort = SERVER_PORT;

void ExitWithError(const std::string& message) {
    std::cout << message << "Error code: " << WSAGetLastError() << "\n";
    WSACleanup();
    exit(1);
}

void printMessage(const std::string& message, int len) {
    std::cout << "Primljna poruka: ";
    for (size_t i = 0; i < len; std::cout << message[i++]);
    std::cout << '\n';
}

int main() {
    WSAData wsa;
    SOCKET clientSocket;
    if (WSAStartup(0x02020, &wsa) != 0) {
        ExitWithError("Startup failed.");
    }
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        ExitWithError("Listening socket not created");
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverAddr);
    server.sin_port = htons(serverPort);
    
    if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        ExitWithError("connect() failed!");
    }

    std::string echoBuf;
    while (true) {
        std::getline(std::cin, echoBuf);
        if (send(clientSocket, echoBuf.c_str(), echoBuf.length(), 0) != echoBuf.length()) {
            ExitWithError("send() failed");
        }
        int recvMsgSize = 0;
        std::vector<char> buff(BUF_SIZE);
        do {
            if ((recvMsgSize = recv(clientSocket, buff.data(), buff.size(), 0)) < 0) {
                ExitWithError("recv() failed!");
            }
            printMessage(buff.data(), recvMsgSize);
        } while (recvMsgSize == BUF_SIZE);
    }
    std::cout << "IZasao sam";
    closesocket(clientSocket);
    WSACleanup();

    return 0;

}