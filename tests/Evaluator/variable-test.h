#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void VariableTest() {
  Evaluator evaluator;

  AstNode* node = new VariableStatementNode({
      new VariableDeclarationNode(new IdentifierNode("x"),
                                  new NumericLiteralNode(56)),
  });
  CHECK_UNDEFINED(evaluator.eval(node));

  node = new IdentifierNode("x");
  CHECK_NUMBER(evaluator.eval(node), 56);

  node = new IdentifierNode("y");
  CHECK_EVAL_ERROR(evaluator, node, ReferenceError);

  node = new VariableStatementNode({
      new VariableDeclarationNode(
          new IdentifierNode("z"),
          new BinaryExpressionNode("+", new NumericLiteralNode(1),
                                   new IdentifierNode("x"))),
  });
  CHECK_UNDEFINED(evaluator.eval(node));

  node = new IdentifierNode("z");
  CHECK_NUMBER(evaluator.eval(node), 57);
}
