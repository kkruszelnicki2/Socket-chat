// SocketChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(int argc, char* argv[])
{
    // Set up DLL
    SOCKET serverSocket, acceptSocket;
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
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); for UDP
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else cout << "socket() is OK!" << endl;

    //Bind Socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else cout << "Bind() is OK!" << endl;

    //Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) cout << "listen(): Error listening to socket" << WSAGetLastError() << endl;
    else cout << "listen() is OK, I'm waiting for connections..." << endl;

    //Chat
    char buffer[200];
    char confirmation[200] = "Message Received";

    //Single client

    //Accept connection

    /*acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    cout << "Accepted connection" << endl;

    while (true) {
        int byteCount = recv(acceptSocket, buffer, 200, 0);

        if (byteCount > 0) cout << "Message received: " << buffer << endl;
        else if (byteCount == 0) {
            cout << "Disconnecting";
            break;
        }
        else cout<<"No message received";

        byteCount = send(acceptSocket, confirmation, 200, 0);

        if (byteCount > 0) cout << "Confirmation send!" << endl;
    }*/

    //Multiple clients

    fd_set server;
    FD_ZERO(&server);

    FD_SET(serverSocket, &server);
    bool isRunning = true;

    while (isRunning) {
        fd_set copy = server;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i = 0; i < socketCount; i++) {
            SOCKET clientSocket = copy.fd_array[i];

            if (clientSocket == serverSocket) { //New connection
                SOCKET client = accept(serverSocket, nullptr, nullptr); //accept connection

                FD_SET(client, &server); //add client to set

                char welcomeMsg[200] = "Welcome to the Chat! \n";
                send(client, welcomeMsg, 200, 0);
            }
            else { //New message
                char buffer[200];

                int byteCount = recv(clientSocket, buffer, 200, 0);

                if (byteCount <= 0) { //if client disconnects
                    closesocket(clientSocket); //close client socket
                    FD_CLR(clientSocket, &server); //remove clientSocket from set
                }
                else {
                    if (buffer[0] == '\\') {
                        if (string(buffer, byteCount) == "\\exit") {
                            isRunning = false;
                            break;
                        }
                        else continue;
                    }

                    for (int i = 0; i < server.fd_count; i++) {
                        SOCKET outSock = server.fd_array[i];
                        if (outSock != serverSocket && outSock != clientSocket) {
                            send(outSock, buffer, byteCount, 0);
                        }
                    }
                }
            }
        }
    }

    closesocket(serverSocket);

    char exitMsg[200] = "Server shuting down";

    while (server.fd_count > 0) {
        SOCKET socket = server.fd_array[0];
        send(socket, exitMsg, 200, 0);

        FD_CLR(socket, &server);
        closesocket(socket);
    }

    //Close Socket
    WSACleanup();
    system("pause");
}


