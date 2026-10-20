# Mini Compiler Frontend in C++

A small, educational **lexer + recursive-descent parser + interpreter** implemented in modern C++17. It is a good starting point for learning the fundamentals of compiler frontends: tokenization, syntax analysis, expression precedence, symbol tables, and diagnostics.

> **Scope:** This is a toy-language interpreter with a compiler-style frontend, **not** a native-code compiler, PTX compiler, or CUDA implementation. It does not generate assembly or machine code.

## Features

- **Lexical analysis:** identifiers, integer literals, keywords, arithmetic operators, parentheses, assignment, semicolons, `#` comments.
- **Recursive-descent parsing:** respects `*`/`/` precedence over `+`/`-`, supports unary minus and parentheses.
- **Symbol table:** tracks declared variables and their integer values using `std::map`.
- **Execution:** `let` variable declarations and `print` statements.
- **Diagnostics:** line/column information for invalid tokens, syntax errors, undefined variables, and division by zero.
- **Debugging modes:** `--tokens` to inspect token streams and `--symbols` to display variable state.
- **Automated tests:** eight cases covering normal execution and failures.

## Requirements

- C++17 compiler (`g++`, `clang++`, or MSVC)
- CMake 3.16+ (recommended), or compile directly with `g++`
- Python 3 (only for the test script)

## Build and run

```bash
cmake -S . -B build
cmake --build build
./build/mini_compiler examples/demo.mini
```

**macOS/Linux without CMake:**

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/*.cpp -o mini_compiler
./mini_compiler examples/demo.mini
```

**Windows:** use the executable in your CMake build output directory (location depends on generator).

Expected output:

```text
40
25
-5
```

Inspect tokens or the symbol table:

```bash
./build/mini_compiler examples/demo.mini --tokens
./build/mini_compiler examples/demo.mini --symbols
```

## Toy language

```text
# Comments start with #
let x = 10;
let y = (x + 2) * 3;
print y;
print y / 2;
```

### Grammar (simplified EBNF)

```text
program    = { statement } EOF ;
statement  = "let" IDENTIFIER "=" expression ";"
           | "print" expression ";" ;
expression = term { ("+" | "-") term } ;
term       = factor { ("*" | "/") factor } ;
factor     = "-" factor | NUMBER | IDENTIFIER | "(" expression ")" ;
```

Integers use signed `long long`; arithmetic overflow is **not checked** and should not be relied upon. Division is integer division truncating toward zero. Variables must be declared before use; `let` with an existing name overwrites the previous value.

## Architecture

```text
Source file (.mini)
       |
       v
Lexer (src/lexer.cpp)
       |  vector<Token>
       v
Recursive-descent parser (src/parser.cpp)
       |  statements + expression evaluation
       v
Symbol table (map<string, long long>)
       |
       v
Printed output / diagnostics
```

## Project structure

```text
mini-compiler-cpp/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── .gitignore
├── include/
│   ├── lexer.hpp
│   └── parser.hpp
├── src/
│   ├── main.cpp
│   ├── lexer.cpp
│   └── parser.cpp
├── examples/
│   ├── demo.mini
│   └── error.mini
└── tests/
    └── run_tests.py
```

## Testing

```bash
cmake -S . -B build && cmake --build build
python3 tests/run_tests.py build/mini_compiler
```

The test suite checks arithmetic precedence, parentheses, variables, unary minus, undefined variables, division by zero, missing semicolons, and invalid characters.

## What I learned / technical concepts

- How a lexer converts raw source text into a token stream
- How a recursive-descent parser implements a grammar with operator precedence
- How a symbol table stores variable bindings
- How error messages can include source line and column
- How to structure a C++17 project with headers, implementation files, CMake, and tests

## Limitations and next steps

- No AST or intermediate representation (expressions are evaluated during parsing).
- No code generation, optimization, linking, GPU instructions, or PTX support.
- No functions, types beyond integers, or control flow.
- Arithmetic overflow is not detected.

Possible future enhancements: build an AST, implement static semantic analysis, generate three-address IR, add constant folding, and optionally build a backend targeting a simple stack VM.

## GitHub publishing

Create a new public repository named `mini-compiler-cpp` under your GitHub account, then run:

```bash
git init
git add .
git commit -m "Build C++ mini compiler frontend and interpreter"
git branch -M main
git remote add origin https://github.com/Adarsh18002005/mini-compiler-cpp.git
git push -u origin main
```

Create the empty GitHub repository **without** initializing it with another README to avoid an initial merge conflict. Review and understand the code before discussing it in an interview.

## License

MIT — see [LICENSE](LICENSE).
