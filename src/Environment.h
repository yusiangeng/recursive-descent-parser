#pragma once

#include <string>
#include <unordered_map>

#include "EvalValue.h"

/**
 * Environment: names storage.
 */
class Environment {
  using RecordType = std::unordered_map<std::string, EvalValue*>;

 public:
  Environment(const RecordType& record = RecordType{}) : record(record) {}

  void define(const std::string& name, EvalValue* value);
  EvalValue* lookup(const std::string& name);

 private:
  RecordType record;
};
