#include "Environment.h"

#include <sstream>
#include <string>

#include "Error.h"

void Environment::define(const std::string& name, EvalValue* value) {
  record[name] = value;
}

EvalValue* Environment::lookup(const std::string& name) {
  if (!record.count(name)) {
    std::stringstream ss;
    ss << "Variable \"" << name << "\" is not defined";
    throw ReferenceError(ss.str());
  }
  return record[name];
}
