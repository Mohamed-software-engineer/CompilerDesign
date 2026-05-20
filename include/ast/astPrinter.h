#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#pragma once

#include "expr.h"
#include "expressions/arrayExpr.h"
#include "expressions/binaryExpr.h"
#include "expressions/callExpr.h"
#include "expressions/identifierExpr.h"
#include "expressions/numberExpr.h"
#include "statements/assignStmt.h"
#include "statements/funcStmt.h"
#include "statements/ifStmt.h"
#include "statements/program.h"
#include "statements/varDeclStmt.h"
#include "statements/whileStmt.h"
#include "stmt.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

class ASTPrinter {
public:
  explicit ASTPrinter(std::ostream &out);
  ASTPrinter();

  void print(const Program &program);

private:
  std::ostream &out;

  void printStmt(const Stmt *stmt, int indent);
  void printExpr(const Expr *expr, int indent);
  void printBlock(const std::vector<std::unique_ptr<Stmt>> &stmts, int indent);
  void writeIndent(int indent);

  std::string exprToString(const Expr *expr);
  std::string opToString(const std::string &op);
};

#endif
