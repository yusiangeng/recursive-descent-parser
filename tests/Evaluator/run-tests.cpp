// test cases
#include "assignment-test.h"
#include "block-test.h"
#include "empty-statement-test.h"
#include "equality-test.h"
#include "expression-test.h"
#include "literals-test.h"
#include "logical-test.h"
#include "math-test.h"
#include "relational-test.h"
#include "statement-list-test.h"
#include "unary-test.h"
#include "variable-test.h"

int main() {
  LiteralsTest();
  EmptyStatementTest();
  UnaryTest();
  MathTest();
  LogicalTest();
  EqualityTest();
  RelationalTest();
  VariableTest();
  ExpressionTest();
  AssignmentTest();
  BlockTest();
  StatementListTest();

  std::cout << "All assertions passed!" << std::endl;
}
