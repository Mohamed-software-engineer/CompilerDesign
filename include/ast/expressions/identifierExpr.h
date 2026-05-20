#ifndef IDENTIFIER_EXPR_H
#define IDENTIFIER_EXPR_H

#pragma once

#include "../expr.h"
#include <string>

using namespace std;

class IdentifierExpr : public Expr {
public:
  string name;
  IdentifierExpr(string name) : name(name) {}
};
#endif
