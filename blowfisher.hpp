#ifndef BLOWFISHER_HPP
#define BLOWFISHER_HPP

struct blowfisher
{
  //PartTwo 
  std::string encryptedString;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(encryptedString);  
  }

  
};

#endif