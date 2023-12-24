#include "AstNode.h"

#include <vector>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

ProgramNode::ProgramNode(std::vector<AstNode *> body) {
  type = "Program";
  this->body = body;
}

json ProgramNode::toJson() const {
  std::vector<json> bodyJson;
  for (AstNode *statement : body) {
    bodyJson.push_back(statement->toJson());
  }
  return json{{"type", type}, {"body", bodyJson}};
}

EmptyStatementNode::EmptyStatementNode() { type = "EmptyStatement"; }

json EmptyStatementNode::toJson() const { return json{{"type", type}}; }

BlockStatementNode::BlockStatementNode(std::vector<AstNode *> body) {
  type = "BlockStatement";
  this->body = body;
}

json BlockStatementNode::toJson() const {
  std::vector<json> bodyJson;
  for (AstNode *statement : body) {
    bodyJson.push_back(statement->toJson());
  }
  return json{{"type", type}, {"body", bodyJson}};
}

ExpressionStatementNode::ExpressionStatementNode(AstNode *expression) {
  this->type = "ExpressionStatement";
  this->expression = expression;
}

json ExpressionStatementNode::toJson() const {
  return json{{"type", type}, {"expression", expression->toJson()}};
}

StringLiteralNode::StringLiteralNode(std::string value) {
  type = "StringLiteral";
  this->value = value;
}

json StringLiteralNode::toJson() const {
  return json{{"type", type}, {"value", value}};
}

NumericLiteralNode::NumericLiteralNode(long long value) {
  type = "NumericLiteral";
  this->value = value;
}

json NumericLiteralNode::toJson() const {
  return json{{"type", type}, {"value", value}};
}

BinaryExpressionNode::BinaryExpressionNode(std::string op, AstNode *left,
                                           AstNode *right) {
  type = "BinaryExpression";
  this->op = op;
  this->left = left;
  this->right = right;
}

json BinaryExpressionNode::toJson() const {
  return json{{"type", type},
              {"operator", op},
              {"left", left->toJson()},
              {"right", right->toJson()}};
}

AssignmentExpressionNode::AssignmentExpressionNode(std::string op,
                                                   AstNode *left,
                                                   AstNode *right) {
  type = "AssignmentExpression";
  this->op = op;
  this->left = left;
  this->right = right;
}

json AssignmentExpressionNode::toJson() const {
  return json{{"type", type},
              {"operator", op},
              {"left", left->toJson()},
              {"right", right->toJson()}};
}

IdentifierNode::IdentifierNode(std::string name) {
  type = "Identifier";
  this->name = name;
}

json IdentifierNode::toJson() const {
  return json{
      {"type", type},
      {"name", name},
  };
}

VariableStatementNode::VariableStatementNode(
    std::vector<AstNode *> declarations) {
  type = "VariableStatement";
  this->declarations = declarations;
}

json VariableStatementNode::toJson() const {
  std::vector<json> declarationsJson;
  for (AstNode *declaration : declarations) {
    declarationsJson.push_back(declaration->toJson());
  }
  return json{
      {"type", type},
      {"declarations", declarationsJson},
  };
}

VariableDeclarationNode::VariableDeclarationNode(AstNode *id, AstNode *init) {
  type = "VariableDeclaration";
  this->id = id;
  this->init = init;
}

json VariableDeclarationNode::toJson() const {
  return json{{"type", type},
              {"id", id->toJson()},
              {"init", init ? init->toJson() : nullptr}};
}

IfStatementNode::IfStatementNode(AstNode *test, AstNode *consequent,
                                 AstNode *alternate) {
  type = "IfStatement";
  this->test = test;
  this->consequent = consequent;
  this->alternate = alternate;
}

json IfStatementNode::toJson() const {
  return json{{"type", type},
              {"test", test->toJson()},
              {"consequent", consequent ? consequent->toJson() : nullptr},
              {"alternate", alternate ? alternate->toJson() : nullptr}};
}

BooleanLiteralNode::BooleanLiteralNode(bool value) : value(value) {
  type = "BooleanLiteral";
}

json BooleanLiteralNode::toJson() const {
  return json{{"type", type}, {"value", value}};
}

NullLiteralNode::NullLiteralNode() { type = "NullLiteral"; }

json NullLiteralNode::toJson() const {
  return json{{"type", type}, {"value", nullptr}};
}

LogicalExpressionNode::LogicalExpressionNode(std::string op, AstNode *left,
                                             AstNode *right) {
  type = "LogicalExpression";
  this->op = op;
  this->left = left;
  this->right = right;
}

json LogicalExpressionNode::toJson() const {
  return json{{"type", type},
              {"operator", op},
              {"left", left->toJson()},
              {"right", right->toJson()}};
}
