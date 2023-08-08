#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void MathTest(std::function<void(std::string, json)> test) {
  // Addition:
  test(R"(2 + 2;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "BinaryExpression"},
                    {"operator", "+"},
                    {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}},
             }}});

  // Nested binary expressions:
  // left: 3 + 2
  // right: 2
  test(R"(3 + 2 - 2;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "BinaryExpression"},
                    {"operator", "-"},
                    {"left",
                     {{"type", "BinaryExpression"},
                      {"operator", "+"},
                      {"left", {{"type", "NumericLiteral"}, {"value", 3}}},
                      {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}},
             }}});

  test(R"(2 * 2;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "BinaryExpression"},
                    {"operator", "*"},
                    {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}},
             }}});

  test(R"(2 * 2 * 2;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "BinaryExpression"},
                    {"operator", "*"},
                    {"left",
                     {{"type", "BinaryExpression"},
                      {"operator", "*"},
                      {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                      {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}},
             }}});

  // Precedence of operations:
  test(
      R"(2 + 2 * 2;)",
      json{{"type", "Program"},
           {"body",
            {
                {{"type", "ExpressionStatement"},
                 {"expression",
                  {{"type", "BinaryExpression"},
                   {"operator", "+"},
                   {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                   {"right",
                    {{"type", "BinaryExpression"},
                     {"operator", "*"},
                     {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                     {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}}}},
            }}});

  // Precedence of operations:
  test(R"((2 + 2) * 2;)",
       json{{"type", "Program"},
            {"body",
             {
                 {{"type", "ExpressionStatement"},
                  {"expression",
                   {{"type", "BinaryExpression"},
                    {"operator", "*"},
                    {"left",
                     {{"type", "BinaryExpression"},
                      {"operator", "+"},
                      {"left", {{"type", "NumericLiteral"}, {"value", 2}}},
                      {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 2}}}}}},
             }}});
}