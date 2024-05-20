#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void MathTest() {
  Evaluator evaluator;

  AstNode* node = new BinaryExpressionNode(
      "*",
      new BinaryExpressionNode("+", new NumericLiteralNode(1),
                               new NumericLiteralNode(5)),
      new NumericLiteralNode(4));
  CHECK_NUMBER(evaluator.eval(node), 24);

  node = new BinaryExpressionNode("-", new NumericLiteralNode(93),
                                  new NumericLiteralNode(4));
  CHECK_NUMBER(evaluator.eval(node), 89);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new NumericLiteralNode(3));
  CHECK_NUMBER(evaluator.eval(node), 4);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(93),
                                  new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, DivideZeroError);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new StringLiteralNode("hi"));
  CHECK_EVAL_ERROR(evaluator, node, TypeError);
}
