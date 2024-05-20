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
  Environment(Environment* parent, const RecordType& record = RecordType{})
      : parent(parent), record(record) {}

  void define(const std::string& name, EvalValue* value);

  // Returns the value of a defined variable, or throws if name is not defined.
  EvalValue* lookup(const std::string& name);

  // Updates an existing variable.
  EvalValue* assign(const std::string& name, EvalValue* value);

 private:
  // Returns the environment in the environment chain in which a variable is
  // defined, or throws if name is not found.
  Environment* resolve(const std::string& name);

  Environment* parent;
  RecordType record;
};
