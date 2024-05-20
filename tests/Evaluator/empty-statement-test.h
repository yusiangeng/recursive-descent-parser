#pragma once

#include "../../src/AstNode.h"
#include "../../src/EvalValue.h"
#include "../../src/Evaluator.h"
#include "checkValue.h"

void EmptyStatementTest() {
  Evaluator evaluator;

  AstNode* node = new EmptyStatementNode();
  CHECK_UNDEFINED(evaluator.eval(node));
}
