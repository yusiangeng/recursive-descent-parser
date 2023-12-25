
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void UnaryTest(std::function<void(std::string, json)> test) {
  test(R"(-x;)", {
                     {"type", "Program"},
                     {
                         "body",
                         {
                             {
                                 {"type", "ExpressionStatement"},
                                 {
                                     "expression",
                                     {
                                         {"type", "UnaryExpression"},
                                         {"operator", "-"},
                                         {
                                             "argument",
                                             {
                                                 {"type", "Identifier"},
                                                 {"name", "x"},
                                             },
                                         },
                                     },
                                 },
                             },
                         },
                     },
                 });

  // chained unary operator
  test(R"(-+x;)",
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
                               {"type", "UnaryExpression"},
                               {"operator", "-"},
                               {
                                   "argument",
                                   {
                                       {"type", "UnaryExpression"},
                                       {"operator", "+"},
                                       {
                                           "argument",
                                           {
                                               {"type", "Identifier"},
                                               {"name", "x"},
                                           },
                                       },
                                   },
                               },
                           },
                       },
                   },
               },
           },
       });

  test(R"(!x;)",
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
                               {"type", "UnaryExpression"},
                               {"operator", "!"},
                               {
                                   "argument",
                                   {{"type", "Identifier"}, {"name", "x"}},
                               },
                           },
                       },
                   },
               },
           },
       });

  // precedence
  test(R"(+x * -10;)",
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
                               {"type", "BinaryExpression"},
                               {"operator", "*"},
                               {
                                   "left",
                                   {
                                       {"type", "UnaryExpression"},
                                       {"operator", "+"},
                                       {
                                           "argument",
                                           {
                                               {"type", "Identifier"},
                                               {"name", "x"},
                                           },
                                       },
                                   },
                               },
                               {
                                   "right",
                                   {
                                       {"type", "UnaryExpression"},
                                       {"operator", "-"},
                                       {
                                           "argument",
                                           {
                                               {"type", "NumericLiteral"},
                                               {"value", 10}
                                           },
                                       },
                                   },
                               },
                           },
                       },
                   },
               },
           },
       });
}