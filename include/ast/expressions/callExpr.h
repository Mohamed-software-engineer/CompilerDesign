#ifndef CALL_EXPR_H
#define CALL_EXPR_H
#pragma once

#include "../expr.h"
#include "../stmt.h"
#include <memory>
#include <string>
#include <vector>

// sum(x, y)
// args -> {x, y};

class callExpr : public Expr, public Stmt {
public:
  std::string nameoffunc;

  std::vector<std::unique_ptr<Expr>> arguments;

  callExpr(std::string nameoffunc, std::vector<std::unique_ptr<Expr>> arguments)
      : nameoffunc(nameoffunc), arguments(std::move(arguments)) {}
};

#endif
