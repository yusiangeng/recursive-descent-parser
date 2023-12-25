![test workflow](https://github.com/yusiangeng/recursive-descent-parser/actions/workflows/test.yaml/badge.svg)

# Recursive Descent Parser

Parser for a JavaScript-like language. Outputs a JSON abstract syntax tree (AST).

## Example

```bash
bin/parse -e 'let x = 2 + 3;'
```

Output:

```json
{
  "type": "Program",
  "body": [
    {
      "type": "VariableStatement",
      "declarations": [
        {
          "type": "VariableDeclaration",
          "id": {
            "type": "Identifier",
            "name": "x"
          },
          "init": {
            "type": "BinaryExpression",
            "left": {
              "type": "NumericLiteral",
              "value": 2
            },
            "operator": "+",
            "right": {
              "type": "NumericLiteral",
              "value": 3
            }
          }
        }
      ]
    }
  ]
}
```

## Usage

```bash
# Enter your program inline
bin/parse -e '<your program>'

# Read your program from a file
bin/parse -f path/to/file
```

## Language Features

- Math
  ```js
  4 * (5 + 6) / 2;
  ```
- Variable declarations with no explicit typing
  ```js
  let x, y = 10, z = "hello";
  ```
- Supported types: integer, string, boolean, null
- Assignment
  ```js
  x = 10;
  y += 2;
  ```
- Block scopes
  ```js
  let x = 2;
  {
    let y = 3;
  }
  ```
- If-else statements
  ```js
  if (x > 3) {
    y = 1;
  } else {
    y = 2;
  }
  ```

## How to Build

1. Install a C++ compiler. The `Makefile` uses `g++` by default. If you are using something else, change the `CXX` variable accordingly.
2. Install make.
3. Run `make`.
4. The executable will be created as `bin/parse`.

## Run automated tests

Run `make test`. Tests are defined in `tests` folder.

## Third Party Libraries

- [nlohmann/json](https://github.com/nlohmann/json)
