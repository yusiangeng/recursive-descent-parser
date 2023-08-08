#include "SyntaxError.h"

#include <iostream>
#include <string>

SyntaxError::SyntaxError(const std::string& what) : std::runtime_error(what) {}
