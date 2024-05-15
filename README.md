![test workflow](https://github.com/yusiangeng/recursive-descent-parser/actions/workflows/test-ubuntu.yaml/badge.svg)
![test workflow](https://github.com/yusiangeng/recursive-descent-parser/actions/workflows/test-macos.yaml/badge.svg)


# Recursive Descent Parser

Parser for a JavaScript-like language. Outputs a JSON abstract syntax tree (AST).


## Example

```bash
./parse -e 'let x = 2 + 3;'
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
./parse -e '<your program>'

# Read your program from a file
./parse -f path/to/file
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

- Supported types: integer (`123`), string (`"hello"`), boolean (`true`/`false`), `null`
  
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

- Loops
  ```js
  while (x < 3) {
    x += 1;
  }

  for (let i = 0; i < 3; i += 1) {
    x += i;
  }
  ```


## How to Build

This project uses CMake.

```bash
mkdir build && cd build && cmake .. && make
```
The executable created is `build/parse`.


## Run automated tests

Run `build/run-tests`. Tests are defined in `tests` folder.

Tests are also run on GitHub Actions runners using `ubuntu-latest`, `macos-latest`, and `windows-2019`.


## Third Party Libraries used

- [nlohmann/json](https://github.com/nlohmann/json)
