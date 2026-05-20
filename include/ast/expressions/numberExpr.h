#ifndef NUMBER_EXPR_H
#define NUMBER_EXPR_H
#pragma once

#include "../expr.h"

// 10
class NumberExpr : public Expr {
public:
  int value;
  NumberExpr(int value) : value(value) {}
};
#endif
