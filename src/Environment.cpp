#include "Environment.h"

#include <sstream>
#include <string>

#include "Error.h"

void Environment::define(const std::string& name, EvalValue* value) {
  record[name] = value;
}

EvalValue* Environment::lookup(const std::string& name) {
  return resolve(name)->record[name];
}

EvalValue* Environment::assign(const std::string& name, EvalValue* value) {
  resolve(name)->record[name] = value;
  return value;
}

Environment* Environment::resolve(const std::string& name) {
  if (record.count(name)) {
    return this;
  }
  if (!parent) {
    std::stringstream ss;
    ss << "Variable \"" << name << "\" is not defined";
    throw ReferenceError(ss.str());
  }
  return parent->resolve(name);
}
