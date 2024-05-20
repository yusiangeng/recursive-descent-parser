#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void AssignmentTest() {
  Evaluator evaluator;

  AstNode* node = new VariableStatementNode({
      new VariableDeclarationNode(new IdentifierNode("x"),
                                  new NumericLiteralNode(10)),
  });
  CHECK_UNDEFINED(evaluator.eval(node));

  node = new AssignmentExpressionNode("=", new IdentifierNode("y"),
                                      new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, ReferenceError);

  node = new AssignmentExpressionNode("=", new IdentifierNode("x"),
                                      new NumericLiteralNode(0));
  CHECK_NUMBER(evaluator.eval(node), 0);

  node = new AssignmentExpressionNode("+=", new IdentifierNode("x"),
                                      new NumericLiteralNode(15));
  CHECK_NUMBER(evaluator.eval(node), 15);

  node = new AssignmentExpressionNode("/=", new IdentifierNode("x"),
                                      new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, DivideZeroError);
}
