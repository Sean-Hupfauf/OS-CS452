#ifndef PARTTWOB_HPP
#define PARTTWOB_HPP

struct PartTwoB
{
  int IDa;
  std::string sessionKey;
  //std::string encryptedString;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(IDa, sessionKey);  
  }

  
};

#endif