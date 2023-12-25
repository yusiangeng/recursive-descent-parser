#pragma once
#include <string>

class SyntaxError {
 public:
  SyntaxError(const std::string& msg = "");
  std::string getMsg();

 private:
  std::string msg;
};