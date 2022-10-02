// Code source: https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif

int main(int argc, char const* argv[]) {

    if(argc !=3) {
		printf("ERR: Invalid number of arguments!");
		exit(EXIT_FAILURE);
	}

#if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != NO_ERROR) {
        printf("\nERR: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#endif

    SOCKET client_soc = INVALID_SOCKET;
    int valread;
	struct sockaddr_in server_addr;
	char sendbuf[] = "RASPI: Capturing started";
	char recvbuf[1024] = {0};

	//Create a socket for connecting to server
    if ((client_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
        WSACleanup();
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);     //server's IP address
	server_addr.sin_port = htons(atoi(argv[2]));          //server's port number
    

	if ((connect(client_soc, (struct sockaddr*)&server_addr, sizeof(server_addr))) == SOCKET_ERROR) {
		printf("\nConnecting to server failed \n");
        closesocket (client_soc);
        WSACleanup();
		exit(EXIT_FAILURE);
	}
    
    send(client_soc, sendbuf, strlen(sendbuf), 0);
	printf("INFO: Capturing started..\n");
    system(".\\capture.exe");

    #if defined(_WIN32) || defined(_WIN64)  
        valread = recv(client_soc, recvbuf, 1024, 0);
    #else
        valread = read(client_soc, recvbuf, 1024);
    #endif
	
	printf("%s\n", recvbuf);


// closing the connected socket
#if defined(_WIN32) || defined(_WIN64)  
    closesocket(client_soc);
    WSACleanup();
#else
    close(client_soc);
#endif
	
    exit(EXIT_SUCCESS);
}
