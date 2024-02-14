// SocketChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(int argc, char* argv[])
{
    // Set up DLL
    SOCKET clientSocket;
    WSADATA wsaData;
    int port = 55555;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Winsock dll not found!" << endl;
        return 0;
    }
    else {
        cout << "Winsock dll found!" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    //Set up server Socket
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else cout << "socket() is OK!" << endl;

    //Connect Client Socket
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);

    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect."<< endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client: connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }
    system("pause");
    WSACleanup();
    return 0;
}


