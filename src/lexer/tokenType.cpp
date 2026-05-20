#include "../../include/lexer/tokenType.h"
std::string tokenTypeToString(tokenType type) {
  switch (type) {

  // keywords
  case INT:
    return "INT";
  case FLOAT:
    return "FLOAT";
  case BOOL:
    return "BOOL";
  case CHAR:
    return "CHAR";
  case USING:
    return "USING";
  case SET:
    return "SET";
  case SETARR:
    return "SETARR";
  case IF:
    return "IF";
  case ELIF:
    return "ELIF";
  case ELSE:
    return "ELSE";
  case WHILE:
    return "WHILE";
  case FUNC:
    return "FUNC";
  case BREAK:
    return "BREAK";
  case CONTINUE:
    return "CONTINUE";
  case RETURN:
    return "RETURN";

  // identifiers & literals
  case IDENTIFIER:
    return "IDENTIFIER";
  case NUMBER:
    return "NUMBER";

  // operators
  case PLUS:
    return "PLUS";
  case MINUS:
    return "MINUS";
  case STAR:
    return "STAR";
  case SLASH:
    return "SLASH";
  case ASSIGN:
    return "ASSIGN";
  case EQUAL:
    return "EQUAL";
  case NOT_EQUAL:
    return "NOT_EQUAL";
  case GREATER:
    return "GREATER";
  case LESS:
    return "LESS";
  case GREATER_EQUAL:
    return "GREATER_EQUAL";
  case LESS_EQUAL:
    return "LESS_EQUAL";

  // symbols
  case SEMICOLON:
    return "SEMICOLON";
  case COLON:
    return "COLON";
  case COMA:
    return "COMA";
  case LPAREN:
    return "LPAREN";
  case RPAREN:
    return "RPAREN";
  case LBRACE:
    return "LBRACE";
  case RBRACE:
    return "RBRACE";
  case LSBRACT:
    return "LSBRACT";
  case RSBRACT:
    return "RSBRACT";

  case _EOF:
    return "EOF";

  default:
    return "UNKNOWN_TOKEN";
  }
}
