#ifndef MYCLASS_HPP
#define MYCLASS_HPP

struct MyClass
{
  int nonceOne;
  std::string request;
  int fnoncer;
  
  template<class Archive>
  void serialize(Archive & archive) {
    archive(nonceOne, request, fnoncer);  
  }


};

#endif