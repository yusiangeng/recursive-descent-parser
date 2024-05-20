
#include "AstNode.h"

#include <json.hpp>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "Environment.h"
#include "Error.h"
#include "EvalValue.h"

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

EvalValue *ProgramNode::eval(Environment &env) const {
  EvalValue *result = nullptr;
  for (const AstNode *statement : body) {
    result = statement->eval(env);
  }
  return result;
}

EmptyStatementNode::EmptyStatementNode() { type = "EmptyStatement"; }

json EmptyStatementNode::toJson() const { return json{{"type", type}}; }

EvalValue *EmptyStatementNode::eval(Environment &env) const {
  UNUSED(env);
  return new EvalUndefined();
}

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

EvalValue *BlockStatementNode::eval(Environment &env) const {
  Environment blockEnv(&env);
  EvalValue *result = nullptr;
  for (const AstNode *statement : body) {
    result = statement->eval(blockEnv);
  }
  return result;
}

ExpressionStatementNode::ExpressionStatementNode(AstNode *expression) {
  this->type = "ExpressionStatement";
  this->expression = expression;
}

json ExpressionStatementNode::toJson() const {
  return json{{"type", type}, {"expression", expression->toJson()}};
}

EvalValue *ExpressionStatementNode::eval(Environment &env) const {
  return expression->eval(env);
}

StringLiteralNode::StringLiteralNode(std::string value) {
  type = "StringLiteral";
  this->value = value;
}

json StringLiteralNode::toJson() const {
  return json{{"type", type}, {"value", value}};
}

EvalValue *StringLiteralNode::eval(Environment &env) const {
  UNUSED(env);
  return new EvalString(value);
}

NumericLiteralNode::NumericLiteralNode(long long value) {
  type = "NumericLiteral";
  this->value = value;
}

json NumericLiteralNode::toJson() const {
  return json{{"type", type}, {"value", value}};
}

