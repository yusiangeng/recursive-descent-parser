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
