/* 
    Code: https://www.geeksforgeeks.org/socket-programming-cc/ adapted for the purpose of Windows-Raspberry Pi testing
    TCP client code for Raspberry PI
    g++ -o ./build/client ./src/app/client.cpp
    Usage: client <SERVER IP ADDRESS> <SERVER PORT>
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <math.h>
#include <vector>
#include <algorithm>

#define BUFFER_SIZE 16384

std::vector<int> calculateDigits(int number) {
    std::vector<int> digits;
    while(number > 0) {
	digits.push_back(number % 10);
	number/=10;
    }
    std::reverse(digits.begin(), digits.end());
    return digits;
}

char* constructFilename(std::vector<int> digits) {
    char filename[] = "_data.txt";
    char* fullname = (char *)malloc(digits.size() + strlen(filename) + 1);
    for(int i=0; i < digits.size(); i++) {
	fullname[i] = digits[i] + '0';
    }
    strcat(fullname, filename);
    return fullname;
}

int main(int argc, char const* argv[]) {

    if(argc !=3) {
	printf("ERR: Invalid number of arguments!");
	exit(EXIT_FAILURE);
    }

    int client_soc = -1;
    struct sockaddr_in server_addr;
    FILE *fp;
    int ctr = 1; //sample counter
    char* filename;
    char syncbuf[] = "####";
    char recvbuf[5] = {0};
    char run[] = "./executables/pigpio_sampler ";

    //Create a socket for connecting to server
    if ((client_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	    printf("ERR: Could not create socket: %s. Exiting..\n", strerror(errno));
	    exit(EXIT_FAILURE);
    }
  
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  //server's IP address
    server_addr.sin_port = htons(atoi(argv[2]));       //server's port number
    
    int connect_status = connect(client_soc, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (connect_status < 0) {
	    printf("ERR: Connecting to server failed.\n");
	    close(client_soc);
	    exit(EXIT_FAILURE);
    } else {
        printf("INFO: Connection with the server established. \n");
    }

    while(true) {
	std::vector<int> ctr_digits = calculateDigits(ctr);
	filename = constructFilename(ctr_digits);
	char* command;
	command = (char *)malloc(strlen(run) + strlen(filename) + 1);
	strcpy(command, run);
	strcat(command, filename);  
        
	printf("INFO: Sending sync signal to the server.\n");
	send(client_soc, syncbuf, strlen(syncbuf), 0);
        printf("INFO: Capturing started..\n");
        system(command); //start capturing
	free(command);
	free(filename);

        printf("INFO: Awaiting server signal..\n");
	memset(recvbuf, 0, sizeof(recvbuf)); //clear the buffer
	recv(client_soc, recvbuf, sizeof(recvbuf),0);
	printf(">> %s\n", recvbuf);
        
	if(strcmp(recvbuf, "STOP") == 0) {
	    printf("INFO: Received stop signal from Windows. Exiting..\n");
	    break;
	}
	ctr +=1;
	printf("INFO: Sleeping for 10 seconds..\n");
	sleep(10); //waiting for the card to have 0 on the I/O pin
    }
    
    //Close client socket
    close(client_soc);
	
    exit(EXIT_SUCCESS);
}
