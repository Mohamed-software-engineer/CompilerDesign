#ifndef ANALYZER_H
#define ANALYZER_H
#include "../ast/expr.h"
#include "../ast/statements/program.h"
#include "../ast/stmt.h"
#include "symbolTable.h"
#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class VarDeclStmt;
class AssignStmt;
class ifstmt;
class whilestmt;
class FuncStmt;
class callExpr;

class Analyzer {
private:
  symbolTable table;
  std::vector<std::string> errors;
  bool debug;
  std::ostream *out;

  void error(const std::string &message);
  void declareFunctions(Program *program);
  void analyzeStmt(Stmt *stmt);
  void analyzeBlock(const std::vector<std::unique_ptr<Stmt>> &stmts);
  void analyzeExpr(Expr *expr);
  void analyzeVarDecl(VarDeclStmt *stmt);
  void analyzeAssign(AssignStmt *stmt);
  void analyzeIf(ifstmt *stmt);
  void analyzeWhile(whilestmt *stmt);
  void analyzeFunc(FuncStmt *stmt);
  void analyzeCall(callExpr *expr);
  void enterScope();
  void exitScope();

public:
  Analyzer();

  void setDebug(bool value);
  void setOutput(std::ostream &output);
  bool hasErrors() const;
  const std::vector<std::string> &getErrors() const;
  void analyze(Program *pragma);
};
#endif
