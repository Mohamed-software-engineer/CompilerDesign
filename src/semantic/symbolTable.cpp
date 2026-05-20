#include "../../include/semantic/symbolTable.h"
#include <iostream>

symbolTable::symbolTable() : debug(false), out(&std::cout) {
  current = new Scope(nullptr, 0);
}

symbolTable::~symbolTable() {
  while (current) {
    Scope *temp = current;
    current = current->parant;
    delete temp;
  }
}

void symbolTable::enterScope() {
  if (debug)
    *out << "ENTER SCOPE" << std::endl;
  current = new Scope(current, current->level + 1);
}

void symbolTable::exitScopee() {
  if (!current)
    return;
  if (debug)
    *out << "EXIT SCOPE" << std::endl;
  Scope *temp = current;
  current = current->parant;
  delete temp;
}

void symbolTable::setDebug(bool value) { debug = value; }

void symbolTable::setOutput(std::ostream &output) { out = &output; }

bool symbolTable::insert(const Symbol &symbol) {
  if (current->symbols.count(symbol.name)) {
    return false;
  }

  current->symbols[symbol.name] = symbol;
  if (debug) {
    *out << "INSERT SYMBOL:" << std::endl;
    *out << "name=" << symbol.name << " type=" << symbol.type
         << " kind=" << symbol.kind << std::endl;
  }
  return true;
}

Symbol *symbolTable::lookup(const std::string &name) {
  Scope *scope = current;
  while (scope) {
    auto it = scope->symbols.find(name);
    if (it != scope->symbols.end()) {
      if (debug)
        *out << "LOOKUP:" << std::endl << name << " -> found" << std::endl;
      return &it->second;
    }
    scope = scope->parant;
  }
  if (debug)
    *out << "LOOKUP:" << std::endl << name << " -> not found" << std::endl;
  return nullptr;
}

Symbol *symbolTable::lookupCurrentScope(const std::string &name) {
  auto it = current->symbols.find(name);
  if (it != current->symbols.end()) {
    return &it->second;
  }
  return nullptr;
}

Scope *symbolTable::getCurrentScope() { return current; }
