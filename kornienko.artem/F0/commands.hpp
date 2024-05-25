#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>

#include "dictionary.hpp"

namespace kornienko
{
  using mapDict = std::map< std::string, Dictionary >;
  void createDictionary(std::istream & in, std::ostream & out, mapDict & dictionaries);
  void deleteDictionary(std::istream & in, std::ostream & out, mapDict & dictionaries);
  void addWord(std::istream & in, std::ostream & out, mapDict & dictionaries);
}

#endif