#ifndef VAR_DECL_STMT_H
#define VAR_DECL_STMT_H

#pragma once

#include "../expr.h"
#include "../stmt.h"

#include <memory>
#include <string>

// set int x;

class VarDeclStmt : public Stmt {
public:
  std::string type;
  std::string name;

  std::unique_ptr<Expr> initializer;

  VarDeclStmt(std::string type, std::string name,
              std::unique_ptr<Expr> initializer)
      : type(type), name(name), initializer(std::move(initializer)) {}
};
#endif
