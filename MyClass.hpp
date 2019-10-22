#ifndef MYCLASS_HPP
#define MYCLASS_HPP

struct MyClass
{
  int nonceOne;
  std::string request;

  // Этот метод позволяет cereal узнать какие члены данных сериализовать:
  template<class Archive>
  void serialize(Archive & archive) {
    archive(nonceOne, request);  // для сериализации "вещей" нужно передать их в архив.
  }

  // Функцию сериализации можно вынести из дефиниции класса или разделить
  // на две отдельные функции загрузки и сохранения.
  // Поддерживается версионирование классов, приватные методы сериализации,
  // и даже классы не поддерживающие конструкцию по умолчанию.
  // Можно сериализовать примитивные типы данных и почти каждый тип из
  // стандартной библиотеки без необходимости что-то писать самому.
};

#endif