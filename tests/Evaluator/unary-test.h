#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void UnaryTest() {
  Evaluator evaluator;

  AstNode* node = new UnaryExpressionNode("-", new NumericLiteralNode(1));
  CHECK_NUMBER(evaluator.eval(node), -1);

  node = new UnaryExpressionNode("+", new NumericLiteralNode(-1));
  CHECK_NUMBER(evaluator.eval(node), -1);

  node = new UnaryExpressionNode("!", new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(1));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(0));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(2));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(-1));
  CHECK_BOOL(evaluator.eval(node), false);
}
