#include "../../include/parser/parser.h"
#include "../../include/lexer/tokenType.h"
#include <algorithm>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

Parser::Parser(Lexer lexer) : lexer(lexer), current(lexer.nextToken()) {};

void Parser::advance() { current = lexer.nextToken(); }

bool Parser::check(tokenType type) { return current.getType() == type; }

bool Parser::match(tokenType type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}
void Parser::expect(tokenType type) {
  if (!match(type)) {
    std::string megError =
        std::format("Expected {} but found {}", tokenTypeToString(type),
                    tokenTypeToString(current.getType()));
    error(megError);
  }
}
void Parser::error(string message) {
  std::string error =
      std::format("Parser Error at line {} : {}", current.getLine(), message);
  throw std::runtime_error(error);
}

// 5 == 6 != 7 == 8
//
// term == term
std::unique_ptr<Expr> Parser::parseExpression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {

  // string op;
  // std::unique_ptr<Expr> left, right;
  // BinaryExpr bin(op, std::move(left), std::move(right));
  auto left = comaprsion();
  // left = comaprsion();

  while (check(tokenType::NOT_EQUAL) || check(tokenType::EQUAL)) {

    string op = tokenTypeToString(current.getType());

    advance();

    auto right = comaprsion();

    left = make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    // BinaryExpr bin(op, std::move(left), std::move(right));
  }
  return left;
}

std::unique_ptr<Expr> Parser::comaprsion() {

  auto left = term();

  while (check(tokenType::GREATER) || check(tokenType::LESS) ||
         check(tokenType::GREATER_EQUAL) || check(tokenType::LESS_EQUAL)) {

    string op = tokenTypeToString(current.getType());

    advance();

    auto right = term();

    left = make_unique<BinaryExpr>(op, std::move(left), std::move(right));
  }

  return left;
}

std::unique_ptr<Expr> Parser::term() {

  auto left = factor();

  while (check(tokenType::PLUS) || check(tokenType::MINUS)) {

    string op = tokenTypeToString(current.getType());

    advance();

    auto right = factor();

    left = make_unique<BinaryExpr>(op, std::move(left), std::move(right));
  }

  return left;
}

std::unique_ptr<Expr> Parser::factor() {
  auto left = unary();
  while (check(tokenType::STAR) || check(tokenType::SLASH)) {
    string op = tokenTypeToString(current.getType());
    advance();

    auto right = unary();

    left = make_unique<BinaryExpr>(op, std::move(left), std::move(right));
  }

  return left;
}

// set int x = (3 * 5);

std::unique_ptr<Expr> Parser::unary() {
  if (check(tokenType::NUMBER)) {

    int value = stoi(current.getlexeme());

    advance();

    return make_unique<NumberExpr>(value);
  }

  else if (check(tokenType::IDENTIFIER)) {

    string name = current.getlexeme();

    advance();

    if (check(tokenType::LPAREN)) {
      auto args = call_args();
      return make_unique<callExpr>(name, std::move(args));
    }

    return make_unique<IdentifierExpr>(name);
  }

  else if (match(tokenType::LPAREN)) {

    auto expr = parseExpression();

    expect(tokenType::RPAREN);

    return expr;

  } else if (match(tokenType::LBRACE)) {
    std::vector<std::unique_ptr<Expr>> elements;

    while (!check(tokenType::RBRACE)) {
      elements.push_back(parseExpression());

      if (check(tokenType::COMA))
        advance();
    }

    expect(tokenType::RBRACE);
    return make_unique<ArrayExpr>(std::move(elements));

  } else {
    std::string megError = std::format("Unexpected token: {}",
                                       tokenTypeToString(current.getType()));

    error(megError);
  }
  return nullptr;
}

std::unique_ptr<Stmt> Parser::statment() {

  if (check(tokenType::SET) || check(tokenType::SETARR)) {
    return declare();
  } else if (check(tokenType::IF)) {
    return if_stmt();
  } else if (check(tokenType::WHILE)) {
    return while_stmt();
  } else if (check(tokenType::FUNC)) {
    return func();
  } else if (check(tokenType::IDENTIFIER)) {
    string name = current.getlexeme();
    advance();
    if (check(tokenType::ASSIGN)) {
      return assign(name);
    } else if (check(tokenType::LPAREN)) {
      // return function call node
      return call_func(name);
    }
  } else {
    std::string megerror = std::format("unexpected token: {}",
                                       tokenTypeToString(current.getType()));
    error(megerror);
  }
  return nullptr;
}

