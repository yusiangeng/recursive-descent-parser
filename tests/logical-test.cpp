
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void LogicalTest(std::function<void(std::string, json)> test) {
  test(R"(1 + 2 || false;)",
       {
           {"type", "Program"},
           {
               "body",
               {
                   {
                       {"type", "ExpressionStatement"},
                       {
                           "expression",
                           {
                               {"type", "LogicalExpression"},
                               {"operator", "||"},
                               {
                                   "left",
                                   {
                                       {"type", "BinaryExpression"},
                                       {"operator", "+"},
                                       {
                                           "left",
                                           {{"type", "NumericLiteral"},
                                            {"value", 1}},
                                       },
                                       {
                                           "right",
                                           {{"type", "NumericLiteral"},
                                            {"value", 2}},
                                       },
                                   },
                               },
                               {
                                   "right",
                                   {{"type", "BooleanLiteral"},
                                    {"value", false}},
                               },
                           },
                       },
                   },
               },
           },
       });

  test(R"(x > 0 && y < 1;)",
       {
           {"type", "Program"},
           {"body",
            {
                {
                    {"type", "ExpressionStatement"},
                    {"expression",
                     {{"type", "LogicalExpression"},
                      {"operator", "&&"},
                      {
                          "left",
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
                          },
                      },
                      {
                          "right",
                          {
                              {"type", "BinaryExpression"},
                              {"operator", "<"},
                              {"left",
                               {
                                   {"type", "Identifier"},
                                   {"name", "y"},
                               }},
                              {"right",
                               {
                                   {"type", "NumericLiteral"},
                                   {"value", 1},
                               }},
                          },
                      }}},
                },
            }},
       });
}