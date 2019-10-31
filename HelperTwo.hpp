#ifndef HELPERTWO_HPP
#define HELPERTWO_HPP

struct HelperTwo
{
  int nonceOne;
  std::string request;
  std::string sessionKey;
  std::string encryptedString;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(nonceOne, request, sessionKey, encryptedString);  
  }

  
};

#endif