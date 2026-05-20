#include "../../include/lexer/token.h"

Token::Token(tokenType type, std::string lexeme, int line) {
  this->type = type;
  this->lexeme = lexeme;
  this->line = line;
}

Token::Token() {}
tokenType Token::getType() { return type; }

std::string Token::getlexeme() { return lexeme; }
// TODO impelment
int Token::getLine() { return line; }

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << "Type: " << token.type << std::endl
     << "lexeme: " << token.lexeme << std::endl
     << "line: " << token.line << std::endl
     << "------------------------------" << std::endl;

  return os;
}
