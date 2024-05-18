#include <sstream>

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"

void checkNumber(EvalValue* value, long long expected) {
  if (auto* valueNumber = dynamic_cast<EvalNumber*>(value)) {
    if (valueNumber->getValue() != expected) {
      std::stringstream ss;
      ss << "checkNumber failed: " << valueNumber->str()
         << ", expected: " << expected;
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << "checkNumber failed, " << value->str() << " of type "
       << value->typeStr() << " is not a number, expected: " << expected;
    throw std::runtime_error(ss.str());
  }
}

void checkString(EvalValue* value, const std::string& expected) {
  if (auto* valueString = dynamic_cast<EvalString*>(value)) {
    if (valueString->getValue() != expected) {
      std::stringstream ss;
      ss << "checkString failed: " << valueString->str() << ", expected: \""
         << expected << "\"";
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << "checkString failed, " << value->str() << " of type "
       << value->typeStr() << " is not a string, expected: \"" << expected
       << "\"";
    throw std::runtime_error(ss.str());
  }
}

void checkBool(EvalValue* value, bool expected) {
  if (auto* valueBool = dynamic_cast<EvalBool*>(value)) {
    if (valueBool->getValue() != expected) {
      std::stringstream ss;
      ss << "checkBool failed: " << valueBool->str()
         << ", expected: " << std::boolalpha << expected;
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << "checkBool failed, " << value->str() << " of type "
       << value->typeStr() << " is not a boolean, expected: " << std::boolalpha
       << expected;
    throw std::runtime_error(ss.str());
  }
}

void checkNull(EvalValue* value) {
  auto* valueNull = dynamic_cast<EvalNull*>(value);
  if (!valueNull) {
    std::stringstream ss;
    ss << "checkNull failed, " << value->str() << " of type "
       << value->typeStr() << " is not null";
    throw std::runtime_error(ss.str());
  }
}

void checkUndefined(EvalValue* value) {
  auto* valueUndefined = dynamic_cast<EvalUndefined*>(value);
  if (!valueUndefined) {
    std::stringstream ss;
    ss << "checkUndefined failed, " << value->str() << " of type "
       << value->typeStr() << " is not undefined";
    throw std::runtime_error(ss.str());
  }
}

template <typename ErrorType>
void checkEvalError(Evaluator& evaluator, AstNode* node) {
  EvalValue* value = nullptr;
  try {
    value = evaluator.eval(node);
  } catch (ErrorType& e) {
  }
  if (value)
    throw std::runtime_error("checkEvalError failed for " + value->str());
}

int main() {
  Evaluator evaluator;

  // Literals
  AstNode* node = new NumericLiteralNode(431);
  checkNumber(evaluator.eval(node), 431);

  node = new StringLiteralNode("  Hello hi   haha!");
  checkString(evaluator.eval(node), "  Hello hi   haha!");

  node = new NullLiteralNode();
  checkNull(evaluator.eval(node));

  node = new BooleanLiteralNode(false);
  checkBool(evaluator.eval(node), false);

  // BinaryExpression: math
  node = new BinaryExpressionNode(
      "*",
      new BinaryExpressionNode("+", new NumericLiteralNode(1),
                               new NumericLiteralNode(5)),
      new NumericLiteralNode(4));
  checkNumber(evaluator.eval(node), 24);

  node = new BinaryExpressionNode("-", new NumericLiteralNode(93),
                                  new NumericLiteralNode(4));
  checkNumber(evaluator.eval(node), 89);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new NumericLiteralNode(3));
  checkNumber(evaluator.eval(node), 4);

  node = new BinaryExpressionNode("/", new NumericLiteralNode(13),
                                  new StringLiteralNode("hi"));
  checkEvalError<TypeError>(evaluator, node);

  // BinaryExpression: equality/relational
  node = new BinaryExpressionNode("==", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  checkBool(evaluator.eval(node), true);

  node = new BinaryExpressionNode("!=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  checkBool(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  checkBool(evaluator.eval(node), false);

  node = new BinaryExpressionNode("<=", new NumericLiteralNode(3),
                                  new NumericLiteralNode(3));
  checkBool(evaluator.eval(node), true);

  // variable declaration and access
  node = new VariableStatementNode({new VariableDeclarationNode(
      new IdentifierNode("x"), new NumericLiteralNode(56))});
  checkUndefined(evaluator.eval(node));

  node = new IdentifierNode("x");
  checkNumber(evaluator.eval(node), 56);

  node = new IdentifierNode("y");
  checkEvalError<ReferenceError>(evaluator, node);

  node = new VariableStatementNode({new VariableDeclarationNode(
      new IdentifierNode("z"),
      new BinaryExpressionNode("+", new NumericLiteralNode(1),
                               new IdentifierNode("x")))});
  checkUndefined(evaluator.eval(node));

  node = new IdentifierNode("z");
  checkNumber(evaluator.eval(node), 57);
  std::cout << "All assertions passed!" << std::endl;
}
