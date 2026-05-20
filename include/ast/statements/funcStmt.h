#ifndef FUNC_STMT_H
#define FUNC_STMT_H
#include "../stmt.h"
#include <memory>
#include <string>
#include <vector>
#pragma once

// int sum(x, y){
//
// }
class FuncStmt : public Stmt {
public:
  struct Parameter {
    std::string name;
    std::string type;
    Parameter(std::string name, std::string type) : name(name), type(type) {}
  };

  std::string returnType;

  std::string name;

  std::vector<Parameter> parameters;

  std::vector<std::unique_ptr<Stmt>> body;

  FuncStmt(std::string returnType, std::string name)
      : returnType(returnType), name(name) {}
};
#endif
