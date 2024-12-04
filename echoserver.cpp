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
    SOCKET listensock;
    if (WSAStartup(0x02020, &wsa) != 0) {
        ExitWithError("Startup failed.");
    }
    if ((listensock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        ExitWithError("Listening socket not created");
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    if (bind(listensock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        ExitWithError("bind() Failed!");
    }
    
    if ((listen(listensock, 3)) < 0) {
        ExitWithError("listen() Failed!");
    }

    while (true) {
        sockaddr_in client;
        int cLen = sizeof(struct sockaddr_in);
        SOCKET clientSock;
        if ((clientSock = accept(listensock, (struct sockaddr *)&client, &cLen)) < 0) {
            ExitWithError("accept() Failed!");
        }

        std::vector<char> echoBuf(BUF_SIZE);
        int recvMsgSize = recv(clientSock, echoBuf.data(), echoBuf.size(), 0);

        if (recvMsgSize < 0) {
            ExitWithError("recv() failed");
        }

        printMessage(echoBuf.data(), recvMsgSize);

        printf("receive messgage %d", recvMsgSize);

        while (recvMsgSize > 0) {
            if (send(clientSock, echoBuf.data(), recvMsgSize, 0) != recvMsgSize) {
                ExitWithError("send() failed");
            }
            if ((recvMsgSize = recv(clientSock, echoBuf.data(), echoBuf.size(), 0)) < 0) {
                ExitWithError("recv() failed");
            }
            printf("receive messgage %d", recvMsgSize);
            printMessage(echoBuf.data(), recvMsgSize);
        }
    }

    std::cout << "Izasao sam\n";

    closesocket(listensock);
    WSACleanup();
    return 0;
}