EvalValue *NumericLiteralNode::eval(Environment &env) const {
  UNUSED(env);
  return new EvalNumber(value);
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

EvalValue *BinaryExpressionNode::eval(Environment &env) const {
  // TODO: implement equality for strings
  if (!std::unordered_set<std::string>{"+", "-", "*", "/", "==", "!=", "<",
                                       "<=", ">", ">="}
           .count(op)) {
    std::stringstream ss;
    ss << "Unsupported operator " << op << " for Binary Expression";
    throw std::runtime_error(ss.str());
  }

  EvalValue *leftValue = left->eval(env);
  long long leftNumber;
  if (auto *leftValueNumber = dynamic_cast<EvalNumber *>(leftValue)) {
    leftNumber = leftValueNumber->getValue();
  } else if (auto *leftValueBool = dynamic_cast<EvalBool *>(leftValue)) {
    leftNumber = leftValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Left hand side of operation " << op
       << " is not a number or boolean: " << leftValue->typeStr() << ": "
       << leftValue->str();
    throw TypeError(ss.str());
  }

  EvalValue *rightValue = right->eval(env);
  long long rightNumber;
  if (auto *rightValueNumber = dynamic_cast<EvalNumber *>(rightValue)) {
    rightNumber = rightValueNumber->getValue();
  } else if (auto *rightValueBool = dynamic_cast<EvalBool *>(rightValue)) {
    rightNumber = rightValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Right hand side of operation " << op
       << " is not a number or boolean: " << rightValue->typeStr() << ": "
       << rightValue->str();
    throw TypeError(ss.str());
  }

  if (op == "/" && rightNumber == 0) {
    std::stringstream ss;
    ss << "Cannot divide " << leftNumber << " by zero";
    throw DivideZeroError(ss.str());
  }

  if (op == "+")
    return new EvalNumber(leftNumber + rightNumber);
  else if (op == "-")
    return new EvalNumber(leftNumber - rightNumber);
  else if (op == "*")
    return new EvalNumber(leftNumber * rightNumber);
  else if (op == "/")
    return new EvalNumber(leftNumber / rightNumber);
  else if (op == "==")
    return new EvalBool(leftNumber == rightNumber);
  else if (op == "!=")
    return new EvalBool(leftNumber != rightNumber);
  else if (op == "<")
    return new EvalBool(leftNumber < rightNumber);
  else if (op == "<=")
    return new EvalBool(leftNumber <= rightNumber);
  else if (op == ">")
    return new EvalBool(leftNumber > rightNumber);
  else
    return new EvalBool(leftNumber >= rightNumber);
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

EvalValue *IdentifierNode::eval(Environment &env) const {
  return env.lookup(name);
}

AssignmentExpressionNode::AssignmentExpressionNode(std::string op,
                                                   IdentifierNode *left,
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

EvalValue *AssignmentExpressionNode::eval(Environment &env) const {
  if (!std::unordered_set<std::string>{"=", "+=", "-=", "*=", "/="}.count(op)) {
    std::stringstream ss;
    ss << "Unsupported operator " << op << " for Assignment Expression";
    throw std::runtime_error(ss.str());
  }

  EvalValue *rightValue = right->eval(env);
  if (op == "=") return env.assign(left->name, rightValue);

  EvalValue *leftValue = left->eval(env);
  auto *leftValueNumber = dynamic_cast<EvalNumber *>(leftValue);
  if (!leftValueNumber) {
    std::stringstream ss;
    ss << "Left hand side of operation " << op
       << " is not a number: " << leftValue->typeStr() << ": "
       << leftValue->str();
    throw TypeError(ss.str());
  }
  long long leftNumber = leftValueNumber->getValue();

  long long rightNumber;
  if (auto *rightValueNumber = dynamic_cast<EvalNumber *>(rightValue)) {
    rightNumber = rightValueNumber->getValue();
  } else if (auto *rightValueBool = dynamic_cast<EvalBool *>(rightValue)) {
    rightNumber = rightValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Right hand side of operation " << op
       << " is not a number or boolean: " << rightValue->typeStr() << ": "
       << rightValue->str();
    throw TypeError(ss.str());
  }

  if (op == "/=" && rightNumber == 0) {
    std::stringstream ss;
    ss << "Cannot divide " << leftNumber << " by zero";
    throw DivideZeroError(ss.str());
  }

  if (op == "+=")
    return env.assign(left->name, new EvalNumber(leftNumber + rightNumber));
  else if (op == "-=")
    return env.assign(left->name, new EvalNumber(leftNumber - rightNumber));
  else if (op == "*=")
    return env.assign(left->name, new EvalNumber(leftNumber * rightNumber));
  else
    return env.assign(left->name, new EvalNumber(leftNumber / rightNumber));
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

EvalValue *VariableStatementNode::eval(Environment &env) const {
  for (const AstNode *declaration : declarations) {
    declaration->eval(env);
  }
  return new EvalUndefined();
}

VariableDeclarationNode::VariableDeclarationNode(IdentifierNode *id,
                                                 AstNode *init) {
  type = "VariableDeclaration";
  this->id = id;
  this->init = init;
}

json VariableDeclarationNode::toJson() const {
  return json{{"type", type},
              {"id", id->toJson()},
              {"init", init ? init->toJson() : nullptr}};
}

EvalValue *VariableDeclarationNode::eval(Environment &env) const {
  env.define(id->name, init->eval(env));
  return env.lookup(id->name);
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

EvalValue *BooleanLiteralNode::eval(Environment &env) const {
  UNUSED(env);
  return new EvalBool(value);
}

NullLiteralNode::NullLiteralNode() { type = "NullLiteral"; }

json NullLiteralNode::toJson() const {
  return json{{"type", type}, {"value", nullptr}};
}

EvalValue *NullLiteralNode::eval(Environment &env) const {
  UNUSED(env);
  return new EvalNull();
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

EvalValue *LogicalExpressionNode::eval(Environment &env) const {
  if (!std::unordered_set<std::string>{"||", "&&"}.count(op)) {
    std::stringstream ss;
    ss << "Unsupported operator " << op << " for Logical Expression";
    throw std::runtime_error(ss.str());
  }

  EvalValue *leftValue = left->eval(env);
  long long leftNumber;
  if (auto *leftValueNumber = dynamic_cast<EvalNumber *>(leftValue)) {
    leftNumber = leftValueNumber->getValue();
  } else if (auto *leftValueBool = dynamic_cast<EvalBool *>(leftValue)) {
    leftNumber = leftValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Left hand side of operation " << op
       << " is not a number or boolean: " << leftValue->typeStr() << ": "
       << leftValue->str();
    throw TypeError(ss.str());
  }

  EvalValue *rightValue = right->eval(env);
  long long rightNumber;
  if (auto *rightValueNumber = dynamic_cast<EvalNumber *>(rightValue)) {
    rightNumber = rightValueNumber->getValue();
  } else if (auto *rightValueBool = dynamic_cast<EvalBool *>(rightValue)) {
    rightNumber = rightValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Right hand side of operation " << op
       << " is not a number or boolean: " << rightValue->typeStr() << ": "
       << rightValue->str();
    throw TypeError(ss.str());
  }

  if (op == "||")
    return new EvalBool(leftNumber || rightNumber);
  else
    return new EvalBool(leftNumber && rightNumber);
}

UnaryExpressionNode::UnaryExpressionNode(std::string op, AstNode *argument) {
  type = "UnaryExpression";
  this->op = op;
  this->argument = argument;
}

json UnaryExpressionNode::toJson() const {
  return json{
      {"type", type}, {"operator", op}, {"argument", argument->toJson()}};
}

EvalValue *UnaryExpressionNode::eval(Environment &env) const {
  if (!std::unordered_set<std::string>{"!", "+", "-"}.count(op)) {
    std::stringstream ss;
    ss << "Unsupported operator " << op << " for Unary Expression";
    throw std::runtime_error(ss.str());
  }

  EvalValue *argValue = argument->eval(env);
  long long argNumber;
  if (auto *argValueNumber = dynamic_cast<EvalNumber *>(argValue)) {
    argNumber = argValueNumber->getValue();
  } else if (auto *argValueBool = dynamic_cast<EvalBool *>(argValue)) {
    argNumber = argValueBool->getValue();
  } else {
    std::stringstream ss;
    ss << "Left hand side of operation " << op
       << " is not a number or boolean: " << argValue->typeStr() << ": "
       << argValue->str();
    throw TypeError(ss.str());
  }

  if (op == "!")
    return new EvalBool(!argNumber);
  else if (op == "+")
    return new EvalNumber(argNumber);
  else
    return new EvalNumber(-argNumber);
}

WhileStatementNode::WhileStatementNode(AstNode *test, AstNode *body) {
  type = "WhileStatement";
  this->test = test;
  this->body = body;
}

json WhileStatementNode::toJson() const {
  return json{{"type", type},
              {"test", test->toJson()},
              {"body", body ? body->toJson() : nullptr}};
}

DoWhileStatementNode::DoWhileStatementNode(AstNode *test, AstNode *body) {
  type = "DoWhileStatement";
  this->test = test;
  this->body = body;
}

json DoWhileStatementNode::toJson() const {
  return json{{"type", type},
              {"test", test->toJson()},
              {"body", body ? body->toJson() : nullptr}};
}

ForStatementNode::ForStatementNode(AstNode *init, AstNode *test,
                                   AstNode *update, AstNode *body) {
  type = "ForStatement";
  this->init = init;
  this->test = test;
  this->update = update;
  this->body = body;
}

json ForStatementNode::toJson() const {
  return json{{"type", type},
              {"init", init ? init->toJson() : nullptr},
              {"test", test ? test->toJson() : nullptr},
              {"update", update ? update->toJson() : nullptr},
              {"body", body ? body->toJson() : nullptr}};
}
