#ifndef TOKEN
#define TOKEN
#include "tokenType.h"
#include <ostream>
#include <string>

class Token {
private:
  tokenType type;
  std::string lexeme;
  int line;
  std::string value;

public:
  Token();
  Token(tokenType type, std::string lexeme, int line);

  tokenType getType();
  std::string getlexeme();
  int getLine();

  friend std::ostream &operator<<(std::ostream &os, const Token &token);
};
#endif
