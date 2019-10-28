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
#include "md5.hpp"
#include "secureConnection.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"

#define PORT 9518
#define MAXVALUE 11500

typedef MyClass MyData;
typedef PartTwo MyTwo;
typedef PartTwoB MyTwoB;
typedef blowfisher MyBlow;

using namespace std;

long nonceTwo;
std::string bKey;
std::string sessionKey;


void error(const char *msg) {
	perror(msg);
	exit(1);
}

long f(long nonce) {
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
	return (long)((((double) state/M)* nonce)+(M/nonce));
}


//-----------------------------------------------------

int main (int argc, char *argv[]) {
	


    std::cout << "----Client 'B' Setup----" << endl;
	
	std::cout << "Enter a nonce: ";
	std::cin >> nonceTwo;
	
	std::cout << "Enter client private key for 'B': ";
	std::cin >> bKey;
	
	
	
	
	

    char buffer[256];
	/*
	============================
	SET UP CONNECTION
	============================
	*/

	int sockfb, newsockfb;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	// create a socket
	// socket(int domain, int type, int protocol)
	sockfb = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfb < 0) error("ERROR opening socket");

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
	if (bind(sockfb, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}

	// This listen() call tells the socket to listen to the incoming connections
	// The listen() function places all incoming connection into a backlog queue
	// until accept() call accepts the connection
	// max backlog queue is 5
	listen(sockfb, 5);

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
	
	newsockfb = accept(sockfb, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfb < 0) {
		error("ERROR on accept");
	}
	

	
			/* //char buffer[256];
			n = read(newsockfb,buffer,255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s\n",buffer); */
			
			std::string keyers;
			BLOWFISHONE bf(bKey); //b key
			

			std::stringstream sr; 
			char buf[1000];
			
			read(newsockfb, buf, 1000);
			
			string str(buf);
			//std::cout << str << std::endl;
			sr << str;
			
			{
			cereal::JSONInputArchive iarchive(sr);	
			MyBlow myblow;
			iarchive(myblow);
			keyers=myblow.encryptedString;
			} 
			
			//-----------------------------------------------------
			//Decypted and exposed the Ks, Request, Nonce, and encryptedString for
			//B's encrypted data containing Ks and IDa
			
			std::string varThree = bf.Decrypt_CBC(keyers);  //decrypt with 'b' key
			
			std::stringstream sq;
			sq << varThree;
			
			//-----------------------------------------------------
			//Decypted and exposed B's encrypted data containing Ks and IDa
			//Now it gets deserilized below
			
			{
				
			cereal::JSONInputArchive iarchive(sq);	
			MyTwoB mytwoB;
			iarchive(mytwoB);
			sessionKey = mytwoB.sessionKey;
			std::cout << mytwoB.IDa << std::endl << mytwoB.sessionKey << std::endl;;
			
			} 
			
			
			
			std::string inputM;
        
			//-----------------------------------------------------
	
			std::stringstream ss; 
			{
				cereal::JSONOutputArchive oarchive(ss);
				MyData mydata;
				mydata.nonceOne = nonceTwo;	
				
				oarchive(mydata);
			}
			
			const char* input4 = ss.str().c_str();
			
            BLOWFISHTWO b(sessionKey);  //session key
			inputM = b.Encrypt_CBC(input4);  //encrypt with sessionKey
			
			//-----------------------------------------------------
			//Then it seralizes the complete encrypted payload that is going to A and writes it to A.
			
			std::stringstream we;
				{
					cereal::JSONOutputArchive oarchive(we);
					MyBlow myblow;
					myblow.encryptedString = inputM;	
					
					oarchive(myblow);
				}
				
				std::string nextx = we.str();
				
				
				write(newsockfb, nextx.c_str(), 1000);
			
			//-----------------------------------------------------
			//Part 5 gets the serialized encrypted nonce and deserilize it
			
			std::stringstream ty; 
			std::string newkey;
			char bufer[1000];
			
			read(newsockfb, bufer, 1000);
			
			string stri(bufer);
			
			ty << stri;
			
			{
			cereal::JSONInputArchive iarchive(ty);	
			MyBlow myblow5;
			iarchive(myblow5);
			newkey=myblow5.encryptedString;
			//std::cout << newkey << std::endl;
			} 
			
			//-----------------------------------------------------
			//Decrypt it and deserilize it again to expose the nonce
			
			int fnonce5;
			
			
			std::string varNew = b.Decrypt_CBC(newkey);  //decrypt using session key
			
			std::stringstream vb;
			vb << varNew;
			//std::cout << varNew << std::endl;
			{
			cereal::JSONInputArchive iarchive(vb);	
			MyClass mydata5;
			iarchive(mydata5);
			fnonce5=mydata5.fnoncer;
			} 
			std::cout << fnonce5 << std::endl;
			
			nonceTwo = f(nonceTwo);
			std::cout << nonceTwo << std::endl;
			
			
			//the good stuff happens: begin transfer
			
			
			string secure = "no";
			if(nonceTwo ==  fnonce5) { //if nonceTwo equals fnonce5 then there is a secure connection
			    secure = "yes";				
			} 
			
			string encryptedSecure = b.Encrypt_CBC(secure);  //encrypt with sessionKey
			
			
			std::stringstream returnSecureMessage;
			{
			cereal::JSONOutputArchive oarchive(returnSecureMessage);	
			secureConnection sc;
			sc.secure = encryptedSecure;
			oarchive(sc);
			} 
			
			//send over encrypted message saying that connection is secure
			write(newsockfb, returnSecureMessage.str().c_str(), 1000); 
			
			
			
			
			
			
			
			//read in encrypted string
			std::stringstream encryptedTest; 
			
			char encryptedTestStringBuffer[1000];
			
			read(newsockfb, encryptedTestStringBuffer, 1000);
			
			string encryptedTestString(encryptedTestStringBuffer);
			std::cout << endl << encryptedTestString  << endl;
			encryptedTest << encryptedTestString;
			
			
			//deserialize encrypted string
			string encrypted;
			{
			cereal::JSONInputArchive iarchive(encryptedTest);	
			MyBlow blower;
			iarchive(blower);
			encrypted = blower.encryptedString;
			} 
			
			
			
			//decrypt 
			string decryptedTestString = b.Decrypt_CBC(encrypted); //decrypt with session key
			
			std::cout << "Decrypted Message converted to hex: " << md5(decryptedTestString) << endl;
			
			std::cout << "Print S: " << decryptedTestString << endl; 
			
			
	close(newsockfb);
	close(sockfb);
	return 0;
		
	
}
