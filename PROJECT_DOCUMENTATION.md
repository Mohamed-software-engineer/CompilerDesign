# DesignCompiler Project Documentation

## 1. Project Overview

DesignCompiler is a C++ compiler frontend for a small custom programming language. The current implementation includes:

- A lexer/tokenizer that reads source text from an input stream and produces tokens.
- A recursive descent parser that consumes tokens and builds a partial AST.
- AST node classes for expressions, statements, functions, control flow, arrays, and programs.
- An AST printer/debug utility that prints the parsed tree to the terminal.

The project currently focuses on frontend parsing and AST construction. It does not implement semantic analysis, type checking, code generation, optimization, or execution.

### Supported Language Features

Based on the current code, the language supports parsing:

- Variable declarations using `set`
- Array declarations using `setarr`
- Assignments
- Integer number literals
- Identifier expressions
- Array literal expressions using `{ ... }`
- Binary arithmetic expressions
- Binary comparison/equality expressions
- `if`, `elif`, and `else` control flow
- `while` loops
- Function declarations
- Statement-level function calls

The lexer recognizes additional tokens such as `return`, `break`, `continue`, and `using`, but the parser does not currently parse those constructs.

## 2. Language Syntax

### Variable Declaration

Variables are declared with `set`, followed by a type, an identifier, an optional initializer, and a semicolon.

```txt
set int x;
set int y = 10;
set bool check = (3 * 1) == (1 * 7);
```

Supported type keywords recognized by the parser:

- `int`
- `float`
- `char`
- `bool`

Internally, declarations are represented as `VarDeclStmt`.

### Arrays

Arrays are declared with `setarr`.

```txt
setarr int array1[size];
setarr int array2[5] = {x, y};
```

The parser accepts either a number or identifier between brackets:

```txt
setarr int arr[5];
setarr int arr[size];
```

Array initializer values use brace syntax:

```txt
{1, 2, 3}
{x, y}
```

Internally, the current parser still stores array declarations as `VarDeclStmt`. The bracket size is consumed by the parser but is not stored in the AST.

### Assignments

Assignments use an identifier, `=`, an expression, and a semicolon.

```txt
x = 10;
array1 = {1, 2, 3};
```

Assignments are represented as `AssignStmt`.

### Expressions

The parser supports:

- Number literals:

```txt
10
```

- Identifier references:

```txt
x
```

- Parenthesized expressions:

```txt
(3 * 5)
```

- Array literals:

```txt
{1, 2, 3}
```

- Binary arithmetic:

```txt
x + y
x - y
x * y
x / y
```

- Binary comparison/equality:

```txt
x > 5
x < 5
x >= 5
x <= 5
x == 5
x != 5
```

### If / Elif / Else

Conditional statements use parentheses around the condition and braces around each block.

```txt
if (x == 20) {
  set int z;
  z = 100;
} elif (y > x) {
  set int w;
  w = 100;
} else {
  if (1) {}
}
```

Internally, this is represented as `ifstmt`, containing:

- `condition`
- `thenBranch`
- `elseIfBranches`
- `elseBranch`

### While Loops

While loops use a parenthesized condition and a braced body.

```txt
while (9) {
  set int i = 50;
}
```

Internally, this is represented as `whilestmt`.

### Functions

Functions use `func`, followed by a return type, a function name, parameters, and a braced body.

```txt
func int sum(x:int, y:int) {
  set int j = 50;
}
```

Parameters use the syntax:

```txt
name:type
```

Internally, functions are represented as `FuncStmt`.

### Function Calls

The parser supports statement-level function calls:

```txt
sum(x, y);
```

Function calls require commas between arguments. Calls may also be nested inside
other call arguments or expressions:

```txt
sum(sum(x, y), y);
```

Function calls are represented using `callExpr`, which currently inherits from both `Expr` and `Stmt`.

## 3. Lexer Overview

The lexer is implemented in:

- `include/lexer/lexer.h`
- `src/lexer/lexer.cpp`
- `include/lexer/token.h`
- `src/lexer/token.cpp`
- `include/lexer/tokenType.h`
- `src/lexer/tokenType.cpp`

The lexer reads characters from a `std::istream`.

### Token Generation Flow

The main tokenization entry point is:

```cpp
Token Lexer::nextToken()
```

The lexer:

