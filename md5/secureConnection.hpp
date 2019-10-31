#ifndef SECURECONNECTION_HPP
#define SECURECONNECTION_HPP

struct secureConnection
{
  std::string secure;
 
  template<class Archive>
  void serialize(Archive & archive) {
    archive(secure);  
  }

};

#endif