#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void AssignmentTest(std::function<void(std::string, json)> test) {
  // Simple assignment:
  test(R"(x = 42;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "AssignmentExpression"},
                    {"operator", "="},
                    {"left", {{"type", "Identifier"}, {"name", "x"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 42}}}}}},
             }}});

  // Chained assignment:
  test(
      R"(x = y = 42;)",
      json{
          {"type", "Program"},
          {"body",
           {
               {{"type", "ExpressionStatement"},
                {"expression",
                 {{"type", "AssignmentExpression"},
                  {"operator", "="},
                  {"left", {{"type", "Identifier"}, {"name", "x"}}},
                  {"right",
                   {{"type", "AssignmentExpression"},
                    {"operator", "="},
                    {"left", {{"type", "Identifier"}, {"name", "y"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 42}}}}}}}},
           }}});
}