1. Skips whitespace using `skipWhitespace()`.
2. Returns `_EOF` when the stream ends.
3. Reads alphabetic sequences as identifiers or keywords.
4. Reads digit sequences as number tokens.
5. Recognizes single-character operators and symbols.
6. Recognizes two-character comparison operators such as `==`, `!=`, `>=`, and `<=`.
7. Throws `std::runtime_error` for unsupported characters.

### Token Types

Token types are defined in `include/lexer/tokenType.h`.

#### Keywords

```txt
INT
FLOAT
BOOL
CHAR
USING
SET
SETARR
IF
ELIF
ELSE
WHILE
FUNC
BREAK
CONTINUE
RETURN
```

The lexer currently maps these lexemes:

```txt
set
setarr
int
float
bool
if
elif
else
while
func
return
```

`USING`, `BREAK`, `CONTINUE`, and `CHAR` exist in the enum, but not all of them are currently mapped by `Lexer::identifier()`.

#### Identifiers and Literals

```txt
IDENTIFIER
NUMBER
```

Identifiers may contain letters, digits, and underscores after the first character. Numbers are currently integer-only digit sequences.

#### Operators

```txt
PLUS          +
MINUS         -
STAR          *
SLASH         /
ASSIGN        =
EQUAL         ==
NOT_EQUAL     !=
GREATER       >
LESS          <
GREATER_EQUAL >=
LESS_EQUAL    <=
```

#### Symbols

```txt
SEMICOLON ;
COLON     :
COMA      ,
LPAREN    (
RPAREN    )
LBRACE    {
RBRACE    }
LSBRACT   [
RSBRACT   ]
```

#### End of File

```txt
_EOF
```

## 4. Parser Overview

The parser is implemented in:

- `include/parser/parser.h`
- `src/parser/parser.cpp`

It is a recursive descent parser. It stores:

```cpp
Lexer lexer;
Token current;
```

The parser advances one token at a time using:

```cpp
void Parser::advance()
```

It checks and consumes tokens using:

```cpp
bool Parser::check(tokenType type)
bool Parser::match(tokenType type)
void Parser::expect(tokenType type)
```

`expect()` throws a parser error if the expected token is not found.

### Program Parsing

The program entry point is:

```cpp
std::unique_ptr<Program> Parser::parse()
```

It creates a `Program` node and repeatedly parses statements until `_EOF`:

```cpp
while (!check(tokenType::_EOF)) {
  program->statements.push_back(statment());
}
```

### Statement Dispatch

Statement parsing is handled by:

```cpp
std::unique_ptr<Stmt> Parser::statment()
```

The dispatch logic is:

- `SET` or `SETARR` -> `declare()`
- `IF` -> `if_stmt()`
- `WHILE` -> `while_stmt()`
- `FUNC` -> `func()`
- `IDENTIFIER` followed by `ASSIGN` -> `assign(name)`
- `IDENTIFIER` followed by `LPAREN` -> `call_func(name)`

If no known statement form matches, the parser reports an error.

### Declarations

Declarations are parsed by:

```cpp
std::unique_ptr<Stmt> Parser::declare()
```

For `set`, it parses a normal variable declaration.

For `setarr`, it parses an array declaration with brackets, but stores the result as `VarDeclStmt`.

### Blocks

Blocks are parsed by:

```cpp
std::vector<std::unique_ptr<Stmt>> Parser::block()
```

A block starts with `{`, contains zero or more statements, and ends with `}`.

### Conditions

Conditions are parsed by:

```cpp
std::unique_ptr<Expr> Parser::condition()
```

The syntax requires parentheses:

```txt
(expression)
```

### Expression Precedence

The expression parser uses several recursive descent levels:

```txt
parseExpression()
  equality()
    comaprsion()
      term()
        factor()
          unary()
```

The current precedence order is:

1. Primary expressions in `unary()`
   - numbers
   - identifiers
   - function call expressions
   - parenthesized expressions
   - array literals
2. Multiplication and division in `factor()`
3. Addition and subtraction in `term()`
4. Comparison operators in `comaprsion()`
5. Equality operators in `equality()`

Binary expressions are represented as `BinaryExpr`.

### Function Call Arguments

Function call arguments are parsed by:

```cpp
std::vector<std::unique_ptr<Expr>> Parser::call_args()
```

The syntax accepts zero or more arguments inside parentheses:

```txt
sum();
sum(x);
sum(x, y);
sum(sum(x, y), y);
```

