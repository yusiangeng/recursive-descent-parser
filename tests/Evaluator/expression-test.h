#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void ExpressionTest() {
  Evaluator evaluator;

  AstNode* node = new ExpressionStatementNode(new NumericLiteralNode(77));
  CHECK_NUMBER(evaluator.eval(node), 77);

  node = new ExpressionStatementNode(new StringLiteralNode("bar"));
  CHECK_STRING(evaluator.eval(node), "bar");

  node = new VariableStatementNode({
      new VariableDeclarationNode(new IdentifierNode("x"),
                                  new NumericLiteralNode(56)),
  });

  CHECK_UNDEFINED(evaluator.eval(node));
  node = new ExpressionStatementNode(new IdentifierNode("x"));
  CHECK_NUMBER(evaluator.eval(node), 56);
}
