#ifndef IF_STMT_H
#define IF_STMT_H
#pragma once

#include "../expr.h"
#include "../stmt.h"

#include <memory>
#include <vector>

using namespace std;

class ifstmt : public Stmt {
public:
  // if condition
  unique_ptr<Expr> condition;

  // if body
  vector<unique_ptr<Stmt>> thenBranch;

  // else-if branches
  struct ElseIfBranch {
    unique_ptr<Expr> condition;
    vector<unique_ptr<Stmt>> body;
  };

  vector<ElseIfBranch> elseIfBranches;

  // else body
  vector<unique_ptr<Stmt>> elseBranch;

  ifstmt(unique_ptr<Expr> cond) : condition(std::move(cond)) {}
};
#endif