std::unique_ptr<Stmt> Parser::declare() {
  tokenType tmp_token = current.getType();
  advance();

  if (check(tokenType::INT) || check(tokenType::FLOAT) ||
      check(tokenType::CHAR) || check(tokenType::BOOL)) {

    string type = tokenTypeToString(current.getType());
    advance();
    string name = current.getlexeme();
    expect(tokenType::IDENTIFIER);

    std::unique_ptr<Expr> init = nullptr;

    if (check(tokenType::ASSIGN) && tmp_token == SET) {
      advance();
      init = parseExpression();
    } else if (check(tokenType::LSBRACT) && tmp_token == SETARR) {
      advance();

      if (check(tokenType::IDENTIFIER) || check(tokenType::NUMBER)) {
        advance();
        expect(tokenType::RSBRACT);
      }
      if (check(tokenType::ASSIGN)) {
        advance();
        init = parseExpression();
      }
    }

    expect(tokenType::SEMICOLON);
    return make_unique<VarDeclStmt>(type, name, std::move(init));
  }

  std::string megError =
      std::format("Unexpected token: {}", tokenTypeToString(current.getType()));

  error(megError);
  return nullptr;
}
//
// std::unique_ptr<Expr> Parser::initial() {
//   advance();
//   if (check(tokenType::LBRACE)) {
//     advance();
//
//     std::vector<std::unique_ptr<Expr>> elements;
//
//     while (!check(tokenType::RBRACE)) {
//
//       elements.push_back(parseExpression());
//
//       if (check(tokenType::COMA))
//         advance();
//     }
//
//     advance();
//     return make_unique<ArrayExpr>(std::move(elements));
//   } else {
//     auto expr = parseExpression();
//     expect(tokenType::SEMICOLON);
//
//     return expr;
//   }
// }
//
std::unique_ptr<Stmt> Parser::assign(string name) {
  advance();
  if (check(tokenType::LBRACE)) {
    advance();
    std::vector<std::unique_ptr<Expr>> elements;
    while (check(tokenType::NUMBER) || check(tokenType::IDENTIFIER)) {
      elements.push_back(parseExpression());
      if (check(tokenType::COMA))
        advance();
    }
    expect(tokenType::RBRACE);
    expect(tokenType::SEMICOLON);
    return make_unique<AssignStmt>(name,
                                   make_unique<ArrayExpr>(std::move(elements)));
  } else {
    auto value = parseExpression();
    expect(tokenType::SEMICOLON);
    return make_unique<AssignStmt>(name, std::move(value));
  }
}

// if (5>6){
//
// }else if(){
//
// }

std::unique_ptr<Expr> Parser::condition() {
  expect(tokenType::LPAREN);
  auto expr = parseExpression();
  expect(tokenType::RPAREN);
  return expr;
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
  expect(tokenType::LBRACE);

  std::vector<std::unique_ptr<Stmt>> stmts;

  while (!check(tokenType::RBRACE)) {

    stmts.push_back(statment());
  }

  expect(tokenType::RBRACE);

  return stmts;
}

std::unique_ptr<Stmt> Parser::if_stmt() {
  advance(); // consume IF
  auto cond = condition();
  auto stmt = make_unique<ifstmt>(std::move(cond));
  stmt->thenBranch = block();
  else_if(stmt.get());
  else_stmt(stmt.get());

  return stmt;
}

void Parser::else_if(ifstmt *stmt) {

  while (check(tokenType::ELIF)) {

    advance(); // consume elif

    auto cond = condition();

    auto body = block();

    stmt->elseIfBranches.push_back({std::move(cond), std::move(body)});
  }
}

void Parser::else_stmt(ifstmt *stmt) {
  if (check(tokenType::ELSE)) {
    advance(); // consume else
    stmt->elseBranch = block();
  }
}

std::unique_ptr<Stmt> Parser::while_stmt() {

  advance();

  auto cond = condition();

  auto stmt = make_unique<whilestmt>(std::move(cond));

  stmt->body = block();

  return stmt;
}

std::unique_ptr<Stmt> Parser::func() {

  advance();

  string returnType;

  if (check(tokenType::INT) || check(tokenType::FLOAT) ||
      check(tokenType::CHAR) || check(tokenType::BOOL)) {
    returnType = tokenTypeToString(current.getType());
    advance();
  }

  string name = current.getlexeme();
  expect(tokenType::IDENTIFIER);

  expect(tokenType::LPAREN);

  auto func = make_unique<FuncStmt>(returnType, name);

  // parameters
  if (!check(tokenType::RPAREN)) {

    do {
      string paramName = current.getlexeme();
      expect(tokenType::IDENTIFIER);

      expect(tokenType::COLON);

      string paramType;

      if (check(tokenType::INT) || check(tokenType::FLOAT) ||
          check(tokenType::CHAR) || check(tokenType::BOOL)) {

        paramType = current.getlexeme();
        advance();
      }

      func->parameters.push_back({paramName, paramType});

    } while (match(tokenType::COMA));
  }

  expect(tokenType::RPAREN);

  func->body = block();

  return func;
}

// namoffunc(5*2, x, sum(x));
std::vector<std::unique_ptr<Expr>> Parser::call_args() {
  expect(tokenType::LPAREN);

  vector<unique_ptr<Expr>> args;

  if (!check(tokenType::RPAREN)) {
    args.push_back(parseExpression());

    while (match(tokenType::COMA)) {
      args.push_back(parseExpression());
    }
  }

  expect(tokenType::RPAREN);

  return args;
}

std::unique_ptr<Stmt> Parser::call_func(string name) {

  auto args = call_args();

  expect(tokenType::SEMICOLON);

  return make_unique<callExpr>(name, std::move(args));
}

unique_ptr<Program> Parser::parse() {

  auto program = make_unique<Program>();

  while (!check(tokenType::_EOF)) {
    program->statements.push_back(statment());
  }

  return program;
}
