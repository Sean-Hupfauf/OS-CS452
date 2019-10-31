#ifndef HELPERTHREE_HPP
#define HELPERTHREE_HPP

struct HelperThree
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