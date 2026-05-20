#ifndef BINARY_EXPR_H
#define BINARY_EXPR_H
#pragma once
#pragma once

#include "../expr.h"
#include <memory>
#include <string>

using namespace std;

class BinaryExpr : public Expr {
public:
  string op;

  unique_ptr<Expr> left;
  unique_ptr<Expr> right;

  BinaryExpr(string op, unique_ptr<Expr> left, unique_ptr<Expr> right)
      : op(op), left(std::move(left)), right(std::move(right)) {}
};
#endif
