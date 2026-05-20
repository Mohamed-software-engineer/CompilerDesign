#include "../../include/semantic/Analyzer.h"
#include "../../include/ast/expressions/arrayExpr.h"
#include "../../include/ast/expressions/binaryExpr.h"
#include "../../include/ast/expressions/callExpr.h"
#include "../../include/ast/expressions/identifierExpr.h"
#include "../../include/ast/expressions/numberExpr.h"
#include "../../include/ast/statements/assignStmt.h"
#include "../../include/ast/statements/funcStmt.h"
#include "../../include/ast/statements/ifStmt.h"
#include "../../include/ast/statements/varDeclStmt.h"
#include "../../include/ast/statements/whileStmt.h"

#include <iostream>

Analyzer::Analyzer() : debug(true), out(&std::cout) {
  table.setDebug(debug);
  table.setOutput(*out);
}

void Analyzer::setDebug(bool value) {
  debug = value;
  table.setDebug(value);
}

void Analyzer::setOutput(std::ostream &output) {
  out = &output;
  table.setOutput(output);
}

bool Analyzer::hasErrors() const { return !errors.empty(); }

const std::vector<std::string> &Analyzer::getErrors() const { return errors; }

void Analyzer::error(const std::string &message) {
  errors.push_back(message);
  *out << "Semantic Error:" << std::endl;
  *out << message << std::endl;
}

void Analyzer::enterScope() { table.enterScope(); }

void Analyzer::exitScope() { table.exitScopee(); }

void Analyzer::analyze(Program *program) {
  errors.clear();

  if (!program)
    return;

  declareFunctions(program);

  for (const auto &stmt : program->statements) {
    analyzeStmt(stmt.get());
  }
}

void Analyzer::declareFunctions(Program *program) {
  for (const auto &stmt : program->statements) {
    auto *func = dynamic_cast<FuncStmt *>(stmt.get());
    if (!func)
      continue;

    Symbol symbol(func->name, func->returnType, "FUNCTION");

    for (const auto &param : func->parameters) {
      symbol.parametrs.push_back(param.type);
    }

    if (!table.insert(symbol)) {
      error("Duplicate function '" + func->name + "'");
    }
  }
}

void Analyzer::analyzeBlock(const std::vector<std::unique_ptr<Stmt>> &stmts) {
  for (const auto &stmt : stmts) {
    analyzeStmt(stmt.get());
  }
}

void Analyzer::analyzeStmt(Stmt *stmt) {
  if (!stmt)
    return;

  if (auto *varDecl = dynamic_cast<VarDeclStmt *>(stmt)) {
    analyzeVarDecl(varDecl);
    return;
  }

  if (auto *assign = dynamic_cast<AssignStmt *>(stmt)) {
    analyzeAssign(assign);
    return;
  }

  if (auto *ifStmt = dynamic_cast<ifstmt *>(stmt)) {
    analyzeIf(ifStmt);
    return;
  }

  if (auto *whileStmt = dynamic_cast<whilestmt *>(stmt)) {
    analyzeWhile(whileStmt);
    return;
  }

  if (auto *func = dynamic_cast<FuncStmt *>(stmt)) {
    analyzeFunc(func);
    return;
  }

  if (auto *call = dynamic_cast<callExpr *>(stmt)) {
    analyzeCall(call);
    return;
  }
}

void Analyzer::analyzeVarDecl(VarDeclStmt *stmt) {
  Symbol symbol(stmt->name, stmt->type, "VARIABLE");

  if (!table.insert(symbol)) {
    error("Redeclaration of variable '" + stmt->name + "'");
  }

  analyzeExpr(stmt->initializer.get());
}

void Analyzer::analyzeAssign(AssignStmt *stmt) {
  Symbol *symbol = table.lookup(stmt->name);

  if (!symbol) {
    error("Undefined variable '" + stmt->name + "'");
  } else if (symbol->kind == "FUNCTION") {
    error("Cannot assign to function '" + stmt->name + "'");
  }

  analyzeExpr(stmt->value.get());
}

void Analyzer::analyzeIf(ifstmt *stmt) {
  analyzeExpr(stmt->condition.get());

  enterScope();
  analyzeBlock(stmt->thenBranch);
  exitScope();

  for (auto &branch : stmt->elseIfBranches) {
    analyzeExpr(branch.condition.get());

    enterScope();
    analyzeBlock(branch.body);
    exitScope();
  }

  if (!stmt->elseBranch.empty()) {
    enterScope();
    analyzeBlock(stmt->elseBranch);
    exitScope();
  }
}

void Analyzer::analyzeWhile(whilestmt *stmt) {
  analyzeExpr(stmt->condition.get());

  enterScope();
  analyzeBlock(stmt->body);
  exitScope();
}

void Analyzer::analyzeFunc(FuncStmt *stmt) {
  Symbol *symbol = table.lookupCurrentScope(stmt->name);

  if (!symbol) {
    Symbol funcSymbol(stmt->name, stmt->returnType, "FUNCTION");
    for (const auto &param : stmt->parameters) {
      funcSymbol.parametrs.push_back(param.type);
    }
    if (!table.insert(funcSymbol)) {
      error("Duplicate function '" + stmt->name + "'");
    }
  } else if (symbol->kind != "FUNCTION") {
    error("Duplicate function '" + stmt->name + "'");
  }

  enterScope();

  for (const auto &param : stmt->parameters) {
    Symbol paramSymbol(param.name, param.type, "PARAMETER");
    if (!table.insert(paramSymbol)) {
      error("Redeclaration of variable '" + param.name + "'");
    }
  }

  analyzeBlock(stmt->body);

  exitScope();
}

void Analyzer::analyzeCall(callExpr *expr) {
  Symbol *symbol = table.lookup(expr->nameoffunc);

  if (!symbol) {
    error("Undefined function '" + expr->nameoffunc + "'");
  } else if (symbol->kind != "FUNCTION") {
    error("'" + expr->nameoffunc + "' is not a function");
  } else if (symbol->parametrs.size() != expr->arguments.size()) {
    error("Function '" + expr->nameoffunc + "' expects " +
          std::to_string(symbol->parametrs.size()) + " arguments but got " +
          std::to_string(expr->arguments.size()));
  }

  for (const auto &arg : expr->arguments) {
    analyzeExpr(arg.get());
  }
}

void Analyzer::analyzeExpr(Expr *expr) {
  if (!expr)
    return;

  if (dynamic_cast<NumberExpr *>(expr)) {
    return;
  }

  if (auto *identifier = dynamic_cast<IdentifierExpr *>(expr)) {
    Symbol *symbol = table.lookup(identifier->name);
    if (!symbol) {
      error("Undefined identifier '" + identifier->name + "'");
    } else if (symbol->kind == "FUNCTION") {
      error("'" + identifier->name + "' is a function, not a variable");
    }
    return;
  }

  if (auto *binary = dynamic_cast<BinaryExpr *>(expr)) {
    analyzeExpr(binary->left.get());
    analyzeExpr(binary->right.get());
    return;
  }

  if (auto *array = dynamic_cast<ArrayExpr *>(expr)) {
    for (const auto &element : array->elements) {
      analyzeExpr(element.get());
    }
    return;
  }

  if (auto *call = dynamic_cast<callExpr *>(expr)) {
    analyzeCall(call);
    return;
  }
}
