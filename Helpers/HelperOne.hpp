#ifndef HELPERONE_HPP
#define HELPERONE_HPP

struct HelperOne
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