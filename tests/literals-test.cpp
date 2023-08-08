#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void LiteralsTest(std::function<void(std::string, json)> test) {
  // NumericLiteral
  test(R"(42;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression", {{"type", "NumericLiteral"}, {"value", 42}}}},
             }}});

  // StringLiteral
  test(R"("hello";)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "StringLiteral"}, {"value", "hello"}}}},
             }}});

  // StringLiteral
  test(R"('hello';)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "StringLiteral"}, {"value", "hello"}}}},
             }}});
}