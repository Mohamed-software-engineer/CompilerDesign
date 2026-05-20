#ifndef LEXER
#define LEXER
// ------------
#include "token.h"
#include <fstream>
// #include <iostream>
#include <string>
using namespace std;
class Lexer {
private:
  istream &source;
  int position = 0;
  int line = 1;

  char peek();
  char advance();
  bool isAtEnd();

public:
  Lexer(istream &source);
  ~Lexer();
  Token nextToken();
  void skipWhitespace();
  Token identifier(char firstChar);
  Token number(char firstChar);
};
#endif
