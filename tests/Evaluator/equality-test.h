#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void EqualityTest() {
  Evaluator evaluator;

  AstNode* node = new BinaryExpressionNode("==", new NumericLiteralNode(3),
                                           new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode("!=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);
}
