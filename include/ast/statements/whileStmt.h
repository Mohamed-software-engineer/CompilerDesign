#ifndef WHILE_STMT_H
#define WHILE_STMT_H
#include "../expr.h"
#include "../stmt.h"
#include <vector>
#pragma once

#include <memory>

class whilestmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::vector<std::unique_ptr<Stmt>> body;

  whilestmt(std::unique_ptr<Expr> condition)
      : condition(std::move(condition)) {};
};

#endif
