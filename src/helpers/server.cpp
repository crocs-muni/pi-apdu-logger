/* 
    Code: https://www.geeksforgeeks.org/socket-programming-cc/ adapted for the purpose of Windows-Raspberry Pi testing
    TCP server code for Windows
    Usage: .\build\server.exe <SERVER IP ADDRESS> <SERVER PORT>
    g++ -o .\build\server.exe .\src\helpers\server.cpp -lws2_32
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdint.h>

#define BUFFER_SIZE 5

int main(int argc, char *argv[]) {

    if(argc !=3) {
		printf("ERR: Invalid number of arguments!");
		exit(EXIT_FAILURE);
	}

    // Initialize Windows Sockets subsystem  
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2),&wsa_data) < 0) {
        printf("\nERR: Windows socket subsystem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    SOCKET server_soc = -1, data_soc = -1; //socket descriptors
    struct sockaddr_in server;
    int addrlen_s = sizeof(server);
    struct sockaddr_in client;
    int addrlen_c = sizeof(client);
    char syncbuf[] = "####"; //this buffer is used to sync with RaspberryPi 
    char stopbuf[] = "STOP";
    char recvbuf[BUFFER_SIZE] = {0}; //this buffer us used to receive msgs from RaspberryPi
    
    //Create a socket descriptor
    server_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_soc < 0)    {
        printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);    // first input argument is the IP address
    server.sin_port = htons(atoi(argv[2]));         // second input argument is the port number

    // Bind the socket to the local IP address and port 
    int bind_status = bind(server_soc, (struct sockaddr*) &server, sizeof(server));
    if (bind_status < 0) {
        closesocket(server_soc);
        WSACleanup();
        perror("Socket to IP address and port binding failed!");
        exit(EXIT_FAILURE);
    }

    char command[1200];
    int repetitions = 100;
    
    printf("Enter the command to execute with gp.exe: ");
    scanf("%[^\n]s", &command);
    printf("Command to execute : %s", command);
    
    printf("\nWaiting for the client connection.. \n");
    if (listen(server_soc, 1) < 0) {
        closesocket(server_soc);
        WSACleanup();
        perror("Listening for requests on socket failed!");
        exit(EXIT_FAILURE);
    }
    //Accept a request from the client and create a new socket descriptor
    data_soc = accept(server_soc, (struct sockaddr*) &client, (socklen_t*)&addrlen_c);
    if (data_soc < 0) {
        closesocket(server_soc);
        WSACleanup();
        perror("Accepting requests on socket failed!");
        exit(EXIT_FAILURE);
    }
 
    for(int i = 0; i < repetitions; i++) { 
        printf("INFO: Awaiting client signal..\n");
        memset(recvbuf, sizeof(recvbuf), 0); //clear the syncbuf
        recv(data_soc, recvbuf, sizeof(recvbuf), 0);
        printf(">> %s\n", recvbuf);

        printf("INFO: Smartcard communication started..\n");
        printf("INFO: Executing %s\n", command);
        system(command);
        
        if(i == repetitions-1) {
            printf("INFO: Sending stop signal to the client.\n");
            send(data_soc, stopbuf, strlen(stopbuf), 0); //send stop message
        }
        else {
            printf("INFO: Sending sync signal to the client.\n");
            send(data_soc, syncbuf, strlen(syncbuf), 0); //send the sync command
        }
    } 

    // Close the socket
    printf("INFO: No further data, exiting...\n");
    shutdown(server_soc, SD_BOTH);
    closesocket(server_soc);
    closesocket(data_soc);
    WSACleanup();
    
    exit(EXIT_SUCCESS);
}