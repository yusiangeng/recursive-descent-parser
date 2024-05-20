#include <sstream>

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"

#define CHECK_NUMBER(value, expected) \
  checkNumber(value, expected, __FILE__, __LINE__);
void checkNumber(EvalValue* value, long long expected, const char* file,
                 int line) {
  if (auto* valueNumber = dynamic_cast<EvalNumber*>(value)) {
    if (valueNumber->getValue() != expected) {
      std::stringstream ss;
      ss << file << ":" << line
         << ": checkNumber failed: " << valueNumber->str()
         << ", expected: " << expected;
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << file << ":" << line << ": checkNumber failed, " << value->str()
       << " of type " << value->typeStr()
       << " is not a number, expected: " << expected;
    throw std::runtime_error(ss.str());
  }
}

#define CHECK_STRING(value, expected) \
  checkString(value, expected, __FILE__, __LINE__);
void checkString(EvalValue* value, const std::string& expected,
                 const char* file, int line) {
  if (auto* valueString = dynamic_cast<EvalString*>(value)) {
    if (valueString->getValue() != expected) {
      std::stringstream ss;
      ss << file << ":" << line
         << ": checkString failed: " << valueString->str() << ", expected: \""
         << expected << "\"";
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << file << ":" << line << ": checkString failed, " << value->str()
       << " of type " << value->typeStr() << " is not a string, expected: \""
       << expected << "\"";
    throw std::runtime_error(ss.str());
  }
}

#define CHECK_BOOL(value, expected) \
  checkBool(value, expected, __FILE__, __LINE__);
void checkBool(EvalValue* value, bool expected, const char* file, int line) {
  if (auto* valueBool = dynamic_cast<EvalBool*>(value)) {
    if (valueBool->getValue() != expected) {
      std::stringstream ss;
      ss << file << ":" << line << ": checkBool failed: " << valueBool->str()
         << ", expected: " << std::boolalpha << expected;
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << file << ":" << line << ": checkBool failed, " << value->str()
       << " of type " << value->typeStr()
       << " is not a boolean, expected: " << std::boolalpha << expected;
    throw std::runtime_error(ss.str());
  }
}

#define CHECK_NULL(value) checkNull(value, __FILE__, __LINE__);
void checkNull(EvalValue* value, const char* file, int line) {
  auto* valueNull = dynamic_cast<EvalNull*>(value);
  if (!valueNull) {
    std::stringstream ss;
    ss << file << ":" << line << "checkNull failed, " << value->str()
       << " of type " << value->typeStr() << " is not null";
    throw std::runtime_error(ss.str());
  }
}

#define CHECK_UNDEFINED(value) checkUndefined(value, __FILE__, __LINE__);
void checkUndefined(EvalValue* value, const char* file, int line) {
  auto* valueUndefined = dynamic_cast<EvalUndefined*>(value);
  if (!valueUndefined) {
    std::stringstream ss;
    ss << file << ":" << line << ": checkUndefined failed, " << value->str()
       << " of type " << value->typeStr() << " is not undefined";
    throw std::runtime_error(ss.str());
  }
}

#define CHECK_EVAL_ERROR(evaluator, node, errType) \
  checkEvalError<errType>(evaluator, node, __FILE__, __LINE__);
template <typename ErrorType>
void checkEvalError(Evaluator& evaluator, AstNode* node, const char* file,
                    int line) {
  EvalValue* value = nullptr;
  try {
    value = evaluator.eval(node);
  } catch (ErrorType& e) {
  }
  if (value) {
    std::stringstream ss;
    ss << file << ":" << line
       << ": checkEvalError failed, error not thrown for " << value->str();
    throw std::runtime_error(ss.str());
  }
}

int main() {
  Evaluator evaluator;

  // Literals
  AstNode* node = new NumericLiteralNode(431);
  CHECK_NUMBER(evaluator.eval(node), 431);

  node = new NumericLiteralNode(-10000);
  CHECK_NUMBER(evaluator.eval(node), -10000);

  node = new StringLiteralNode("  Hello hi   haha!");
  CHECK_STRING(evaluator.eval(node), "  Hello hi   haha!");

  node = new NullLiteralNode();
  CHECK_NULL(evaluator.eval(node));

  node = new BooleanLiteralNode(true);
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BooleanLiteralNode(false);
  CHECK_BOOL(evaluator.eval(node), false);

  // EmptyStatement
  node = new EmptyStatementNode();
  CHECK_UNDEFINED(evaluator.eval(node));

  // UnaryExpression
  node = new UnaryExpressionNode("-", new NumericLiteralNode(1));
  CHECK_NUMBER(evaluator.eval(node), -1);

  node = new UnaryExpressionNode("+", new NumericLiteralNode(-1));
  CHECK_NUMBER(evaluator.eval(node), -1);

  node = new UnaryExpressionNode("!", new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new BooleanLiteralNode(false));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(1));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(0));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(2));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new UnaryExpressionNode("!", new NumericLiteralNode(-1));
  CHECK_BOOL(evaluator.eval(node), false);

  // BinaryExpression: math
  node = new BinaryExpressionNode(
      "*",
      new BinaryExpressionNode("+", new NumericLiteralNode(1),
                               new NumericLiteralNode(5)),
      new NumericLiteralNode(4));
  CHECK_NUMBER(evaluator.eval(node), 24);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(93),
                                  new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, DivideZeroError);

  node = new BinaryExpressionNode("-", new NumericLiteralNode(93),
                                  new NumericLiteralNode(4));
  CHECK_NUMBER(evaluator.eval(node), 89);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new NumericLiteralNode(3));
  CHECK_NUMBER(evaluator.eval(node), 4);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new StringLiteralNode("hi"));
  CHECK_EVAL_ERROR(evaluator, node, TypeError);

  // BinaryExpression: equality/relational
  node = new BinaryExpressionNode("==", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new BinaryExpressionNode("!=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  CHECK_BOOL(evaluator.eval(node), true);

  // LogicalExpression
  node = new LogicalExpressionNode("||", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), true);

  node = new LogicalExpressionNode("&&", new BooleanLiteralNode(false),
                                   new BooleanLiteralNode(true));
  CHECK_BOOL(evaluator.eval(node), false);

  // variable declaration and access
  node = new VariableStatementNode({new VariableDeclarationNode(
      new IdentifierNode("x"), new NumericLiteralNode(56))});
  CHECK_UNDEFINED(evaluator.eval(node));

  node = new IdentifierNode("x");
  CHECK_NUMBER(evaluator.eval(node), 56);

  node = new IdentifierNode("y");
  CHECK_EVAL_ERROR(evaluator, node, ReferenceError);

  node = new VariableStatementNode({new VariableDeclarationNode(
      new IdentifierNode("z"),
      new BinaryExpressionNode("+", new NumericLiteralNode(1),
                               new IdentifierNode("x")))});
  CHECK_UNDEFINED(evaluator.eval(node));

  node = new IdentifierNode("z");
  CHECK_NUMBER(evaluator.eval(node), 57);

  // ExpressionStatement
  node = new ExpressionStatementNode(new NumericLiteralNode(77));
  CHECK_NUMBER(evaluator.eval(node), 77);

  node = new ExpressionStatementNode(new IdentifierNode("z"));
  CHECK_NUMBER(evaluator.eval(node), 57);

  // AssignmentExpression
  node = new AssignmentExpressionNode("=", new IdentifierNode("undefined_name"),
                                      new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, ReferenceError);

  node = new AssignmentExpressionNode("=", new IdentifierNode("x"),
                                      new NumericLiteralNode(0));
  CHECK_NUMBER(evaluator.eval(node), 0);

  node = new AssignmentExpressionNode("/=", new IdentifierNode("x"),
                                      new NumericLiteralNode(0));
  CHECK_EVAL_ERROR(evaluator, node, DivideZeroError);

  // BlockStatement
  node = new BlockStatementNode(
      {new VariableStatementNode({new VariableDeclarationNode(
           new IdentifierNode("x"), new NumericLiteralNode(10))}),
       new VariableStatementNode({new VariableDeclarationNode(
           new IdentifierNode("y"), new NumericLiteralNode(20))}),
       new BinaryExpressionNode(
           "+",
           new BinaryExpressionNode("*", new IdentifierNode("x"),
                                    new IdentifierNode("y")),
           new NumericLiteralNode(30))});
  CHECK_NUMBER(evaluator.eval(node), 230);

  node = new BlockStatementNode(
      {new VariableStatementNode({new VariableDeclarationNode(
           new IdentifierNode("x"), new NumericLiteralNode(10))}),
       new BlockStatementNode(
           {new VariableStatementNode({new VariableDeclarationNode(
                new IdentifierNode("x"), new NumericLiteralNode(20))}),
            new ExpressionStatementNode(new IdentifierNode("x"))}),
       new ExpressionStatementNode(new IdentifierNode("x"))});
  CHECK_NUMBER(evaluator.eval(node), 10);

  node = new BlockStatementNode(
      {new VariableStatementNode(
           {new VariableDeclarationNode(new IdentifierNode("value"),
                                        new NumericLiteralNode(10)),
            new VariableDeclarationNode(
                new IdentifierNode("result"),
                new BlockStatementNode({
                    new VariableStatementNode({
                        new VariableDeclarationNode(
                            new IdentifierNode("x"),
                            new BinaryExpressionNode(
                                "+", new IdentifierNode("value"),
                                new NumericLiteralNode(10))),
                    }),
                    new ExpressionStatementNode(new IdentifierNode("x")),
                }))}),
       new ExpressionStatementNode(new IdentifierNode("result"))});
  CHECK_NUMBER(evaluator.eval(node), 20);

  node = new BlockStatementNode(
      {new VariableStatementNode({new VariableDeclarationNode(
           new IdentifierNode("data"), new NumericLiteralNode(10))}),
       new BlockStatementNode({
           new AssignmentExpressionNode("=", new IdentifierNode("data"),
                                        new NumericLiteralNode(100)),
       }),
       new ExpressionStatementNode(new IdentifierNode("data"))});
  CHECK_NUMBER(evaluator.eval(node), 100);

  std::cout << "All assertions passed!" << std::endl;
}
