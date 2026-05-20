#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "scope.h"
#include "symbol.h"
#pragma once

#include <memory>
#include <ostream>
#include <string>

class symbolTable {
private:
  Scope *current;
  bool debug;
  std::ostream *out;

public:
  symbolTable();
  ~symbolTable();

  void setDebug(bool value);
  void setOutput(std::ostream &output);

  void enterScope();
  void exitScopee();

  bool insert(const Symbol &symbol);

  Symbol *lookup(const std::string &name);
  Symbol *lookupCurrentScope(const std::string &name);

  Scope *getCurrentScope();
};
#endif
