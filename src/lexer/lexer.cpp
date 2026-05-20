#include "../../include/lexer/lexer.h"
#include <cctype>
// #include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

Lexer::Lexer(istream &source) : source(source) {};

char Lexer::peek() {

  if (source.eof())
    return '\0';

  return source.peek();
}

char Lexer::advance() {
  char c;
  source.get(c);
  return c;
}

bool Lexer::isAtEnd() { return source.eof(); }

void Lexer::skipWhitespace() {
  while (!isAtEnd()) {
    char c = peek();
    if (c == ' ' || c == '\r' || c == '\t') {
      advance();
    } else if (c == '\n') {
      line++;
      advance();
    } else {
      break;
    }
  }
}

// int x = 10;
Token Lexer::nextToken() {
  skipWhitespace();

  if (isAtEnd())
    return Token(tokenType::_EOF, "", line); // Fix 1: Scope operator

  char current = advance();

  if (isalpha(current))
    return identifier(current);
  if (isdigit(current))
    return number(current);

  switch (current) {
  case '+':
    return Token(tokenType::PLUS, "+", line);
  case '-':
    return Token(tokenType::MINUS, "-", line);
  case '/':
    return Token(tokenType::SLASH, "/", line);
  case ';':
    return Token(tokenType::SEMICOLON, ";", line);
  case '(':
    return Token(tokenType::LPAREN, "(", line);
  case ')':
    return Token(tokenType::RPAREN, ")", line);
  case '{':
    return Token(tokenType::LBRACE, "{", line); // Fix 2: Cleaned syntax
  case '}':
    return Token(tokenType::RBRACE, "}", line);
  case '*':
    return Token(tokenType::STAR, "*", line);
  case '[':
    return Token(tokenType::LSBRACT, "[", line);
  case ']':
    return Token(tokenType::RSBRACT, "]", line);
  case ',':
    return Token(tokenType::COMA, ",", line);
  case ':':
    return Token(tokenType::COLON, ":", line);

  case '=':
    if (peek() == '=') {
      advance();
      return Token(tokenType::EQUAL, "==", line);
    }
    return Token(tokenType::ASSIGN, "=", line);

  case '!':
    if (peek() == '=') {
      advance();
      return Token(tokenType::NOT_EQUAL, "!=", line);
    }
    // Handle single '!' if your language doesn't support it
    throw runtime_error("Unexpected symbol '!' at line " + to_string(line));

  case '>':
    if (peek() == '=') {
      advance();
      return Token(tokenType::GREATER_EQUAL, ">=", line);
    }
    return Token(tokenType::GREATER, ">", line);

  case '<':
    if (peek() == '=') {
      advance();
      return Token(tokenType::LESS_EQUAL, "<=", line);
    }
    return Token(tokenType::LESS, "<", line);

  default: // Fix 3: Spelling
    throw runtime_error("Unexpected symbol: " +
                        string(1, current)); // Fix 4: Char to string
  }
}

Token Lexer::identifier(char firstChar) {
  string lexeme = "";
  lexeme += firstChar;

  while (isalnum(peek()) || peek() == '_') // Fix 5: Allow numbers in names
    lexeme += advance();

  // Mapping keywords...
  if (lexeme == "set")
    return Token(tokenType::SET, lexeme, line);
  if (lexeme == "setarr")
    return Token(tokenType::SETARR, lexeme, line);
  if (lexeme == "int")
    return Token(tokenType::INT, lexeme, line);
  if (lexeme == "float")
    return Token(tokenType::FLOAT, lexeme, line);
  if (lexeme == "bool")
    return Token(tokenType::BOOL, lexeme, line);
  if (lexeme == "if")
    return Token(tokenType::IF, lexeme, line);
  if (lexeme == "elif")
    return Token(tokenType::ELIF, lexeme, line);
  if (lexeme == "else")
    return Token(tokenType::ELSE, lexeme, line);
  if (lexeme == "while")
    return Token(tokenType::WHILE, lexeme, line);
  if (lexeme == "func")
    return Token(tokenType::FUNC, lexeme, line);

  if (lexeme == "return")
    return Token(tokenType::RETURN, lexeme, line);
  // ... add your other keywords ...

  return Token(tokenType::IDENTIFIER, lexeme, line);
}

Token Lexer::number(char firstChar) {
  string lexeme = "";
  lexeme += firstChar;

  while (isdigit(peek())) // Fix 6: Use peek() here!
    lexeme += advance();

  return Token(tokenType::NUMBER, lexeme, line);
}
Lexer::~Lexer() {}
