#pragma once

#include <sstream>
#include <string>

class EvalValue {
 public:
  virtual std::string str() const = 0;
  std::string typeStr() const { return type; }

 protected:
  EvalValue(std::string type) : type(type) {}

  std::string type;
};

class EvalUndefined : public EvalValue {
 public:
  EvalUndefined() : EvalValue("undefined") {}

  std::string str() const override { return "undefined"; }
};

class EvalNull : public EvalValue {
 public:
  EvalNull() : EvalValue("null") {}

  std::string str() const override { return "null"; }
};

class EvalNumber : public EvalValue {
 public:
  EvalNumber(long long value) : EvalValue("number"), value(value) {}

  long long getValue() const { return value; }
  std::string str() const override { return std::to_string(value); }

 private:
  long long value;
};

class EvalString : public EvalValue {
 public:
  EvalString(const std::string& value) : EvalValue("string"), value(value) {}

  const std::string& getValue() const { return value; }
  std::string str() const override { return "\"" + value + "\""; }

 private:
  std::string value;
};

class EvalBool : public EvalValue {
 public:
  EvalBool(bool value) : EvalValue("boolean"), value(value) {}

  bool getValue() const { return value; }
  std::string str() const override { return value ? "true" : "false"; }

 private:
  bool value;
};
