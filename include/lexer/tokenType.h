#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H
#include <string>
enum tokenType {
  // keywards
  INT,
  FLOAT,
  BOOL,
  CHAR,
  USING,
  SET,
  SETARR,
  IF,
  ELIF,
  ELSE,
  WHILE,
  FUNC,
  BREAK,
  CONTINUE,
  RETURN,

  // Identifiers & literals
  IDENTIFIER,
  NUMBER,

  // operator
  PLUS,
  MINUS,
  STAR,
  SLASH,
  ASSIGN,
  EQUAL,
  NOT_EQUAL,
  GREATER,
  LESS,
  GREATER_EQUAL,
  LESS_EQUAL,

  // Symbols
  SEMICOLON,
  COLON,
  COMA,
  LPAREN,
  RPAREN, //()
  LBRACE,
  RBRACE, // {}
  LSBRACT,
  RSBRACT, // []

  _EOF

};
std::string tokenTypeToString(tokenType type);
#endif
