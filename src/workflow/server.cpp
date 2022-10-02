// Code source: https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif

int main(int argc, char *argv[]) {

    if(argc !=3) {
		printf("ERR: Invalid number of arguments!");
		exit(EXIT_FAILURE);
	}

//Initialize Windows Sockets subsystem
#if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2),&wsa_data) != NO_ERROR) {
        printf("\nERR: Windows socket subsystem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#endif

    SOCKET server_soc = INVALID_SOCKET, data_soc = INVALID_SOCKET;
    int valread;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    char sendbuf[] = "WIN: Smartcard communication started";
    char recvbuf[1024] = {0};
    
    //Create a socket descriptor
    if((server_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)    {
        printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);    // first argument is the IP address
    server.sin_port = htons(atoi(argv[2]));         // second argument is port number

    // Bind the socket to the local IP address and port 
    if (bind(server_soc, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(server_soc);
        WSACleanup();
        perror("Socket to IP address and port binding failed!");
        exit(EXIT_FAILURE);
    }
    if (listen(server_soc, 3) < 0) {
        closesocket(server_soc);
        WSACleanup();
        perror("Listening on socket failed!");
        exit(EXIT_FAILURE);
    }
    if ((data_soc = accept(server_soc, (struct sockaddr*) &server, (socklen_t*)&addrlen)) == INVALID_SOCKET) {
        closesocket(server_soc);
        WSACleanup();
        perror("Accepting on socket failed!");
        exit(EXIT_FAILURE);
    }
    
    
    #if defined(_WIN32) || defined(_WIN64)  
        valread = recv(data_soc, recvbuf, 1024, 0);
    #else
        valread = read(data_soc, recvbuf, 1024);
    #endif

    printf("%s\n", recvbuf);
    send(data_soc, sendbuf, strlen(sendbuf), 0);
    printf("INFO: Smartcard communication started..\n");
    printf("GP.EXE: ");
    system("gp --applet 73696D706C656170706C6574 --apdu B0570000023131 -d > GP_out.txt");

    // Close the socket
#if defined(_WIN32) || defined(_WIN64)  
    shutdown(server_soc, SD_BOTH);
    closesocket(server_soc);
    closesocket(data_soc);
    WSACleanup();
#else
    shutdown(server_soc, SHUT_RDWR);
    close(server_soc)
    close(data_soc);
#endif

    exit(EXIT_SUCCESS);
}