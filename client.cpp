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

#define PORT 9434
#define MAXVALUE 11500

typedef MyClass MyData;
using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(0);
}

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
	
        //Nonce 
		long nonceR = 5647892341;
		const int MAX = 20;
		  long nonce = 5647892341;
		  for  (int i = 1; i <= MAX; i++) {

				nonceR = f(nonce);
		  } 
		printf("Here is the number: %ld\n",nonceR);
        
		
		
		
		
		
		
		
			//Serialize variables
		
		/*
		std::stringstream ss; 
		
		
		printf("here1");
		{
		cereal::BinaryOutputArchive oarchive(ss);  // создание выходного архива.

		MyClass m1 = { 15, 6, 2019 };
		MyClass m2, m3;
		m2.x = 16;
		m2.y = 6;
		m2.z = 2019;
		m3 = { 17, 6, 2019 };
		printf("here2");

		oarchive(m1, m2, m3);  // запись данных в архив.
		}  // архив выходит из скопа - гарантируется flush всего контента.

			printf("here3");

		
	  // Архивы спроектированы по идиоме RAII и flush контента гарантируется
	  // только при их деструкции. Некоторые архивы могут безопасно завершить
	  // flush только при их деструкции. Нужно убедиться, особенно для выходной
	  // сериализации, что архив автоматически уничтожается после завершения
	  // работы с ним.

	  {
		  
		printf("here4");

		cereal::BinaryInputArchive iarchive(ss);  // создание входного архива.
			printf("here5");

		
		iarchive(m1, m2, m3);  // чтение данных из архива.

		std::cout << "m1 { x = " << m1.x << ", y = " << m1.y << ", z = " << m1.z << " }" << std::endl;
		std::cout << "m2 { x = " << m2.x << ", y = " << m2.y << ", z = " << m2.z << " }" << std::endl;
		std::cout << "m3 { x = " << m3.x << ", y = " << m3.y << ", z = " << m3.z << " }" << std::endl;

		
	  }
		
			printf("here6");
		
		
		
		*/
		std::string stringOne = "hello there";
		std::fstream ss; 
		ss.open("file.txt", std::ios::binary | ios::out);
        {
		cereal::BinaryOutputArchive oarchive(ss);  // создание выходного архива.

		MyData m1 = {nonceR};
		
	

		oarchive(m1);  // запись данных в архив.
		
		}
		/*size_t sii = 0;
		ss >> sii;
			write(sockfd, ss, sii);
*/

		
		
		
		
		
		
        size_t len = 0;
              
       
        BLOWFISH bf("FEDCBA9876543210");

        string encryptedString = bf.Encrypt_CBC("hey");
        
    
        std::istringstream sstream(encryptedString);
        


        const void * a = encryptedString.c_str(); 

			size_t si = 0;
			sstream >> si;
			
			
			
			
			
			
			
			
			
				
			write(sockfd, a, si);
				
				n = read(sockfd,buffer,255);
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n",buffer);


	close(sockfd);
	return 0;
}
