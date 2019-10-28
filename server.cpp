#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <bitset>
#include <math.h>
#include <sstream>
#include "MyClass.hpp"
#include "PartTwo.hpp"
#include "PartTwoB.hpp"
#include "blowfisher.hpp"
#include "blowfish.h"
#include <vector>
#include "files.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9537
#define MAXVALUE 11500

typedef MyClass MyData;
typedef PartTwo MyTwo;
typedef PartTwoB MyTwoB;
typedef blowfisher MyBlow;
typedef files MyFile;

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
	
	int nonceOne;
	std::string efile;
	BLOWFISH bf("FEDCBA9876543210");
	//-----------------------------------------------------
	//Reads in the request and nonce, copies both.
	
			char buf[1000];
			read(newsockfd, buf, 1000);
			//std::cout << buf << std::endl;
			std::stringstream ss;
			string str(buf);
			ss << str;
			{
			cereal::JSONInputArchive iarchive(ss);	
			//std::cout << ss << std::endl;
			MyFile myfile;
			iarchive(myfile);
			std::cout << myfile.x << std::endl;
			efile = myfile.x;
			//request = mydata.request;
			}
			std::string newfile = bf.Decrypt_CBC(efile);
			std::cout << newfile << std::endl;
			
			std::ofstream out("output.txt");
			out << newfile;
			out.close();
	//std::string inputM;
	//std::string inputBefore;
	
	//-----------------------------------------------------
	//First it creates the payload going to b, seralizes it, and encrypts it
	
	/* std::stringstream sf;

		{
			cereal::JSONOutputArchive oarchive(sf);
			MyTwoB mytwoB;
			
			mytwoB.IDa = "10.35.195.46";
			mytwoB.sessionKey = "FEDCBA9876543210";
			
			oarchive(mytwoB);
		}
		const char* inputB = sf.str().c_str();
		BLOWFISH bf("FEDCBA9876543210");
		BLOWFISH b("AEDCBA9876543210");
		inputBefore = bf.Encrypt_CBC(inputB); */
	
	
	//===============================================
	//Then it combines the encrypted string b payload with the stuff going with it in part two
	//Serializes it and then encrypts it again.
	
	/* 
	std::stringstream st;

		{
			cereal::JSONOutputArchive oarchive(st);
			MyTwo mytwo;
			
			mytwo.nonceOne = nonceOne;	
			mytwo.request = request;
			mytwo.encryptedString = inputBefore;
			mytwo.sessionKey = "FEDCBA9876543210";
			
			oarchive(mytwo);
		}
		const char* input = st.str().c_str();
		inputM = b.Encrypt_CBC(input); */
	
	
	//-----------------------------------------------------
	//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
	
	/* std::stringstream sr;
		{
			cereal::JSONOutputArchive oarchive(sr);
			MyBlow myblow;
			myblow.encryptedString = inputM;	
			
			oarchive(myblow);
		}
		
		std::string nextx = sr.str();
		
		//size_t t = sizeof(nextx);
		write(newsockfd, nextx.c_str(), 1000);
		 */
	//-----------------------------------------------------
	
	
	close(newsockfd);
	close(sockfd);
	return 0;
		
	
}
