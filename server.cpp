#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include <sstream>
#include "MyClass.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9551
#define MAXVALUE 11500

typedef MyClass MyData;
using namespace std;


void error(const char *msg) {
	perror(msg);
	exit(1);
}

//-----------------------------------------------------

int main (int argc, char *argv[]) {
	
char buffer[256];
	/*
	============================
	SET UP CONNECTION
	============================
	*/

	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	// create a socket
	// socket(int domain, int type, int protocol)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	// clear address structure
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// setup the host_addr structure for use in bind call
	// server byte order
	serv_addr.sin_family = AF_INET;

	// automatically be filled with current hosts IP address
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// convert short integer value for port must be converted into network byte order
	serv_addr.sin_port = htons(PORT);

	// bind(ind fd, struct socaddr *local_addr, socklen_t addr_length)
	// bind() passes file descriptor, the address of the structure,
	// and the length of the address structure
	// This bind() call will bind the socket to the current IP address on port: PORT
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}

	// This listen() call tells the socket to listen to the incoming connections
	// The listen() function places all incoming connection into a backlog queue
	// until accept() call accepts the connection
	// max backlog queue is 5
	listen(sockfd, 5);

	// the accept() call actually accepts an incoming connections
	clilen = sizeof(cli_addr);

	/* This accept() function will write the connecting client's address info
	into the address structure and the size of that structer is clilen.
	The accept() returns a new socket file descriptor for the accepted connection.
	So, the origial socket file descriptor can continue to be used
	for accepting new connections while the new socket file descriptor is used for
	communcicating with the connected client.
	*/
	
	//-----------------------------------------------------	
	
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	
	//-----------------------------------------------------
			char buf[256];
			read(newsockfd, buf, 255);
			std::cout << buf << std::endl;
			std::stringstream ss;
			string str(buf);
			ss << str;
			{
			cereal::JSONInputArchive iarchive(ss);	
			MyData mydata;
			iarchive(mydata);
			std::cout << mydata.nonceOne << std::endl << mydata.request << std::endl;
			}
			
	
	
	//-----------------------------------------------------
	/*MyData m1;
	int siz;
	n = read(newsockfd, &siz, 4);
	//printf("%d \n", siz);
	char buf[8];
	n = read(newsockfd, buf,siz);
	printf("%s \n", buf);
        if (n < 0) error("ERROR reading from socket");
	//printf("Here is the message: %s\n",buffer);
	
	//-----------------------------------------------------
	
	std::istringstream ss(std::ios::binary | std::ios::out | std::ios::in);
	string inp(buf);
	ss.str(inp);
	std::cout << sizeof(ss) << std::endl;
	//std::stringstream ss(buf, std::ios::binary | std::ios::out | std::ios::in); 
	//Error here: Failed to read 8 bytes from input stream! Read 0 Aborted
	
		{
		  
		cereal::BinaryInputArchive iarchive(ss);  
		//iarchive.loadBinary(&buf, sizeof(buf));

		iarchive(m1);  
		//std::cout << "number: " << m1.x << std::endl;
		
		}		*/
		
    //-----------------------------------------------------
	
	/*std::istringstream sstream(buffer);
	size_t si = 0;
	sstream >> si;

	write(newsockfd, buffer, si);
	*/
	
	//-----------------------------------------------------
	
	close(newsockfd);
	close(sockfd);
	return 0;
		
	
}
