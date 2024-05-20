#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void RelationalTest() {
  Evaluator evaluator;

  // <, <=
  AstNode* node = new BinaryExpressionNode("<", new NumericLiteralNode(3),
                                           new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<", new NumericLiteralNode(2),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode("<", new NumericLiteralNode(4),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode("<=", new NumericLiteralNode(2),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode("<=", new NumericLiteralNode(4),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  // >, >=
  node = new BinaryExpressionNode(">", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode(">", new NumericLiteralNode(2),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode(">", new NumericLiteralNode(4),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode(">=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode(">=", new NumericLiteralNode(2),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode(">=", new NumericLiteralNode(4),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);
}
