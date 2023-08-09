#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void VariableTest(std::function<void(std::string, json)> test) {
  // Simple variable declaration:
  test(R"(let x = 42;)", json{{"type", "Program"},
                              {"body",
                               {
                                   {{"type", "VariableStatement"},
                                    {"declarations",
                                     {
                                         {{"type", "VariableDeclaration"},
                                          {"id",
                                           {
                                               {"type", "Identifier"},
                                               {"name", "x"},
                                           }},
                                          {"init",
                                           {
                                               {"type", "NumericLiteral"},
                                               {"value", 42},
                                           }}},
                                     }}},
                               }}});

  // Variable declaration, no init:
  test(R"(let x;)", json{{"type", "Program"},
                         {
                             "body",
                             {
                                 {{"type", "VariableStatement"},
                                  {"declarations",
                                   {
                                       {{"type", "VariableDeclaration"},
                                        {"id",
                                         {
                                             {"type", "Identifier"},
                                             {"name", "x"},
                                         }},
                                        {"init", nullptr}},
                                   }}},
                             },
                         }});

  // Multiple variable declarations, no init:
  test(R"(let x, y;)", json{{"type", "Program"},
                            {
                                "body",
                                {
                                    {{"type", "VariableStatement"},
                                     {"declarations",
                                      {
                                          {{"type", "VariableDeclaration"},
                                           {"id",
                                            {
                                                {"type", "Identifier"},
                                                {"name", "x"},
                                            }},
                                           {"init", nullptr}},
                                          {{"type", "VariableDeclaration"},
                                           {"id",
                                            {
                                                {"type", "Identifier"},
                                                {"name", "y"},
                                            }},
                                           {"init", nullptr}},
                                      }}},
                                },
                            }});

  // Multiple variable declarations,:
  test(R"(let x, y = 42;)",
       json{{"type", "Program"},
            {
                "body",
                {
                    {{"type", "VariableStatement"},
                     {"declarations",
                      {
                          {{"type", "VariableDeclaration"},
                           {"id",
                            {
                                {"type", "Identifier"},
                                {"name", "x"},
                            }},
                           {"init", nullptr}},
                          {{"type", "VariableDeclaration"},
                           {"id",
                            {
                                {"type", "Identifier"},
                                {"name", "y"},
                            }},
                           {
                               "init",
                               {{"type", "NumericLiteral"}, {"value", 42}},
                           }},
                      }}},
                },
            }});
}