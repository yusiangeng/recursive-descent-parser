
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void RelationalTest(std::function<void(std::string, json)> test) {
  test(R"(x > 0;)", {
                        {"type", "Program"},
                        {"body",
                         {
                             {
                                 {"type", "ExpressionStatement"},
                                 {"expression",
                                  {
                                      {"type", "BinaryExpression"},
                                      {"operator", ">"},
                                      {"left",
                                       {
                                           {"type", "Identifier"},
                                           {"name", "x"},
                                       }},
                                      {"right",
                                       {
                                           {"type", "NumericLiteral"},
                                           {"value", 0},
                                       }},
                                  }},
                             },
                         }},
                    });

  test(R"(3 <= y;)", {
                         {"type", "Program"},
                         {
                             "body",
                             {
                                 {
                                     {"type", "ExpressionStatement"},
                                     {
                                         "expression",
                                         {
                                             {"type", "BinaryExpression"},
                                             {"operator", "<="},
                                             {
                                                 "left",
                                                 {
                                                     {"type", "NumericLiteral"},
                                                     {"value", 3},
                                                 },
                                             },
                                             {
                                                 "right",
                                                 {
                                                     {"type", "Identifier"},
                                                     {"name", "y"},
                                                 },
                                             },
                                         },
                                     },
                                 },
                             },
                         },
                     });
}