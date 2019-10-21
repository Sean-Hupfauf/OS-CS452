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
#include "MyClass.hpp"
#include "cereal/archives/binary.hpp"

#define PORT 9550
#define MAXVALUE 11500

typedef MyClass MyData;
using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(0);
}

//-----------------------------------------------------

long f(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}

//-----------------------------------------------------

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
	//-----------------------------------------------------
	
        //Nonce 
		long nonceR = 5647892341;
		const int MAX = 20;
		  long nonce = 5647892341;
		  for  (int i = 1; i <= MAX; i++) {

				long nonceR = f(nonce);
		  } 
		printf("Here is the number: %ld\n",nonceR);
        
	//-----------------------------------------------------
	
		std::string stringOne = "hello there";
		//This is wrong -> std::stringstream ss(std::ios::binary); 
		std::stringstream ss(std::ios::binary | std::ios::out | std::ios::in); 
		MyData m1;
        {
			
		cereal::BinaryOutputArchive oarchive(ss);  

		m1 = {nonceR};
		oarchive(m1);  
		
		}

	//-----------------------------------------------------
	
		size_t len = 0;
   
        //BLOWFISH bf("FEDCBA9876543210");

        //string encryptedString = bf.Encrypt_CBC("hey");
        
        //std::istringstream sstream(m1);
        

        //const void * a = encryptedString.c_str(); 

			size_t si = 0;
			ss >> si;
			
	
			write(sockfd, (const void*)&ss, si);
	//-----------------------------------------------------
	
	/* 			n = read(sockfd,buffer,255);
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n",buffer);
    */

	close(sockfd);
	return 0;
}
