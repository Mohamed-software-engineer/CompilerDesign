# Custom Compiler Design

A small C++ compiler frontend for a custom teaching language. The project reads a source file, tokenizes it, parses it into an AST, and runs a semantic analysis pass with scoped symbol tables.

This is not a full compiler yet. It does not generate machine code, bytecode, or an executable program. The focus is on the early compiler stages: lexer, parser, AST structure, and semantic checks.

## What It Can Do

- Tokenize a source program into keywords, identifiers, literals, operators, and punctuation.
- Parse declarations, assignments, expressions, arrays, conditionals, loops, functions, and function calls.
- Build an AST using C++ classes and smart pointers.
- Track scopes with a symbol table.
- Report semantic issues such as duplicate declarations, undefined identifiers, assigning to functions, and wrong function argument counts.

## Project Layout

```txt
.
├── Makefile
├── PROJECT_DOCUMENTATION.md
├── README.md
├── include/
│   ├── ast/
│   ├── lexer/
│   ├── parser/
│   └── semantic/
└── src/
    ├── astPrinter.cpp
    ├── lexer/
    ├── parser/
    ├── semantic/
    └── main.cpp
```

The main pieces are:

- `src/lexer/` and `include/lexer/`: lexical analysis.
- `src/parser/` and `include/parser/`: recursive descent parser.
- `include/ast/`: AST node definitions.
- `src/semantic/` and `include/semantic/`: semantic analyzer, scopes, and symbol table.
- `src/lexer/sourceCode.txt`: sample input program.

## Requirements

- A C++20-capable compiler, such as `g++`
- `make`

## Build

```bash
make
```

This creates the executable:

```txt
app
```

To clean generated files:

```bash
make clean
```

## Run

```bash
make run
```

Important: `src/main.cpp` currently opens the input file using an absolute path:

```cpp
ifstream source("/home/reda/DesignCompiler/src/lexer/sourceCode.txt");
```

Before running on another machine, change that path to the local sample file:

```cpp
ifstream source("src/lexer/sourceCode.txt");
```

After that, rebuild and run:

```bash
make
make run
```

The analyzer prints scope and symbol-table debug output, then reports whether semantic analysis found errors.

## Language Example

```txt
set int x;
set int y = 10;
x = 20;

set bool check = (3 * 1) == (1 * 7);

set int size = 3;
setarr int array1[size];
array1 = {1, 2, 3};

if (x == 20) {
  set int z;
  z = 100;
} elif (y > x) {
  set int w;
  w = 100;
} else {
  if (1) {}
}

while (9) {
  set int i = 50;
}

func int sum(x:int, y:int) {
  set int j = 50;
}

sum(x, y);
```

## Supported Syntax

Variable declarations:

```txt
set int x;
set int y = 10;
```

Array declarations:

```txt
setarr int values[5];
setarr int values[size] = {1, 2, 3};
```

Assignments:

```txt
x = 20;
values = {1, 2, 3};
```

Expressions:

```txt
x + y
x * 10
(3 * 1) == (1 * 7)
x >= y
```

Conditionals:

```txt
if (x == 20) {
  set int z;
} elif (x > 10) {
  set int w;
} else {
  set int fallback;
}
```

Loops:

```txt
while (x < 10) {
  x = x + 1;
}
```

Functions:

```txt
func int sum(x:int, y:int) {
  set int result = x + y;
}

sum(1, 2);
```

## Current Limitations

- The project is a frontend only; it does not execute programs or generate code.
- Types are parsed and stored, but full type checking is not implemented yet.
- `return`, `break`, `continue`, and `using` exist as token types or planned keywords, but they are not fully parsed.
- Array sizes are consumed by the parser but are not deeply represented or checked in the AST.
- The input file path in `main.cpp` is currently machine-specific and should be made relative or passed as a command-line argument.

## Useful Files

- `PROJECT_DOCUMENTATION.md` contains deeper notes about the current implementation.
- `src/lexer/sourceCode.txt` is the easiest place to edit the sample program.
- `src/main.cpp` is the entry point if you want to change how input files are loaded.

## Next Good Improvements

- Accept the source file path from the command line.
- Add parser support for `return`, `break`, and `continue`.
- Add stronger type checking.
- Store array sizes in the AST and validate array initializers.
- Add automated tests for the lexer, parser, and semantic analyzer.
