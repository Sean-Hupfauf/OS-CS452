#ifndef BLOWFISHER_HPP
#define BLOWFISHER_HPP

struct blowfisher
{
  //PartTwo 
  std::string encryptedString;
  int choice;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(encryptedString, choice);  
  }

  
};

#endif