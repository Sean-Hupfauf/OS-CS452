#include "blowfish.h"
#include <iostream>
#include <string.h>

int main() {
    
    std::cout << "hello";
    

    std::cout << "hereOne:";
    BLOWFISH bf("FEDCBA9876543210");
    std::string stringOne = "hello there";
    std::string stringTwo = "general kenobi";
    
    std::string var = bf.Encrypt_CBC(stringTwo);
    
    std::cout << var;

    std::string varTwo = bf.Decrypt_CBC(var);

    
    std::cout << varTwo;


    return 0;
}
