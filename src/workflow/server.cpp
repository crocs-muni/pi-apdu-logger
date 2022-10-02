/* 
    Code source: https://www.geeksforgeeks.org/socket-programming-cc/
    TCP server code for Windows
    Usage: server.exe <SERVER IP ADDRESS> <SERVER PORT>
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define BUFF 1024

void write_file(int sockfd){
  int status;
  FILE *fp;
  char filename[] = "recv.txt";
  char buffer[BUFF];
 
  fp = fopen(filename, "w");
  while (1) {
    status = recv(sockfd, buffer, BUFF, 0);
    if (status <= 0) {
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    memset(buffer, 0, BUFF);
  }
  return;
}

int main(int argc, char *argv[]) {

    if(argc !=3) {
		printf("ERR: Invalid number of arguments!");
		exit(EXIT_FAILURE);
	}

//Initialize Windows Sockets subsystem  
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2),&wsa_data) < 0) {
        printf("\nERR: Windows socket subsystem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    SOCKET server_soc = -1, data_soc = -1;
    int valread;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    char sendbuf[] = "WIN: Smartcard communication started";
    char recvbuf[1024] = {0};
    
    //Create a socket descriptor
    if((server_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)    {
        printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);    // first argument is the IP address
    server.sin_port = htons(atoi(argv[2]));         // second argument is port number

    // Bind the socket to the local IP address and port 
    if (bind(server_soc, (struct sockaddr*) &server, sizeof(server)) < 0) {
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
    if ((data_soc = accept(server_soc, (struct sockaddr*) &server, (socklen_t*)&addrlen)) < 0) {
        closesocket(server_soc);
        WSACleanup();
        perror("Accepting on socket failed!");
        exit(EXIT_FAILURE);
    }
    
    valread = recv(data_soc, recvbuf, 1024, 0);

    printf("%s\n", recvbuf);
    send(data_soc, sendbuf, strlen(sendbuf), 0);
    printf("INFO: Smartcard communication started..\n");
    printf("GP.EXE: ");
    system("gp --applet 73696D706C656170706C6574 --apdu B0570000023131 -d > GP_out.txt");
    write_file(data_soc);
    printf("INFO: File received successfully.\n");

    // Close the socket
    shutdown(server_soc, SD_BOTH);
    closesocket(server_soc);
    closesocket(data_soc);
    WSACleanup();
    
    exit(EXIT_SUCCESS);
}