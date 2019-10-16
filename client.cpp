#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include "blowfish.h"
#include <sstream>

#define PORT 9553
#define MAXVALUE 11500

using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char*argv[]) {
char buffer[256];
	/*
	============================
	SET UP CONNECTION
	============================
	*/

	int sockfd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 2) {
		fprintf(stderr, "usage is %s hostname\n", argv[0]);
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");
		server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(PORT);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}
	
        //blowfish 

        
        size_t len = 0;
              
       
        BLOWFISH bf("FEDCBA9876543210");
        string stringOne = "hello there";
        string stringTwo = "general kenobi";

        string encryptedString = bf.Encrypt_CBC(stringTwo);
        
    
        std::istringstream sstream(encryptedString);
        


        const void * a = encryptedString.c_str(); 

			size_t si = 0;
			sstream >> si;
				
			write(sockfd, a, si);
				
				n = read(sockfd,buffer,255);
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n",buffer);


	close(sockfd);
	return 0;
}
