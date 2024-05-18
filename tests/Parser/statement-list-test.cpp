#include <functional>
#include <string>

#include <json.hpp>

using json = nlohmann::json;

void StatementListTest(std::function<void(std::string, json)> test) {
  test(R"(
    "hello";

    42;
    
    )",
       json{
           {"type", "Program"},
           {
               "body",
               {
                   {
                       {"type", "ExpressionStatement"},
                       {
                           "expression",
                           {{"type", "StringLiteral"}, {"value", "hello"}},
                       },
                   },
                   {
                       {"type", "ExpressionStatement"},
                       {
                           "expression",
                           {{"type", "NumericLiteral"}, {"value", 42}},
                       },
                   },
               },
           },
       });
}