After the first argument, every additional argument must be preceded by `COMA`.
For example, `sum(x y);` is rejected because the comma between `x` and `y` is
missing.

## 5. AST Structure

The AST base classes are:

- `ASTNode`
- `Expr`
- `Stmt`

`ASTNode` has a virtual destructor, enabling safe polymorphic deletion through base pointers.

### Program Node

Defined in:

```txt
include/ast/statements/program.h
```

Structure:

```cpp
class Program : public ASTNode {
public:
  std::vector<std::unique_ptr<Stmt>> statements;
};
```

### Statement Nodes

Implemented statement classes:

- `VarDeclStmt`
- `AssignStmt`
- `ifstmt`
- `whilestmt`
- `FuncStmt`
- `callExpr` as a statement-level call

### Expression Nodes

Implemented expression classes:

- `NumberExpr`
- `IdentifierExpr`
- `BinaryExpr`
- `ArrayExpr`
- `callExpr`

### AST Class Diagram

```txt
ASTNode
 ├── Program
 │    └── vector<unique_ptr<Stmt>> statements
 ├── Stmt
 │    ├── VarDeclStmt
 │    │    ├── type
 │    │    ├── name
 │    │    └── initializer: unique_ptr<Expr>
 │    ├── AssignStmt
 │    │    ├── name
 │    │    └── value: unique_ptr<Expr>
 │    ├── ifstmt
 │    │    ├── condition: unique_ptr<Expr>
 │    │    ├── thenBranch: vector<unique_ptr<Stmt>>
 │    │    ├── elseIfBranches
 │    │    └── elseBranch: vector<unique_ptr<Stmt>>
 │    ├── whilestmt
 │    │    ├── condition: unique_ptr<Expr>
 │    │    └── body: vector<unique_ptr<Stmt>>
 │    ├── FuncStmt
 │    │    ├── returnType
 │    │    ├── name
 │    │    ├── parameters
 │    │    └── body: vector<unique_ptr<Stmt>>
 │    └── callExpr
 │         ├── nameoffunc
 │         └── arguments: vector<unique_ptr<Expr>>
 └── Expr
      ├── NumberExpr
      │    └── value
      ├── IdentifierExpr
      │    └── name
      ├── BinaryExpr
      │    ├── op
      │    ├── left: unique_ptr<Expr>
      │    └── right: unique_ptr<Expr>
      ├── ArrayExpr
      │    └── elements: vector<unique_ptr<Expr>>
      └── callExpr
           ├── nameoffunc
           └── arguments: vector<unique_ptr<Expr>>
```

### Ownership Model

The AST uses `std::unique_ptr` for owning child nodes:

- `Program` owns statements.
- Statement blocks own nested statements.
- `BinaryExpr` owns left and right expressions.
- `ArrayExpr` owns element expressions.
- `callExpr` owns argument expressions.

## 6. AST Printer

The project includes an AST printer/debug utility:

- `include/ast/astPrinter.h`
- `src/astPrinter.cpp`

It supports printing:

- `Program`
- `VarDeclStmt`
- `AssignStmt`
- `ifstmt`
- `whilestmt`
- `FuncStmt`
- statement-level `callExpr`, printed as `ExprStmt(...)`
- `NumberExpr`
- `IdentifierExpr`
- `BinaryExpr`
- `callExpr`
- `ArrayExpr`

Usage in `src/main.cpp`:

```cpp
auto program = parser.parse();
ASTPrinter printer;
printer.print(*program);
```

Example output:

```txt
Program
  VarDecl(INT x)
  VarDecl(INT y = 10)
  Assign(x = 20)
  If
    Condition: (x == 20)
    Then:
      VarDecl(INT z)
      Assign(z = 100)
```

## 7. Known Issues and Limitations

This section reflects the current implementation.

### Parser and AST Limitations

- Array declarations are represented as `VarDeclStmt`; there is no separate `ArrayDeclStmt`.
- Array sizes inside `setarr int arr[5];` are consumed but not stored in the AST.
- `callExpr` inherits from both `Expr` and `Stmt`, so function calls are used both as expressions and statement-level calls.
- `ExprStmt` is referenced conceptually by the AST printer output, but there is no separate `ExprStmt` class in the AST.
- The `initial()` parser helper exists but is not used by the main declaration parsing path.
- `return`, `break`, `continue`, and `using` token types exist, but corresponding parser logic is not implemented.
- The lexer recognizes integer numbers only; floating-point literals are not implemented even though `FLOAT` is a type token.
- The `CHAR` token exists in the enum and parser type checks, but the lexer does not currently map the string `"char"` to `CHAR`.
- There is no semantic analysis stage, so declarations, types, scopes, and function calls are not validated beyond syntax.
- There is no code generation stage.

