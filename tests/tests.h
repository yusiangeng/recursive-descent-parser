#pragma once
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void BlockTest(std::function<void(std::string, json)> test);
void EmptyStatementTest(std::function<void(std::string, json)> test);
void LiteralsTest(std::function<void(std::string, json)> test);
void StatementListTest(std::function<void(std::string, json)> test);
void MathTest(std::function<void(std::string, json)> test);
void AssignmentTest(std::function<void(std::string, json)> test);
