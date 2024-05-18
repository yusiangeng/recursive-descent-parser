
#include <functional>
#include <string>

#include <json.hpp>

using json = nlohmann::json;

void IfTest(std::function<void(std::string, json)> test) {
  // If-else:
  test(R"(

    if (x) {
      x = 1;
    } else {
      x = 2;
    }

  )",
       {
           {"type", "Program"},
           {"body",
            {
                {{"type", "IfStatement"},
                 {
                     "test",
                     {
                         {"type", "Identifier"},
                         {"name", "x"},
                     },
                 },
                 {"consequent",
                  {
                      {"type", "BlockStatement"},
                      {"body",
                       {{
                           {"type", "ExpressionStatement"},
                           {"expression",
                            {
                                {"type", "AssignmentExpression"},
                                {"operator", "="},
                                {"left",
                                 {
                                     {"type", "Identifier"},
                                     {"name", "x"},
                                 }},
                                {"right",
                                 {
                                     {"type", "NumericLiteral"},
                                     {"value", 1},
                                 }},
                            }},
                       }}},
                  }},
                 {"alternate",
                  {
                      {"type", "BlockStatement"},
                      {"body",
                       {{
                           {"type", "ExpressionStatement"},
                           {"expression",
                            {
                                {"type", "AssignmentExpression"},
                                {"operator", "="},
                                {
                                    "left",
                                    {
                                        {"type", "Identifier"},
                                        {"name", "x"},
                                    },
                                },
                                {
                                    "right",
                                    {
                                        {"type", "NumericLiteral"},
                                        {"value", 2},
                                    },
                                },
                            }},
                       }}},
                  }}},
            }},
       });

  // No else part:
  test(R"(

    if (x) {
      x = 1;
    }

  )",
       {
           {"type", "Program"},
           {"body",
            {
                {
                    {"type", "IfStatement"},
                    {
                        "test",
                        {
                            {"type", "Identifier"},
                            {"name", "x"},
                        },
                    },
                    {"consequent",
                     {
                         {"type", "BlockStatement"},
                         {"body",
                          {
                              {
                                  {"type", "ExpressionStatement"},
                                  {
                                      "expression",
                                      {
                                          {"type", "AssignmentExpression"},
                                          {"operator", "="},
                                          {"left",
                                           {
                                               {"type", "Identifier"},
                                               {"name", "x"},
                                           }},
                                          {
                                              "right",
                                              {
                                                  {"type", "NumericLiteral"},
                                                  {"value", 1},
                                              },
                                          },
                                      },
                                  },
                              },
                          }},
                     }},
                    {"alternate", nullptr},
                },
            }},
       });

  // No block:
  test(R"(

    if (x) x = 1;

  )",
       {
           {"type", "Program"},
           {"body",
            {
                {
                    {"type", "IfStatement"},
                    {
                        "test",
                        {
                            {"type", "Identifier"},
                            {"name", "x"},
                        },
                    },
                    {"consequent",
                     {
                         {"type", "ExpressionStatement"},
                         {"expression",
                          {
                              {"type", "AssignmentExpression"},
                              {"operator", "="},
                              {"left", {{"type", "Identifier"}, {"name", "x"}}},
                              {"right",
                               {{"type", "NumericLiteral"}, {"value", 1}}},
                          }},
                     }},
                    {"alternate", nullptr},
                },
            }},
       });

  // Nested if:
  test(
      R"(

    if (x) if (y) {} else {}

  )",
      {
          {"type", "Program"},
          {
              "body",
              {
                  {
                      {"type", "IfStatement"},
                      {"test",
                       {
                           {"type", "Identifier"},
                           {"name", "x"},
                       }},
                      {"consequent",
                       {
                           {"type", "IfStatement"},
                           {"test", {{"type", "Identifier"}, {"name", "y"}}},
                           {"consequent",
                            {
                                {"type", "BlockStatement"},
                                {"body", json::array()},
                            }},
                           {"alternate",
                            {
                                {"type", "BlockStatement"},
                                {"body", json::array()},
                            }},
                       }},
                      {"alternate", nullptr},
                  },
              },
          },
      });

  // Nested if:
  test(
      R"(

    if (x) if (y) {} else {} else {}

  )",
      {
          {"type", "Program"},
          {
              "body",
              {
                  {
                      {"type", "IfStatement"},
                      {"test",
                       {
                           {"type", "Identifier"},
                           {"name", "x"},
                       }},
                      {"consequent",
                       {
                           {"type", "IfStatement"},
                           {"test", {{"type", "Identifier"}, {"name", "y"}}},
                           {"consequent",
                            {
                                {"type", "BlockStatement"},
                                {"body", json::array()},
                            }},
                           {"alternate",
                            {
                                {"type", "BlockStatement"},
                                {"body", json::array()},
                            }},
                       }},
                      {"alternate",
                       {
                           {"type", "BlockStatement"},
                           {"body", json::array()},
                       }},
                  },
              },
          },
      });
}