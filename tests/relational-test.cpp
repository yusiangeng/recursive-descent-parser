
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
}