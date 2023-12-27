
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void WhileTest(std::function<void(std::string, json)> test) {
  test(
      R"(
    while (x > 10) {
      x -= 1;
    }
  )",
      {
          {"type", "Program"},
          {
              "body",
              {{
                  {"type", "WhileStatement"},
                  {"test",
                   {
                       {"type", "BinaryExpression"},
                       {"operator", ">"},
                       {
                           "left",
                           {{"type", "Identifier"}, {"name", "x"}},
                       },
                       {
                           "right",
                           {{"type", "NumericLiteral"}, {"value", 10}},
                       },
                   }},
                  {"body",
                   {
                       {"type", "BlockStatement"},
                       {"body",
                        {{
                            {"type", "ExpressionStatement"},
                            {
                                "expression",
                                {{"type", "AssignmentExpression"},
                                 {"operator", "-="},
                                 {"left",
                                  {{"type", "Identifier"}, {"name", "x"}}},
                                 {"right",
                                  {{"type", "NumericLiteral"}, {"value", 1}}}},
                            },
                        }}},
                   }},
              }},
          },
      });
}