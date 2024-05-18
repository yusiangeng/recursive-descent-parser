#include <functional>
#include <string>

#include <json.hpp>

using json = nlohmann::json;

void BlockTest(std::function<void(std::string, json)> test) {
  test(
      R"(
    
    {
      42;

      "hello";
    }

    )",
      json{{"type", "Program"},
           {"body",
            {{{"type", "BlockStatement"},
              {"body",
               {{{"type", "ExpressionStatement"},
                 {"expression", {{"type", "NumericLiteral"}, {"value", 42}}}},
                {{"type", "ExpressionStatement"},
                 {"expression",
                  {{"type", "StringLiteral"}, {"value", "hello"}}}}}}}}}});

  // Empty block:
  test(
      R"(
    
    {
      
    }

    )",
      json{{"type", "Program"},
           {"body", {{{"type", "BlockStatement"}, {"body", json::array()}}}}});

  // Nested blocks:
  test(
      R"(
    
    {
      42;
      {
        "hello";
      }
    }

    )",
      json{
          {"type", "Program"},
          {"body",
           {{{"type", "BlockStatement"},
             {"body",
              {{{"type", "ExpressionStatement"},
                {"expression", {{"type", "NumericLiteral"}, {"value", 42}}}},
               {{"type", "BlockStatement"},
                {"body",
                 {{{"type", "ExpressionStatement"},
                   {"expression",
                    {{"type", "StringLiteral"}, {"value", "hello"}}}}}}}}}}}}});
}