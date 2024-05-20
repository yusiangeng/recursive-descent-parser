#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void LogicalTest() {
  Evaluator evaluator;

  AstNode* node = new LogicalExpressionNode("||", new BooleanLiteralNode(true),
                                            new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new LogicalExpressionNode("||", new BooleanLiteralNode(true),
                                   new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new LogicalExpressionNode("||", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new LogicalExpressionNode("||", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new LogicalExpressionNode("&&", new BooleanLiteralNode(true),
                                   new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new LogicalExpressionNode("&&", new BooleanLiteralNode(true),
                                   new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new LogicalExpressionNode("&&", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new LogicalExpressionNode("&&", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), false);
}
