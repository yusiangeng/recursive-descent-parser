#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void BlockTest() {
  Evaluator evaluator;

  AstNode* node = new BlockStatementNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("x"),
                                      new NumericLiteralNode(10)),
      }),
      new AssignmentExpressionNode("=", new IdentifierNode("x"),
                                   new NumericLiteralNode(20)),
  });
  CHECK_NUMBER(evaluator.eval(node), 20);

  node = new BlockStatementNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("x"),
                                      new NumericLiteralNode(10)),
      }),
      new AssignmentExpressionNode("=", new IdentifierNode("x"),
                                   new StringLiteralNode("hi")),
  });
  CHECK_STRING(evaluator.eval(node), "hi");

  node = new BlockStatementNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("x"),
                                      new NumericLiteralNode(10)),
      }),
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("y"),
                                      new NumericLiteralNode(20)),
      }),
      new BinaryExpressionNode(
          "+",
          new BinaryExpressionNode("*", new IdentifierNode("x"),
                                   new IdentifierNode("y")),
          new NumericLiteralNode(30)),
  });
  CHECK_NUMBER(evaluator.eval(node), 230);

  // nested scope
  node = new BlockStatementNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("x"),
                                      new NumericLiteralNode(10)),
      }),
      new BlockStatementNode({
          new VariableStatementNode({
              new VariableDeclarationNode(new IdentifierNode("x"),
                                          new NumericLiteralNode(20)),
          }),
          new ExpressionStatementNode(new IdentifierNode("x")),
      }),
      new ExpressionStatementNode(new IdentifierNode("x")),
  });
  CHECK_NUMBER(evaluator.eval(node), 10);

  node = new BlockStatementNode({
      new VariableStatementNode({
          new VariableDeclarationNode(new IdentifierNode("value"),
                                      new NumericLiteralNode(10)),
          new VariableDeclarationNode(
              new IdentifierNode("result"),
              new BlockStatementNode({
                  new VariableStatementNode({
                      new VariableDeclarationNode(
                          new IdentifierNode("x"),
                          new BinaryExpressionNode("+",
                                                   new IdentifierNode("value"),
                                                   new NumericLiteralNode(10))),
                  }),
                  new ExpressionStatementNode(new IdentifierNode("x")),
              })),
      }),
      new ExpressionStatementNode(new IdentifierNode("result")),
  });
  CHECK_NUMBER(evaluator.eval(node), 20);

  node = new BlockStatementNode({
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
