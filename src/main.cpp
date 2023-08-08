#include <fstream>
#include <iostream>
#include <string>

#include "../lib/nlohmann/json.hpp"
#include "Parser.h"
#include "SyntaxError.h"

using json = nlohmann::json;

int main() {
  Parser parser;

  std::string program;

  std::ifstream file;
  file.open("program.txt", std::ios::in);
  if (file.is_open()) {
    char c;
    while (file.get(c)) {
      program += c;
    }
    file.close();
  }

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