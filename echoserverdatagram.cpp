#include <iostream>
#include <string>
#include <winsock.h>
#include <vector>
#pragma commnet*lib, "wsock32.lib")
#define SERVER_PORT 8888
#define BUF_SIZE 1024

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
    SOCKET serverSock;
    if (WSAStartup(0x02020, &wsa) != 0) {
        ExitWithError("Startup failed.");
    }
    if ((serverSock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        ExitWithError("Listening socket not created");
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    if (bind(serverSock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        ExitWithError("bind() Failed!");
    }

    sockaddr_in client;
    int addrlen = sizeof(sockaddr);

    while (true) {
        char echoBuf[BUF_SIZE];
        int recvMsgSize = recvfrom(serverSock, echoBuf, BUF_SIZE, 0, (sockaddr*)&client, &addrlen);
        if (recvMsgSize < 0) {
            ExitWithError("recv() failed!");
        }
        printMessage(echoBuf, recvMsgSize);
        if (sendto(serverSock, echoBuf, recvMsgSize,0, (sockaddr*)&client, sizeof(sockaddr)) != recvMsgSize) {
            ExitWithError("send() failed!");
        }
    }
    closesocket(serverSock);
    WSACleanup();
}