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
#include "MyNumbers.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9569
#define MAXVALUE 11500
#define bytesRead 1024

typedef MyClass MyData;
typedef MyNumbers MyNumber;
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
	
	//variable declaration
	char x[bytesRead];
    ifstream inFile;
	//ofstream outFile;
    
	//Open Input File
    inFile.open("world192.txt");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
	
	//Figuring Out Size of File
    inFile.seekg(0, std::ios::end);
	size_t z = inFile.tellg();
	//std::cout << z%bytesRead;
	inFile.seekg(0, std::ios::beg);
	
	
	BLOWFISH bf("FEDCB98765432100");
	
	//Number of Strings Sent Over
	int numRotations = z/bytesRead;
	uint32_t convRot = htonl(numRotations);
	write(sockfd, &convRot, sizeof(convRot));
	
	//Number of Chars on Last String
	int remainder = z%bytesRead;
	uint32_t convRem = htonl(remainder);
	write(sockfd, &convRem, sizeof(convRem));
	
	char wri[3000];
	//std::cout << numRotations << endl;
	//int i = 0;
    while (inFile.peek() != EOF && z >=0) {
		//std::cout << i++ <<endl;
		
		if(z > bytesRead) {
			//Read from File
			inFile.read(x, bytesRead);
			z-= bytesRead;
			
			//write (sockfd, x, bytesRead);
			
			//Convert to String
			string str(x);
			//Convert to Encryption String
			string encryptedString = bf.Encrypt_CBC(str);

			std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
			
			//Length of Encrypted String
			//int leng = strlen(encryptedString.c_str());
			int leng = strlen(x);
			uint32_t convLeng = htonl(leng);
			std::cout << leng <<endl;
			write(sockfd, &convLeng, sizeof(convLeng));
			
			//Write Encrypted String
			strcpy(wri, encryptedString.c_str());
			write(sockfd, x, leng);

			//Resets x
			memset(x, 0, bytesRead);
			memset(wri, 0, 2999);
		}
		else {
			//Read the File
			inFile.read(x, z);
			//write (sockfd, x, z);
			
			//Convert to String
			string str(x);
			
			//Encrypt String
			string encryptedString = bf.Encrypt_CBC(str);
			//std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
			
			
			//string decryptedString = bf.Decrypt_CBC(encryptedString);
			//strcpy(y, decryptedString.c_str());
			//outFile.write(x,z);
			//std::cout << str << endl << "... " << str.length() << endl;
			z-= bytesRead;
		}
    }
	inFile.close();
	//outFile.close();
	
	
	
	//-----------------------------------------------------
	
		//std::string stringOne = "hello there";
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