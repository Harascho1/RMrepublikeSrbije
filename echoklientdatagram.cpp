#include <iostream>
#include <string>
#include <winsock.h>
#include <vector>
#pragma commnet(lib, "wsock32.lib")
#define SERVER_PORT 5000 
#define BUF_SIZE 1024

const char serverAddr[] = "10.66.132.207";

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
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        ExitWithError("Listening socket not created");
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverAddr);
    server.sin_port = htons(SERVER_PORT);

    std::string echoBuf;
    int sz = sizeof(sockaddr);

    
    while (true) {
        std::getline(std::cin, echoBuf);
        if (sendto(clientSocket, echoBuf.c_str(), echoBuf.length(), 0, (sockaddr *)&server, sz) != echoBuf.length()) {
            ExitWithError("send() failed");
        }
        int recvMsgSize = 0;
        char buff[BUF_SIZE];
        if ((recvMsgSize = recvfrom(clientSocket, buff, BUF_SIZE, 0, (sockaddr *)&server, &sz)) < 0) {
            ExitWithError("recv() failed!");
        }
        printMessage(buff, recvMsgSize);
    }
    std::cout << "IZasao sam";
    closesocket(clientSocket);
    WSACleanup();

    return 0;

}