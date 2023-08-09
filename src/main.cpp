#include <fstream>
#include <iostream>
#include <string>

#include "../lib/nlohmann/json.hpp"
#include "Parser.h"
#include "SyntaxError.h"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage:\n"
                 "  bin/parse -e '<your program>'\n"
                 "  bin/parse -f path/to/file"
              << std::endl;
    return 1;
  }

  std::string mode = argv[1];
  std::string exp = argv[2];

  std::string program;
  if (mode == "-e") {
    program = exp;
  } else if (mode == "-f") {
    std::ifstream file;
    file.open(exp, std::ios::in);
    if (file.is_open()) {
      char c;
      while (file.get(c)) {
        program += c;
      }
      file.close();
    }
  } else {
    std::cout << "Invalid mode: has to be either -e or -f" << std::endl;
    return 1;
  }

  Parser parser;
  Ast ast;
  try {
    ast = parser.parse(program);
  } catch (SyntaxError e) {
    std::cout << "SyntaxError: " << e.what() << std::endl;
    return 1;
  }

  json j = ast;
  std::cout << j.dump(2) << std::endl;
}