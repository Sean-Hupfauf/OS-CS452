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

#define PORT 9568
#define MAXVALUE 11500
#define bytesRead 4096
using namespace std;

int main() {
	
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
	std::cout << numRotations << endl;
	int convRot = htonl(numRotations);
    while (inFile.peek() != EOF && z >=0) {
		if(z > bytesRead) {
		inFile.read(x, bytesRead);
		z-= bytesRead;
		string str(x);
        string encryptedString = bf.Encrypt_CBC(str);
        std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
		
		
        string decryptedString = bf.Decrypt_CBC(encryptedString);
		strcpy(y, decryptedString.c_str());
		outFile.write(y,bytesRead);
		memset(x, 0, bytesRead);
		memset(y, 0, bytesRead);
		}
		else {
		inFile.read(x, z);
		string str(x);
        string encryptedString = bf.Encrypt_CBC(str);
        std::cout << encryptedString << endl << "... " << encryptedString.length() << endl;
        /*string decryptedString = bf.Decrypt_CBC(encryptedString);
		strcpy(y, decryptedString.c_str());
		outFile.write(x,z);
        //std::cout << str << endl << "... " << str.length() << endl;*/
		z-= bytesRead;
		}
}
}