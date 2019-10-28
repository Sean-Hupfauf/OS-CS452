#ifndef MYNUMBERS_HPP
#define MYNUMBERS_HPP


struct MyNumbers
{
  int x;
  std::string str;
  

  // Этот метод позволяет cereal узнать какие члены данных сериализовать:
  template<class Archive>
  void serialize(Archive & archive) {
    archive(x, str);  // для сериализации "вещей" нужно передать их в архив.
  } 
  
 };
  
  
#endif