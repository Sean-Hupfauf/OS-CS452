#ifndef PARTTWOB_HPP
#define PARTTWOB_HPP

struct PartTwoB
{
  std::string IDa;
  std::string sessionKey;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(IDa, sessionKey);  
  }

  
};

#endif