#pragma once

#define UNUSED(expr) (void)(expr)

#include <json.hpp>
#include <string>
#include <vector>

#include "Environment.h"
#include "EvalValue.h"

using json = nlohmann::json;

class AstNode {
 public:
  std::string type;

  virtual json toJson() const = 0;

  virtual EvalValue *eval(Environment &env) const {
    UNUSED(env);
    throw std::runtime_error("eval() unimplemented");
  };
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
  virtual EvalValue *eval(Environment &env) const override;
};

class NumericLiteralNode : public AstNode {
 public:
  long long value;

  NumericLiteralNode(long long value);

  json toJson() const override;
  virtual EvalValue *eval(Environment &env) const override;
};

class BinaryExpressionNode : public AstNode {
 public:
  std::string op;
  AstNode *left;
  AstNode *right;

  BinaryExpressionNode(std::string op, AstNode *left, AstNode *right);

  json toJson() const override;
  virtual EvalValue *eval(Environment &env) const override;
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
  virtual EvalValue *eval(Environment &env) const override;
};

class VariableStatementNode : public AstNode {
 public:
  std::vector<AstNode *> declarations;

  VariableStatementNode(std::vector<AstNode *> declarations);

  json toJson() const override;
  virtual EvalValue *eval(Environment &env) const override;
};

class VariableDeclarationNode : public AstNode {
 public:
  IdentifierNode *id;
  AstNode *init;  // nullptr if variable not initialized

  VariableDeclarationNode(IdentifierNode *id, AstNode *init);

  json toJson() const override;
  virtual EvalValue *eval(Environment &env) const override;
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
  virtual EvalValue *eval(Environment &env) const override;
};

class NullLiteralNode : public AstNode {
 public:
  NullLiteralNode();

  json toJson() const override;
  virtual EvalValue *eval(Environment &env) const override;
};

class LogicalExpressionNode : public AstNode {
 public:
  std::string op;
  AstNode *left;
  AstNode *right;

  LogicalExpressionNode(std::string op, AstNode *left, AstNode *right);

  json toJson() const override;
};

class UnaryExpressionNode : public AstNode {
 public:
  std::string op;
  AstNode *argument;

  UnaryExpressionNode(std::string op, AstNode *argument);

  json toJson() const override;
};

class WhileStatementNode : public AstNode {
 public:
  AstNode *test;
  AstNode *body;

  WhileStatementNode(AstNode *test, AstNode *argument);

  json toJson() const override;
};

class DoWhileStatementNode : public AstNode {
 public:
  AstNode *test;
  AstNode *body;

  DoWhileStatementNode(AstNode *test, AstNode *argument);

  json toJson() const override;
};

class ForStatementNode : public AstNode {
 public:
  AstNode *init;
  AstNode *test;
  AstNode *update;
  AstNode *body;

  ForStatementNode(AstNode *init, AstNode *test, AstNode *update,
                   AstNode *body);

  json toJson() const override;
};
