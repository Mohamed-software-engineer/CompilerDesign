// #include "../include/ast/astPrinter.h"
#include "../include/parser/parser.h"
#include "../include/semantic/Analyzer.h"
#include <fstream>
using namespace std;
int main() {
  ifstream source("/home/reda/DesignCompiler/src/lexer/sourceCode.txt");
  Lexer lexer = Lexer(source);
  Parser parser = Parser(lexer);
  auto program = parser.parse();
  Analyzer analyzer;
  analyzer.analyze(program.get());
  // ASTPrinter printer;
  // printer.print(*program);
  if (analyzer.hasErrors()) {
    std::cout << "Semantic analysis found " << analyzer.getErrors().size()
              << " error(s)" << endl;
  } else {
    std::cout << "Semantic analysis completed with no errors" << endl;
  }
  std::cout << "Compelete" << endl;
  return 0;
}

/*
  if (argc) {
 }
 string source = "set int x = 0; \
                  if (x){ \
                   retrun 0;}";

Token token(tokenType::_EOF, "", 0);
 */
