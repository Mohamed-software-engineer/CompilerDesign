#ifndef ARRAY_EXPR_H
#define ARRAY_EXPR_H

#pragma once

#include "../expr.h"

#include <memory>
#include <vector>

using namespace std;

//{442, 8/3, 9029};
class ArrayExpr : public Expr {
public:
  vector<unique_ptr<Expr>> elements;
  ArrayExpr(vector<unique_ptr<Expr>> elements)
      : elements(std::move(elements)) {}
};

#endif
