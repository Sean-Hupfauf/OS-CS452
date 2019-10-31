#ifndef HELPERFOUR_HPP
#define HELPERFOUR_HPP

struct HelperFour
{
  std::string IDa;
  std::string sessionKey;
  //std::string encryptedString;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(IDa, sessionKey);  
  }

  
};

#endif