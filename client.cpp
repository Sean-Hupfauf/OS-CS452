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
#include "blowfishOne.h"
#include "blowfishTwo.h"
#include "blowfisher.hpp"
#include <sstream>
#include "MyClass.hpp"
#include "PartTwo.hpp"
#include "PartTwoB.hpp"
#include "md5.hpp"
#include "secureConnection.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9556
#define MAXVALUE 11500
#define bytesRead 1024

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
/*
int randomNumber() {
	
    int iSecret;

    /* initialize random seed: */
    // srand (time(NULL));

    // /* generate secret number between 1 and 10: */
    // iSecret = rand() % 10 + 1;
    
	// return iSecret;
// }


//-----------------------------------------------------

/* long f(long nonce) {
    
} */

//-----------------------------------------------------

int main(int argc, char*argv[]) {
	
	long nonceOne = 45;
	std::string aKey = "AEDCBA9876543210";
	std::string sessionKey = "FEDCBA9876543210";
	int option = 0;
    // std::cout << "----Client 'A' Setup----" << endl;
	
	// std::cout << "Enter a nonce: ";
	// std::cin >> nonceOne;
	
	// std::cout << "Enter client private key for 'A': ";
	// std::cin >> aKey;
	
	std::cout << "Press [0] for file transfer and [1] for sentence tranfer: ";
	std::cin >> option;
	
	
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
    //starts process by asking kdc for session key by sending a nonce and a request
	
			std::stringstream ss; 
		std::string request;
		{
			cereal::JSONOutputArchive oarchive(ss);
			MyData mydata;
			mydata.nonceOne = nonceOne;	
			mydata.request = "Ks for IDb";
			request = mydata.request;
			oarchive(mydata);
		}
		
		//print part 1
		
		std::cout << endl;
		std::cout << "Sent (KDC):" << endl;
		std::cout << "Request: " << request << endl;
		std::cout << "N1 = " << nonceOne << endl;
		std::cout << endl;
		
		const char* input = ss.str().c_str();
		//size_t t = sizeof(input);
		write(sockfd, input, 255);
	
	
	//-----------------------------------------------------
	std::string keyers;
	std::string keyers2;
	BLOWFISHONE bf(aKey);  //a key

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
			
			std::string varTwo = bf.Decrypt_CBC(keyers); //decrypt with 'a's key
			
			
			
			
			std::stringstream st;
			st << varTwo;
			
			{
			cereal::JSONInputArchive iarchive(st);	
			MyTwo mytwo;
			iarchive(mytwo);
			sessionKey = mytwo.sessionKey;
			keyers2=mytwo.encryptedString;
			} 
			
			
			//print part 4
			
			std::cout << endl;
			std::cout << "Recd from KDC:" << endl;
			std::cout << "Ks: " << sessionKey << endl;
			std::cout << "N1: " << nonceOne << endl;
			std::cout << endl;
			
			
			
	//-----------------------------------------------------
		
	close(sockfd);
	
	
	
	
	//#define PORT 9538
	int sockfb, m;
	struct sockaddr_in serv_addr2;
	struct hostent *server2;
/* 
	if (argc < 2) {
		fprintf(stderr, "usage is %s hostname\n", argv[0]);
		exit(0);
	}
*/
	
	sockfb = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfb < 0) error("ERROR opening socket");
		server2 = gethostbyname(argv[2]);
	 if (server2 == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	 } 

	bzero((char *) &serv_addr2, sizeof(serv_addr2));
	serv_addr2.sin_family = AF_INET;
	bcopy((char *)server2->h_addr,
		(char *)&serv_addr2.sin_addr.s_addr,
		server2->h_length);
	serv_addr2.sin_port = htons(9518);
	 
	if (connect(sockfb, (struct sockaddr *) &serv_addr2, sizeof(serv_addr2)) < 0) {
		error("ERROR connecting");
	}
	 
	 
	//-----------------------------------------------------
	//Then it seralizes the complete encrypted payload that is going to B and writes it to B.
	std::string newkey;
	std::stringstream rs;
		{
			cereal::JSONOutputArchive oarchive(rs);
			MyBlow myblow;
			myblow.encryptedString = keyers2;	
			
			oarchive(myblow);
		}
		
	//print part 5
	
	std::cout << endl;
	std::cout << "Send to ID_B:" << endl;
	std::cout << "EKb[Ks,IDA]: " << keyers2 << endl;
	std::cout << endl;
	
		
		
		std::string nextx = rs.str();
		//std::cout << nextx << std::endl;
		//size_t t = sizeof(nextx);
		write(sockfb, nextx.c_str(), 1000);   // to server2
	
	
	//-----------------------------------------------------
	//Part 4 gets the serialized encrypted nonce and deserilize it
	
	std::stringstream er; 
	char bufe[1000];
	
	read(sockfb, bufe, 1000);
	
	string stri(bufe);
	
	er << stri;
	
	{
	cereal::JSONInputArchive iarchive(er);	
	MyBlow myblow;
	iarchive(myblow);
	newkey=myblow.encryptedString;

	} 
	
	//-----------------------------------------------------
	//Decrypt it and deserilize it again to expose the nonce
	
	int noncetwo;
	
	BLOWFISHTWO b(sessionKey);   //session key
	std::string varNew = b.Decrypt_CBC(newkey);  //decrypts with session key
	
	std::stringstream qw;
	qw << varNew;

	{
	cereal::JSONInputArchive iarchive(qw);	
	MyClass mydata2;
	iarchive(mydata2);
	noncetwo=mydata2.nonceOne;
	} 
	
	
	

	
	//---------------------------------------------------------====
	//long fnonce = f(noncetwo);
	
	const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 65;
	long t = A * (state % Q) - R * (state / Q);
	
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	long fnonce=(long)((((double) state/M)* noncetwo)+(M/noncetwo));
	
	//print part 9
	
	std::cout << endl;
	std::cout << "Recd from ID_A:" << endl;
	std::cout << "EKs[N2]: " << newkey << endl;
	std::cout << "N2: " << noncetwo << endl;
	std::cout << "f(N2): " << fnonce << endl;
	std::cout << endl;
	
	std::string inputN;

	std::stringstream as; 
			{
				cereal::JSONOutputArchive oarchive(as);
				MyData mydata5;
				mydata5.fnoncer = fnonce;	
				
				oarchive(mydata5);
			}
			
			const char* input5 = as.str().c_str();

			inputN = b.Encrypt_CBC(input5);  //session key
			
			//-----------------------------------------------------
			//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
			
			//print part 9
			
			std::cout << endl;
			std::cout << "Send to ID_B:" << endl;
			std::cout << "EKs[f(N2)]: " << inputN << endl;
			std::cout << endl;
			
			std::stringstream zx;
				{
					cereal::JSONOutputArchive oarchive(zx);
					MyBlow myblow5;
					myblow5.encryptedString = inputN;	
					myblow5.choice = option;
					
					oarchive(myblow5);
				}
				
				std::string nexty = zx.str();
				
				
				write(sockfb, nexty.c_str(), 1000);
	
    //read input from 'B' to see if connection is secure	
				
	char secureBuffer[1000];
	
	read(sockfb, secureBuffer, 1000);
	
	string secureString(secureBuffer);
	std::stringstream secureStream;
	secureStream << secureString; 
	
	
	string secureEncrypted;
	{
	cereal::JSONInputArchive iarchive(secureStream);	
	secureConnection sc;
	iarchive(sc);
	secureEncrypted = sc.secure;
	} 
	
	string secure = b.Decrypt_CBC(secureEncrypted);
	string testString;
	
	if(option == 1) {
		
		if(secure == "yes") {
			std::cout << "Enter string S (any length): ";
			std::cin >> testString;
			
		} else {
			std::cout << "Connection not secure" << endl;
			close(sockfb);
		}
		
		std::cout << endl;	
		
		//print out hex of string
		std::cout << "S converted to hex: " << md5(testString) << endl; 
		
		std::cout << endl;	
		
		string encryptedHex = b.Encrypt_CBC(testString);
		
		//print out encrypted string that was done with Ks
		std::cout << "Encrypted (EKs[S]) : " << encryptedHex << endl; 
		
		std::cout << endl;		
		
		std::cout << "Send EKs[S] → IDB" << endl;
		
		//encrypt and serialize then send over to 'B'

		std::stringstream encryptedHexReturnToB;
		
		{
		cereal::JSONOutputArchive oarchive(encryptedHexReturnToB);
		blowfisher blow;
		blow.encryptedString = encryptedHex;	
		
		oarchive(blow);
		}
		string a = encryptedHexReturnToB.str();
		
		write(sockfb, a.c_str(), 1000);
	}else {
			
			string filename;
			if(secure == "yes") {
				std::cout << "Enter File S (any size): ";
				std::cin >> testString;
				filename = testString;
				
				
			} else {
				std::cout << "Connection not secure" << endl;
				close(sockfb);
			}
			char* fileToTransfer=const_cast<char*>(filename.c_str());
			std::cout << fileToTransfer << endl;
			
			
			
			int totalbytesSent=0;
		FILE *fp = fopen(fileToTransfer,"rb");
		if(fp==NULL){ perror("File not present or not readable"); exit(3); }
		
		bool done = true;
		while(done){
			
		  char buff[1024]={0};
		  int bytes_read = fread(buff,1,1024,fp);
		  if(bytes_read > 0){
			totalbytesSent+=bytes_read;
			if(send(sockfb, buff, bytes_read,0)<=0){
				perror("Send Failed");exit(3);
			}
		  }
		  if (bytes_read < 1024){
			  if (ferror(fp))
				  printf("Error reading\n");
			  done = false;
			}
		}
			std::cout << "Send EKs[S] → IDB" << endl;
			
			cout<<"Transfer Done: "<<totalbytesSent<<" bytes\n";
		
		
	}
	//=========================================
	
	
	close(sockfb);
	
	
	return 0;
}
