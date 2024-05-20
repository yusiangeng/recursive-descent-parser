/*
 * User-facing errors.
 */

#pragma once

#include <iostream>
#include <sstream>
#include <string>

// for parser
class SyntaxError : public std::runtime_error {
 public:
  SyntaxError(const std::string& msg = "")
      : std::runtime_error("SyntaxError: " + msg) {}
};

// below errors are for AST interpreter
class EvalError : public std::runtime_error {  // TODO: remove this class
 protected:
  EvalError(const std::string& msg = "") : std::runtime_error(msg) {}
};

class ReferenceError : public EvalError {
 public:
  ReferenceError(const std::string& msg = "")
      : EvalError("ReferenceError: " + msg) {}
};

class TypeError : public EvalError {
 public:
  TypeError(const std::string& msg = "") : EvalError("TypeError: " + msg) {}
};

class DivideZeroError : public EvalError {
 public:
  DivideZeroError(const std::string& msg = "")
      : EvalError("DivideZeroError: " + msg) {}
};
