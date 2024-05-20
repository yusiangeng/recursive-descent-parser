/**
 * Helper macros for checking `EvalValue`s.
 */

#pragma once

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
