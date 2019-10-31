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
#include "PartTwo.hpp"
#include "PartTwoB.hpp"
#include "blowfisher.hpp"
#include "blowfishOne.h"
#include "blowfishTwo.h"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9556
#define MAXVALUE 11500

typedef MyClass MyData;
typedef PartTwo MyTwo;
typedef PartTwoB MyTwoB;
typedef blowfisher MyBlow;

using namespace std;


void error(const char *msg) {
	perror(msg);
	exit(1);
}

//-----------------------------------------------------



int main (int argc, char *argv[]) {
	
char buffer[256];

    std::string sessionKey = "FEDCBA9876543210";
	std::string aKey = "AEDCBA9876543210";
	std::string bKey = "BEDCBA9876543210";

    // std::cout << "----KDC Setup----" << endl;
	
	// std::cout << "Enter session key: ";
	// std::cin >> sessionKey;
	
	// std::cout << "Enter client private key for 'A': ";
	// std::cin >> aKey;
	
	// std::cout << "Enter client private key for 'B': ";
	// std::cin >> bKey;
	
	




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
	std::string request;
	
	//-----------------------------------------------------
	//Reads in the request and nonce, copies both.
	
	char buf[256];
	read(newsockfd, buf, 255);
	std::stringstream ss;
	string str(buf);
	ss << str;
	{
	cereal::JSONInputArchive iarchive(ss);	
	MyData mydata;
	iarchive(mydata);

	nonceOne = mydata.nonceOne;
	request = mydata.request;
	}
			
	//print out step 2
    std::cout << endl;
    std::cout << "Recd from ID_A:" << endl;
    std::cout << "Requesting: " << request << endl;
    std::cout << "N1 recieved: " << nonceOne << endl;
    std::cout << endl;

     	
			
	
	std::string inputM;
	std::string inputBefore;
	
	//-----------------------------------------------------
	//First it creates the payload going to b, seralizes it, and encrypts it
	
	std::stringstream sf;

		{
			cereal::JSONOutputArchive oarchive(sf);
			MyTwoB mytwoB;
			
			mytwoB.IDa = "10.35.195.46";
			mytwoB.sessionKey = sessionKey;  //session key
			
			oarchive(mytwoB);
		}
		const char* inputB = sf.str().c_str();
		BLOWFISHONE b(bKey);  // b key
		BLOWFISHTWO bf(aKey);   // a key  //dont follow!!!!!!!!!!!!!
		inputBefore = b.Encrypt_CBC(inputB); // encrypts with b 
	
	
	//===============================================
	//Then it combines the encrypted string b payload with the stuff going with it in part two
	//Serializes it and then encrypts it again.
	
	
	std::stringstream st;

		{
			cereal::JSONOutputArchive oarchive(st);
			MyTwo mytwo;
			
			mytwo.nonceOne = nonceOne;	
			mytwo.request = request;
			mytwo.encryptedString = inputBefore;
			mytwo.sessionKey = sessionKey;  //session key
			
			oarchive(mytwo);
		}
		const char* input = st.str().c_str();
		inputM = bf.Encrypt_CBC(input);  //encrypt with 'a' key
	
	
	//-----------------------------------------------------
	//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
	
	//print out step 3
	
	std::cout << endl;
    std::cout << "Send to ID_A:" << endl;
    std::cout << "EKa[Ks||Req||N1||EKb(Ks,IDA)]" << endl;
    std::cout << "Ks: " << sessionKey << endl;
    std::cout << "N1: " << nonceOne << endl;
    std::cout << endl;
	
	
	
	std::stringstream sr;
		{
			cereal::JSONOutputArchive oarchive(sr);
			MyBlow myblow;
			myblow.encryptedString = inputM;	
			
			oarchive(myblow);
		}
		
		std::string nextx = sr.str();
		
		//size_t t = sizeof(nextx);
		write(newsockfd, nextx.c_str(), 1000);
		
	//-----------------------------------------------------
		
	
	close(newsockfd);
	close(sockfd);
	return 0;
		
	
}
