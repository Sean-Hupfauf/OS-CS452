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

#define PORT 9537
#define MAXVALUE 11500

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
		  long zach = 5647892341;
		  for  (int i = 1; i <= MAX; i++) {

				long nonceR = f(zach);
		  } 
		  
		int nonce = randomNumber();  
		  
        
	//-----------------------------------------------------
	
			std::stringstream ss; 
		{
			cereal::JSONOutputArchive oarchive(ss);
			MyData mydata;
			mydata.nonceOne = nonce;	
			mydata.request = "Request: Ks for IDb";
			
			oarchive(mydata);
		}
		
		const char* input = ss.str().c_str();
		size_t t = sizeof(input);
		write(sockfd, input, 255);
	
	
	//-----------------------------------------------------
	std::string keyers;
	std::string keyers2;
	BLOWFISH bf("FEDCBA9876543210");

			std::stringstream sr; 
			char buf[1000];
			
			read(sockfd, buf, 1000);
			
			string str(buf);
			
			sr << str;
			
			{
			cereal::JSONInputArchive iarchive(sr);	
			MyBlow myblow;
			iarchive(myblow);
			keyers=myblow.encryptedString;
			} 
			
			//-----------------------------------------------------
			//Got the big chunk in part, deserilize and get the encryptedString
			//with everything in it.
			
			std::string varTwo = bf.Decrypt_CBC(keyers);
			
			std::stringstream st;
			st << varTwo;
			
			{
			cereal::JSONInputArchive iarchive(st);	
			MyTwo mytwo;
			iarchive(mytwo);
			keyers2=mytwo.encryptedString;
			} 
			
			//-----------------------------------------------------
			//Decypted and exposed the Ks, Request, Nonce, and encryptedString for
			//B's encrypted data containing Ks and IDa
			
			std::string varThree = bf.Decrypt_CBC(keyers2);
			
			std::stringstream sq;
			sq << varThree;
			
			//-----------------------------------------------------
			//Decypted and exposed B's encrypted data containing Ks and IDa
			//Now it gets deserilized below
			
			{
				
			cereal::JSONInputArchive iarchive(sq);	
			MyTwoB mytwoB;
			iarchive(mytwoB);
			std::cout << mytwoB.IDa << std::endl << mytwoB.sessionKey << std::endl;;
			
			} 
			
	//-----------------------------------------------------
		
		
	close(sockfd);
	return 0;
}
