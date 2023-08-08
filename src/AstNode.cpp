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
