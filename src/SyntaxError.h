#pragma once
#include <string>

class SyntaxError : public std::runtime_error {
 public:
  SyntaxError(const std::string& what = "");
};