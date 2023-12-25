#include "SyntaxError.h"

#include <iostream>
#include <string>

SyntaxError::SyntaxError(const std::string& msg) : msg(msg) {}

std::string SyntaxError::getMsg() { return msg; }
