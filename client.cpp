#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
#include "blowfisher.hpp"
#include <sstream>
#include "MyClass.hpp"
#include "PartTwo.hpp"
#include "PartTwoB.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9535
#define MAXVALUE 11500
#define LENGTH 512 

typedef MyClass MyData;
typedef PartTwo MyTwo;
typedef blowfisher MyBlow;
typedef PartTwoB MyTwoB;

using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(0);
}

//-----------------------------------------------------

int randomNumber() {
	
    int iSecret;

    /* initialize random seed: */
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    iSecret = rand() % 10 + 1;
    
	return iSecret;
}


//-----------------------------------------------------

/* long f(long nonce) {
    
} */

//-----------------------------------------------------

int main(int argc, char*argv[]) {
//char buffer[256];
char revbuf[LENGTH];
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
	
       
		  
		int nonce1 = randomNumber();  
		  
        
	//-----------------------------------------------------
	
			// std::stringstream ss; 
		// {
			// cereal::JSONOutputArchive oarchive(ss);
			// MyData mydata;
			// mydata.nonceOne = nonce1;	
			// mydata.request = "Request: Ks for IDb";
			
			// oarchive(mydata);
		// }
		
		// const char* input = ss.str().c_str();
		//size_t t = sizeof(input);
	   // write(sockfd, input, 255);
		
	// char* fs_name = "test.txt";
	// printf("here1");
    // char sdbuf[LENGTH]; 
	// printf("here2");
            // char buffer[256];
			// printf("here3");
            // int m;
            // fgets(buffer,255,stdin);
            // printf("here4");
            // m = write(sockfd,buffer, strlen(buffer));
			// printf("here5");
			// bzero(buffer,256);
			// printf("here6");
            // if(m<0) printf("Error: sending filename");
	
	//-----------------------------------------------------
	
	/* Variable Definition */
	
	
	//char revbuf[LENGTH]; 
	struct sockaddr_in remote_addr;

	// /* Get the Socket file descriptor */
	// if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	// {
		// fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
		// exit(1);
	// }

	// /* Fill the socket address struct */
	// remote_addr.sin_family = AF_INET; 
	// remote_addr.sin_port = htons(PORT); 
	// inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
	// bzero(&(remote_addr.sin_zero), 8);

	// /* Try to connect the remote */
	// if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	// {
		// fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
		// exit(1);
	// }
	// else 
		// printf("[Client] Connected to server at port %d...ok!\n", PORT);

	/* Send File to Server */
	//if(!fork())
	//{
		BLOWFISH bf("FEDCBA9876543210");
		char* fs_name = "test.txt";
		char sdbuf[LENGTH]; 
		printf("[Client] Sending %s to the Server... ", fs_name);
		FILE *fs = fopen(fs_name, "r");
		if(fs == NULL)
		{
			printf("ERROR: File %s not found.\n", fs_name);
			exit(1);
		}
		std::string inputBefore;
		bzero(sdbuf, LENGTH); 
		int fs_block_sz; 
		std::stringstream ss; 
		while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
		{	
			inputBefore = bf.Encrypt_CBC(sdbuf);
		{
			cereal::JSONOutputArchive oarchive(ss);
			MyBlow myblow;
			myblow.encryptedString = inputBefore;	
		
			oarchive(myblow);
		}
		std::string input = ss.str();
		std::cout << input << std::endl;
		
		// size_t len = 0;
		// if (1 == sscanf(input.c_str(), "%zu", &len))
		// printf("len is %zu\n", len);
			
		    if(send(sockfd, input.c_str(), 990, 0) < 0)
		    {
		        fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
		        break;
		    }
		    bzero(sdbuf, LENGTH);
		}
		printf("Ok File %s from Client was Sent!\n", fs_name);
	//}

	// /* Receive File from Server */
	// printf("[Client] Receiveing file from Server and saving it as final.txt...");
	// char* fr_name = "/home/aryan/Desktop/progetto/final.txt";
	// FILE *fr = fopen(fr_name, "a");
	// if(fr == NULL)
		// printf("File %s Cannot be opened.\n", fr_name);
	// else
	// {
		// bzero(revbuf, LENGTH); 
		// int fr_block_sz = 0;
	    // while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
	    // {
			// int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
	        // if(write_sz < fr_block_sz)
			// {
	            // error("File write failed.\n");
	        // }
			// bzero(revbuf, LENGTH);
			// if (fr_block_sz == 0 || fr_block_sz != 512) 
			// {
				// break;
			// }
		// }
		// if(fr_block_sz < 0)
        // {
			// if (errno == EAGAIN)
			// {
				// printf("recv() timed out.\n");
			// }
			// else
			// {
				// fprintf(stderr, "recv() failed due to errno = %d\n", errno);
			// }
		// }
	    // printf("Ok received from server!\n");
	    // fclose(fr);
	// }
	
	printf("[Client] Connection lost.\n");
	
	
	
	
	// std::string keyers;
	// std::string keyers2;
	// BLOWFISH bf("FEDCBA9876543210");
	// BLOWFISH b("AEDCBA9876543210");

			// std::stringstream sr; 
			// char buf[1000];
			
			// read(sockfd, buf, 1000);
			
			// string str(buf);
			
			// sr << str;
			
			// {
			// cereal::JSONInputArchive iarchive(sr);	
			// MyBlow myblow;
			// iarchive(myblow);
			// keyers=myblow.encryptedString;
			// } 
			
			//-----------------------------------------------------
			//Got the big chunk in part, deserilize and get the encryptedString
			//with everything in it.
			
			// std::string varTwo = bf.Decrypt_CBC(keyers);
			
			// std::stringstream st;
			// st << varTwo;
			// std::cout << varTwo << std::endl;
			// {
			// cereal::JSONInputArchive iarchive(st);	
			// MyTwo mytwo;
			// iarchive(mytwo);
			// keyers2=mytwo.encryptedString;
			//} 
			
			
			
	//-----------------------------------------------------
		
	close(sockfd);
	//#define PORT 9538
	// int sockfb, m;
	// struct sockaddr_in serv_addr2;
	// struct hostent *server2;
/* 
	if (argc < 2) {
		fprintf(stderr, "usage is %s hostname\n", argv[0]);
		exit(0);
	}
*/
	
	// sockfb = socket(AF_INET, SOCK_STREAM, 0);
	// if (sockfb < 0) error("ERROR opening socket");
		// server2 = gethostbyname(argv[2]);
	 // if (server2 == NULL) {
		// fprintf(stderr, "ERROR, no such host\n");
		// exit(0);
	 // } 

	// bzero((char *) &serv_addr2, sizeof(serv_addr2));
	// serv_addr2.sin_family = AF_INET;
	// bcopy((char *)server2->h_addr,
		// (char *)&serv_addr2.sin_addr.s_addr,
		// server2->h_length);
	// serv_addr2.sin_port = htons(9518);
	 
	// if (connect(sockfb, (struct sockaddr *) &serv_addr2, sizeof(serv_addr2)) < 0) {
		// error("ERROR connecting");
	// }
	 
	 
	//-----------------------------------------------------
	//Then it seralizes the complete encrypted payload that is going to B and writes it to B.
	// std::string newkey;
	// std::stringstream rs;
		// {
			// cereal::JSONOutputArchive oarchive(rs);
			// MyBlow myblow;
			// myblow.encryptedString = keyers2;	
			
			// oarchive(myblow);
		// }
		
		// std::string nextx = rs.str();
		//std::cout << nextx << std::endl;
		//size_t t = sizeof(nextx);
		//write(sockfb, nextx.c_str(), 1000);
	
	
	//-----------------------------------------------------
	//Part 4 gets the serialized encrypted nonce and deserilize it
	
	// std::stringstream er; 
	// char bufe[1000];
	
	// read(sockfb, bufe, 1000);
	
	// string stri(bufe);
	
	// er << stri;
	
	// {
	// cereal::JSONInputArchive iarchive(er);	
	// MyBlow myblow;
	// iarchive(myblow);
	// newkey=myblow.encryptedString;
	//std::cout << newkey << std::endl;
	//} 
	
	//-----------------------------------------------------
	//Decrypt it and deserilize it again to expose the nonce
	
	// int noncetwo;
	
	
	// std::string varNew = bf.Decrypt_CBC(newkey);
	
	// std::stringstream qw;
	// qw << varNew;
	//std::cout << varNew << std::endl;
	// {
	// cereal::JSONInputArchive iarchive(qw);	
	// MyClass mydata2;
	// iarchive(mydata2);
	// noncetwo=mydata2.nonceOne;
	// } 
	// std::cout << noncetwo << std::endl;
	
	//---------------------------------------------------------====
	//long fnonce = f(noncetwo);
	
	// const long A = 48271;
    // const long M = 2147483647;
    // const long Q = M/A;
    // const long R = M%A;

	// static long state = 65;
	// long t = A * (state % Q) - R * (state / Q);
	
	
	// if (t > 0)
		// state = t;
	// else
		// state = t + M;
	// long fnonce=(long)((((double) state/M)* noncetwo)+(M/noncetwo));
	
	
	// std::string inputN;
	// std::cout << fnonce << std::endl;
	// std::stringstream as; 
			// {
				// cereal::JSONOutputArchive oarchive(as);
				// MyData mydata5;
				// mydata5.fnoncer = fnonce;	
				
				// oarchive(mydata5);
			// }
			
			// const char* input5 = as.str().c_str();

			// inputN = b.Encrypt_CBC(input5);
			
			//-----------------------------------------------------
			//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
			
			// std::stringstream zx;
				// {
					// cereal::JSONOutputArchive oarchive(zx);
					// MyBlow myblow5;
					// myblow5.encryptedString = inputN;	
					
					// oarchive(myblow5);
				// }
				
				// std::string nexty = zx.str();
				
				
				// write(sockfb, nexty.c_str(), 1000);
	
	
	// close(sockfb);
	
	
	return 0;
}
