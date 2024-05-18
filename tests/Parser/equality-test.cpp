#include <functional>
#include <string>

#include <json.hpp>

using json = nlohmann::json;

void EqualityTest(std::function<void(std::string, json)> test) {
  test(R"(x > 0 == true;)",
       json{
           {"type", "Program"},
           {
               "body",
               {
                   {{"type", "ExpressionStatement"},
                    {"expression",
                     {
                         {"type", "BinaryExpression"},
                         {"operator", "=="},
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
                             {{"type", "BooleanLiteral"}, {"value", true}},
                         },
                     }}},
               },
           },
       });

  test(R"(x >= 0 != false;)",
       json{
           {"type", "Program"},
           {
               "body",
               {
                   {{"type", "ExpressionStatement"},
                    {"expression",
                     {
                         {"type", "BinaryExpression"},
                         {"operator", "!="},
                         {
                             "left",
                             {
                                 {"type", "BinaryExpression"},
                                 {"operator", ">="},
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
                             {{"type", "BooleanLiteral"}, {"value", false}},
                         },
                     }}},
               },
           },
       });
}