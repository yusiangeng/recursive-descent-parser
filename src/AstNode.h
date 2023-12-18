#pragma once
#include <string>
#include <vector>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

class AstNode {
 public:
  std::string type;

  virtual json toJson() const = 0;
};

class ProgramNode : public AstNode {
 public:
  std::vector<AstNode *> body;

  ProgramNode(std::vector<AstNode *> body);

  json toJson() const override;
};

class EmptyStatementNode : public AstNode {
 public:
  EmptyStatementNode();

  json toJson() const override;
};

class BlockStatementNode : public AstNode {
 public:
  std::vector<AstNode *> body;

  BlockStatementNode(std::vector<AstNode *> body);

  json toJson() const override;
};

class ExpressionStatementNode : public AstNode {
 public:
  AstNode *expression;

  ExpressionStatementNode(AstNode *expression);

  json toJson() const override;
};

class StringLiteralNode : public AstNode {
 public:
  std::string value;

  StringLiteralNode(std::string value);

  json toJson() const override;
};

class NumericLiteralNode : public AstNode {
 public:
  long long value;

  NumericLiteralNode(long long value);

  json toJson() const override;
};

class BinaryExpressionNode : public AstNode {
 public:
  std::string op;
  AstNode *left;
  AstNode *right;

  BinaryExpressionNode(std::string op, AstNode *left, AstNode *right);

  json toJson() const override;
};

class AssignmentExpressionNode : public AstNode {
 public:
  std::string op;
  AstNode *left;
  AstNode *right;

  AssignmentExpressionNode(std::string op, AstNode *left, AstNode *right);

  json toJson() const override;
};

class IdentifierNode : public AstNode {
 public:
  std::string name;

  IdentifierNode(std::string name);

  json toJson() const override;
};

class VariableStatementNode : public AstNode {
 public:
  std::vector<AstNode *> declarations;

  VariableStatementNode(std::vector<AstNode *> declarations);

  json toJson() const override;
};

class VariableDeclarationNode : public AstNode {
 public:
  AstNode *id;
  AstNode *init;  // nullptr if variable not initialized

  VariableDeclarationNode(AstNode *id, AstNode *init);

  json toJson() const override;
};

class IfStatementNode : public AstNode {
 public:
  AstNode *test;
  AstNode *consequent;
  AstNode *alternate;

  IfStatementNode(AstNode *test, AstNode *consequent, AstNode *alternate);

  json toJson() const override;
};

class BooleanLiteralNode : public AstNode {
 public:
  bool value;

  BooleanLiteralNode(bool value);

  json toJson() const override;
};

class NullLiteralNode : public AstNode {
 public:
  NullLiteralNode();

  json toJson() const override;
};
