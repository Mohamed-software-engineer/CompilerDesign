#ifndef PARSER
#define PARSER

#include "../ast/expr.h"
#include "../ast/expressions/arrayExpr.h"
#include "../ast/expressions/binaryExpr.h"
#include "../ast/expressions/callExpr.h"
#include "../ast/expressions/identifierExpr.h"
#include "../ast/expressions/numberExpr.h"
#include "../ast/statements/assignStmt.h"
#include "../ast/statements/funcStmt.h"
#include "../ast/statements/ifStmt.h"
#include "../ast/statements/program.h"
#include "../ast/statements/varDeclStmt.h"
#include "../ast/statements/whileStmt.h"
#include "../ast/stmt.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Parser {
private:
  Lexer lexer;
  Token current;

  // set int id = expr (;)
  // (=, ;)
  void advance();
  bool check(tokenType type);
  bool match(tokenType type);
  void expect(tokenType type);
  void error(string message);
  std::vector<std::unique_ptr<Stmt>> block();
  std::unique_ptr<Expr> condition();
  std::vector<std::unique_ptr<Expr>> call_args();

public:
  Parser(Lexer lexer);

  std::unique_ptr<Stmt> statment();
  std::unique_ptr<Stmt> declare();
  std::unique_ptr<Expr> initial();
  std::unique_ptr<Stmt> assign(std::string name);
  std::unique_ptr<Stmt> if_stmt();
  void else_if(ifstmt *stmt);
  void else_stmt(ifstmt *stmt);
  std::unique_ptr<Stmt> while_stmt();
  std::unique_ptr<Stmt> call_func(std::string name);
  std::unique_ptr<Stmt> func();

  std::unique_ptr<Expr> parseExpression();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comaprsion();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();

  std::unique_ptr<Program> parse();
};
#endif // Parser
