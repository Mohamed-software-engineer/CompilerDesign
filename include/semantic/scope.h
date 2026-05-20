#ifndef SOCPE_H
#define SOCPE_H
#include "symbol.h"
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

/*
 * int x = 10;
 * while () {
   if(){
 * int y = 20;
 }
 *  * }*/

class Scope {
public:
  std::unordered_map<std::string, Symbol> symbols;
  Scope *parant;

  int level;

  Scope(Scope *parant = nullptr, int level = 0)
      : parant(parant), level(level) {}
};
#endif
