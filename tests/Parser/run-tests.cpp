#include <iostream>
#include <json.hpp>
#include <string>
#include <vector>

#include "../../src/Parser.h"
#include "tests.h"

using json = nlohmann::json;

// Our parser implementation.
Parser parser;

void test(std::string program, json expected) {
  Ast ast = parser.parse(program);
  json result = ast;
  if (result != expected) {
    std::cout << "result:" << std::endl << result.dump(2) << std::endl;
    std::cout << "expected:" << std::endl << expected.dump(2) << std::endl;
    throw std::runtime_error("Assertion failed");
  }
}

int main() {
  std::vector<std::function<void(std::function<void(std::string, json)>)>>
      tests{LiteralsTest,   StatementListTest, BlockTest,    EmptyStatementTest,
            MathTest,       AssignmentTest,    VariableTest, IfTest,
            RelationalTest, EqualityTest,      LogicalTest,  UnaryTest,
            WhileTest,      DoWhileTest,       ForTest};

  // Run all tests
  for (auto testRun : tests) {
    testRun(test);
  }

  std::cout << "All assertions passed!" << std::endl;
}
