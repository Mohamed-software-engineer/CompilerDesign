#ifndef SYMBOL_H
#define SYMBOL_H
#pragma once
#include <string>
#include <vector>

enum class SymbolType { VARIABLE, FUNCTION, PARAMETER, ARRAY };

struct Symbol {
  std::string name;
  std::string type;
  std::string kind;

  std::vector<std::string> parametrs;

  Symbol() = default;

  Symbol(std::string name, std::string type, std::string kind)
      : name(name), type(type), kind(kind) {};
};

#endif
