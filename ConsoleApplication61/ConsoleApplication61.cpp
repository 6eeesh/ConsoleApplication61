#include <iostream>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h> 

using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1) {
        cerr << "Invalid address\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    serverAddr.sin_port = htons(54000);

    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Failed to connect to server: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    string user_input;
    while (true) {
        cout << "> ";
        getline(cin, user_input);

        send(clientSocket, user_input.c_str(), user_input.size() + 1, 0);

        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "Сервер: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}