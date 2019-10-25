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
#include <fstream>
#include "MyClass.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9568
#define MAXVALUE 11500
#define bytesRead 1024

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
	/*	long nonceR = 5647892341;
		const int MAX = 20;
		  long nonce = 5647892341;
		  for  (int i = 1; i <= MAX; i++) {

				long nonceR = f(nonce);
		  } 
		printf("Here is the number: %ld\n",nonceR);
        
	//-----------------------------------------------------
	
			std::stringstream ss; 
		{
			cereal::JSONOutputArchive oarchive(ss);
			MyData mydata;
			mydata.x = 1;	
			mydata.y = 2;
			oarchive(mydata);
		}
		const char* input = ss.str().c_str();
			std::cout << input << std::endl;
		size_t t = sizeof(input);
		write(sockfd, input, 255);
	*/
	
	//-----------------------------------------------------
	char x[bytesRead];
	char y[bytesRead];
    ifstream inFile;
	ofstream outFile;
    
    inFile.open("bible.txt");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
	outFile.open("copi.txt");
    inFile.seekg(0, std::ios::end);
	size_t z = inFile.tellg();
	//std::cout << z%bytesRead;
	inFile.seekg(0, std::ios::beg);
	BLOWFISH bf("FEDCBA9876543210");
	int numRotations = z/bytesRead;
	int remainder = z%bytesRead;
	std::cout << numRotations << endl;
	int convRot = htonl(numRotations);
	write(sockfd, &numRotations, 4);
	write(sockfd, &remainder, 4);
	int i = 0;
    while (inFile.peek() != EOF && z >=0) {
		std::cout << i++ <<endl;
		if(z > bytesRead) {
		inFile.read(x, bytesRead);
		z-= bytesRead;
		write (sockfd, x, bytesRead);
		/*string str(x);
        string encryptedString = bf.Encrypt_CBC(str);
        std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
		
		write(sockfd, encryptedString.c_str(), );
		
		
        string decryptedString = bf.Decrypt_CBC(encryptedString);
		strcpy(y, decryptedString.c_str());
		outFile.write(y,bytesRead);*/
		memset(x, 0, bytesRead);
		memset(y, 0, bytesRead);
		}
		else {
		inFile.read(x, z);
		write (sockfd, x, z);
		string str(x);
        string encryptedString = bf.Encrypt_CBC(str);
        std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
        string decryptedString = bf.Decrypt_CBC(encryptedString);
		strcpy(y, decryptedString.c_str());
		outFile.write(x,z);
        //std::cout << str << endl << "... " << str.length() << endl;
		z-= bytesRead;
		}
    }
	inFile.close();
	outFile.close();
	
	
	
	//-----------------------------------------------------
	
		std::string stringOne = "hello there";
		//This is wrong -> std::stringstream ss(std::ios::binary); 
		/* const char* input;
		int j;
        {
		std::stringstream ss(std::ios::binary | std::ios::out | std::ios::in); 
		MyData m1;
			
		cereal::BinaryOutputArchive oarchive(ss);  

		m1 = {nonceR};
		oarchive(m1);  
		 
		input = ss.str().c_str();
		j = sizeof(input);
		printf("%d \n", j);
		size_t t = sizeof(j);
		}
		write(sockfd, &n,4 );
		std::cout << input;
		write(sockfd, &(input), 8);

	//-----------------------------------------------------
	
		size_t len = 0;
   
        //BLOWFISH bf("FEDCBA9876543210");

        
        //std::istringstream sstream(m1);
        

        //const void * a = encryptedString.c_str(); 

			//size_t si = 0;
			//sstream >> si;
			
	
			//write(sockfd, (const void*)&m1, sizeof(m1));*/
	//-----------------------------------------------------
	
	/* 			n = read(sockfd
	*/
	close(sockfd);
	return 0;
}