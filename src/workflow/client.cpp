/* 
    Code source: https://www.geeksforgeeks.org/socket-programming-cc/
    TCP client code for Raspberry PI
    Usage: client <SERVER IP ADDRESS> <SERVER PORT>
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd) {
  int n;
  char data[SIZE] = {0};
 
  while(fgets(data, SIZE, fp) != NULL) {
    if(send(sockfd, data, sizeof(data), 0) < 0) {
      perror("ERR: Sending file failed.");
      exit(1);
    }
    memset(data, 0, SIZE);
  }
}

int main(int argc, char const* argv[]) {

    if(argc !=3) {
	    printf("ERR: Invalid number of arguments!");
	    exit(EXIT_FAILURE);
    }

    int client_soc = -1;
    int valread;
    struct sockaddr_in server_addr;
    char sendbuf[] = "RASPI: Capturing started";
    FILE *fp;
    char filename[] = "raw_data.txt";

    //Create a socket for connecting to server
    if ((client_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	    printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
	    exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  //server's IP address
    server_addr.sin_port = htons(atoi(argv[2]));       //server's port number
    

    if ((connect(client_soc, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0) {
	    printf("\nConnecting to server failed \n");
	    close(client_soc);
	    exit(EXIT_FAILURE);
    }
    
    send(client_soc, sendbuf, strlen(sendbuf), 0);
    printf("INFO: Capturing started..\n");
    system("./executables/capture");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("ERR: Reading file failed.");
        exit(EXIT_FAILURE);
    }
    send_file(fp, client_soc);
    printf("INFO: File sent successfully.\n");

    //Close client socket
    close(client_soc);
	
    exit(EXIT_SUCCESS);
}
