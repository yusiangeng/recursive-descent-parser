#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void LiteralsTest() {
  Evaluator evaluator;

  AstNode* node = new NumericLiteralNode(431);
  CHECK_NUMBER(evaluator.eval(node), 431);

  node = new NumericLiteralNode(-10000);
  CHECK_NUMBER(evaluator.eval(node), -10000);

  node = new StringLiteralNode("  Hello hi   haha!");
  CHECK_STRING(evaluator.eval(node), "  Hello hi   haha!");

  node = new NullLiteralNode();
  CHECK_NULL(evaluator.eval(node));

  node = new BooleanLiteralNode(true);
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BooleanLiteralNode(false);
  CHECK_BOOL(evaluator.eval(node), false);
}
