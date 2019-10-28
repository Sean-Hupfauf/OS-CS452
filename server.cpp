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
#include <sys/wait.h>
#include <math.h>
#include <sstream>
#include "MyClass.hpp"
#include "PartTwo.hpp"
#include "PartTwoB.hpp"
#include "blowfisher.hpp"
#include "blowfish.h"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9535
#define MAXVALUE 11500
#define LENGTH 990 

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
	char revbuf[LENGTH];
//char buffer[256];
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
	
			// char buf[256];
			// read(newsockfd, buf, 255);
			// std::cout << buf << std::endl;
			// std::stringstream ss;
			// string str(buf);
			// ss << str;
			// {
			// cereal::JSONInputArchive iarchive(ss);	
			// MyData mydata;
			// iarchive(mydata);

			// nonceOne = mydata.nonceOne;
			// request = mydata.request;
			// }
			
			// char buffer[256];
            // bzero(buffer,256);
            // int m = 0;
			// printf("here1");
            // m = read(newsockfd, buffer, 255);
            // if (m < 0) error("ERROR reading from socket");
            // printf("msg: %s\n",buffer);
			
	// std::string inputM;
	// std::string inputBefore;
	
	//-----------------------------------------------------
	
	/* Defining Variables */
	//int sockfd; 
	 
	int num;
	int sin_size; 
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	//char revbuf[LENGTH]; // Receiver buffer

	// /* Get the Socket file descriptor */
	// if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	// {
		// fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		// exit(1);
	// }
	// else 
		// printf("[Server] Obtaining socket descriptor successfully.\n");

	// /* Fill the client socket address struct */
	// addr_local.sin_family = AF_INET; // Protocol Family
	// addr_local.sin_port = htons(PORT); // Port number
	// addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
	// bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

	// /* Bind a special Port */
	// if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	// {
		// fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		// exit(1);
	// }
	// else 
		// printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

	// /* Listen remote connect/calling */
	// if(listen(sockfd,BACKLOG) == -1)
	// {
		// fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
		// exit(1);
	// }
	// else
		// printf ("[Server] Listening the port %d successfully.\n", PORT);

	// int success = 0;
	// while(success == 0)
	// {
		// sin_size = sizeof(struct sockaddr_in);

		// /* Wait a connection, and obtain a new socket file despriptor for single connection */
		// if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		// {
		    // fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			// exit(1);
		// }
		// else 
			// printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

		/*Receive File from Client */
		BLOWFISH bf("FEDCBA9876543210");
		std::string varTwo;
		char* fr_name = "output.txt";
		FILE *fr = fopen(fr_name, "a");
		if(fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{
			bzero(revbuf, LENGTH); 
			int fr_block_sz = 0;
			while((fr_block_sz = recv(newsockfd, revbuf, LENGTH, 0)) > 0) 
			{
				 std::stringstream ss;
					string str(revbuf);
					std::cout << str << std::endl;
					ss << str;
					{
					cereal::JSONInputArchive iarchive(ss);	
					MyBlow myblow;
					iarchive(myblow);

					varTwo = myblow.encryptedString;
			
					}
				
				
				 varTwo = bf.Decrypt_CBC(varTwo);
				
				
				
			    int write_sz = fwrite(varTwo.c_str(), sizeof(char), fr_block_sz, fr);
				if(write_sz < fr_block_sz)
			    {
			        error("File write failed on server.\n");
			    }
				bzero(revbuf, LENGTH);
				if (fr_block_sz == 0 || fr_block_sz != 512) 
				{
					break;
				}
			}
			if(fr_block_sz < 0)
		    {
		        if (errno == EAGAIN)
	        	{
	                printf("recv() timed out.\n");
	            }
	            else
	            {
	                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
					exit(1);
	            }
        	}
			printf("Ok received from client!\n");
			fclose(fr); 
		}

		

		
		    close(newsockfd);
		    printf("[Server] Connection with Client closed. Server will wait now...\n");
		    while(waitpid(-1, NULL, WNOHANG) > 0);
	
	//First it creates the payload going to b, seralizes it, and encrypts it
	
	// std::stringstream sf;

		// {
			// cereal::JSONOutputArchive oarchive(sf);
			// MyTwoB mytwoB;
			
			// mytwoB.IDa = "10.35.195.46";
			// mytwoB.sessionKey = "FEDCBA9876543210";
			
			// oarchive(mytwoB);
		// }
		// const char* inputB = sf.str().c_str();
		// BLOWFISH bf("FEDCBA9876543210");
		// BLOWFISH b("AEDCBA9876543210");
		// inputBefore = bf.Encrypt_CBC(inputB);
	
	
	//===============================================
	//Then it combines the encrypted string b payload with the stuff going with it in part two
	//Serializes it and then encrypts it again.
	
	
	// std::stringstream st;

		// {
			// cereal::JSONOutputArchive oarchive(st);
			// MyTwo mytwo;
			
			// mytwo.nonceOne = nonceOne;	
			// mytwo.request = request;
			// mytwo.encryptedString = inputBefore;
			// mytwo.sessionKey = "FEDCBA9876543210";
			
			// oarchive(mytwo);
		// }
		// const char* input = st.str().c_str();
		// inputM = b.Encrypt_CBC(input);
	
	
	//-----------------------------------------------------
	//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
	
	// std::stringstream sr;
		// {
			// cereal::JSONOutputArchive oarchive(sr);
			// MyBlow myblow;
			// myblow.encryptedString = inputM;	
			
			// oarchive(myblow);
		// }
		
		// std::string nextx = sr.str();
		
		//size_t t = sizeof(nextx);
		// write(newsockfd, nextx.c_str(), 1000);
		
	//-----------------------------------------------------
	
	
	close(newsockfd);
	close(sockfd);
	return 0;
		
	
}
