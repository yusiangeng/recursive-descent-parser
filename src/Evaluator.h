#pragma once

#include "AstNode.h"
#include "Environment.h"
#include "Error.h"
#include "EvalValue.h"

class Evaluator {
 public:
  Evaluator(Environment global = Environment()) : global(global) {}

  EvalValue* eval(AstNode* node) { return eval(node, global); }

  EvalValue* eval(AstNode* node, Environment& env) { return node->eval(env); }

 private:
  Environment global;
};
