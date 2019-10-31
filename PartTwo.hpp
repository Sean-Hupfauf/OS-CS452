#ifndef PARTTWO_HPP
#define PARTTWO_HPP

struct PartTwo
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