#ifndef ASSIGN_STMT_H
#define ASSIGN_STMT_H

#pragma once

#include "../expr.h"
#include "../stmt.h"
#include <memory>
#include <string>

// x = value;
class AssignStmt : public Stmt {
public:
  std::string name;
  std::unique_ptr<Expr> value;

  AssignStmt(std::string name, std::unique_ptr<Expr> value)
      : name(std::move(name)), value(std::move(value)) {}
};

#endif
