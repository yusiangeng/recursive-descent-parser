#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void StatementListTest() {
  Evaluator evaluator;

  AstNode* node = new ProgramNode({
      new EmptyStatementNode(),
      new ExpressionStatementNode(new NumericLiteralNode(47)),
      new ExpressionStatementNode(new StringLiteralNode("foo")),
  });
  CHECK_STRING(evaluator.eval(node), "foo");

  node = new ProgramNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("data"),
                                      new NumericLiteralNode(10)),
      }),
      new BlockStatementNode({
          new AssignmentExpressionNode("=", new IdentifierNode("data"),
                                       new NumericLiteralNode(100)),
      }),
      new ExpressionStatementNode(new IdentifierNode("data")),
  });
  CHECK_NUMBER(evaluator.eval(node), 100);
}