### Error Handling Limitations

- Parser errors report the current line and expected token type, but do not include column information.
- `block()` loops until `RBRACE`; malformed input with a missing closing brace may produce later parse errors rather than a specialized unterminated-block diagnostic.
- `statment()` returns `nullptr` on some unmatched paths after attempting dispatch. In normal error paths, `error()` throws an exception.

### Naming and Style Notes

The implementation uses names such as:

- `statment`
- `comaprsion`
- `ifstmt`
- `whilestmt`
- `callExpr`
- `COMA`
- `LSBRACT`
- `RSBRACT`

These names are part of the current code and are documented as-is.

## 8. File Structure

Simplified project layout:

```txt
.
├── Makefile
├── README.md
├── PROJECT_DOCUMENTATION.md
├── include/
│   ├── ast/
│   │   ├── astNode.h
│   │   ├── astPrinter.h
│   │   ├── expr.h
│   │   ├── stmt.h
│   │   ├── expressions/
│   │   │   ├── arrayExpr.h
│   │   │   ├── binaryExpr.h
│   │   │   ├── callExpr.h
│   │   │   ├── identifierExpr.h
│   │   │   └── numberExpr.h
│   │   └── statements/
│   │       ├── assignStmt.h
│   │       ├── funcStmt.h
│   │       ├── ifStmt.h
│   │       ├── program.h
│   │       ├── varDeclStmt.h
│   │       └── whileStmt.h
│   ├── lexer/
│   │   ├── lexer.h
│   │   ├── token.h
│   │   └── tokenType.h
│   └── parser/
│       └── parser.h
├── src/
│   ├── astPrinter.cpp
│   ├── main.cpp
│   ├── lexer/
│   │   ├── lexer.cpp
│   │   ├── sourceCode.txt
│   │   ├── token.cpp
│   │   └── tokenType.cpp
│   └── parser/
│       └── parser.cpp
└── lexer/
    └── Lexer.java
```

Generated build outputs are placed under:

```txt
build/
app
```

## 9. How to Build and Run

The project uses a `Makefile`.

### Build

```bash
make
```

This compiles all `.cpp` files under `src/` using:

```txt
g++ -Wall -Wextra -std=c++20 -Iinclude
```

The output binary is:

```txt
app
```

### Run

```bash
make run
```

or:

```bash
./app
```

The current `main.cpp` reads source code from:

```txt
/home/reda/DesignCompiler/src/lexer/sourceCode.txt
```

After parsing, it prints the AST using `ASTPrinter`.

### Clean

```bash
make clean
```

This removes:

```txt
build/
app
```

## 10. Example Program

Example input:

```txt
set int x = 5;
set int y = 10;

if (x > 3) {
  y = x + 1;
} else {
  y = 0;
}

while (y > 0) {
  y = y - 1;
}

func int sum(a:int, b:int) {
  set int result = a + b;
}

sum(x, y);
```

Expected AST shape:

```txt
Program
  VarDecl(INT x = 5)
  VarDecl(INT y = 10)
  If
    Condition: (x > 3)
    Then:
      Assign(y = (x + 1))
    Else:
      Assign(y = 0)
  While
    Condition: (y > 0)
    Body:
      Assign(y = (y - 1))
  Func(INT sum(a:int, b:int))
    Body:
      VarDecl(INT result = (a + b))
  ExprStmt(Call(sum(x, y)))
```

## 11. Current Sample Input

The repository includes this sample program in `src/lexer/sourceCode.txt`:

```txt
set int x;
set int y = 10;
x = 20;

set int x;

set bool check = (3*1) == (1*7);

set int size = 3;
setarr int array1[size];
array1 = {1, 2, 3};

setarr int array2[5] = {x, y};

if(x==20){
set int z;
z = 100;
}elif(y>x){
  set int w;
  w = 100;
}else{
  if(1){}
}
while(9){
  set int i = 50;
}

func int sum(x:int, y:int){
set int j = 50;
}
sum(x, y);
```

The parser builds an AST for this file when running `./app`. `ASTPrinter` can
produce a readable tree when it is enabled from `src/main.cpp`.
