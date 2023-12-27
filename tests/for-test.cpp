
#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void ForTest(std::function<void(std::string, json)> test) {
  test(
      R"(

    for (let i = 0; i < 10; i += 1) {
      x += i;
    }

  )",
      {
          {"type", "Program"},
          {"body",
           {{
               {"type", "ForStatement"},
               {"init",
                {
                    {"type", "VariableStatement"},
                    {"declarations",
                     {{
                         {"type", "VariableDeclaration"},
                         {"id", {{"type", "Identifier"}, {"name", "i"}}},
                         {"init", {{"type", "NumericLiteral"}, {"value", 0}}},
                     }}},
                }},
               {"test",
                {
                    {"type", "BinaryExpression"},
                    {"operator", "<"},
                    {"left", {{"type", "Identifier"}, {"name", "i"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 10}}},
                }},
               {"update",
                {{"type", "AssignmentExpression"},
                 {"left", {{"type", "Identifier"}, {"name", "i"}}},
                 {"operator", "+="},
                 {"right", {{"type", "NumericLiteral"}, {"value", 1}}}}},
               {"body",
                {{"type", "BlockStatement"},
                 {
                     "body",
                     {{
                         {"type", "ExpressionStatement"},
                         {"expression",
                          {
                              {"type", "AssignmentExpression"},
                              {"left", {{"type", "Identifier"}, {"name", "x"}}},
                              {"operator", "+="},
                              {"right",
                               {{"type", "Identifier"}, {"name", "i"}}},
                          }},
                     }},
                 }}},
           }}},
      });

  // multiple init
  test(
      R"(

    for (let i = 0, z = 0; i < 10; i += 1) {
      x += i;
    }

  )",
      {
          {"type", "Program"},
          {"body",
           {{
               {"type", "ForStatement"},
               {"init",
                {
                    {"type", "VariableStatement"},
                    {"declarations",
                     {
                         {
                             {"type", "VariableDeclaration"},
                             {"id", {{"type", "Identifier"}, {"name", "i"}}},
                             {"init",
                              {{"type", "NumericLiteral"}, {"value", 0}}},
                         },
                         {
                             {"type", "VariableDeclaration"},
                             {"id", {{"type", "Identifier"}, {"name", "z"}}},
                             {"init",
                              {{"type", "NumericLiteral"}, {"value", 0}}},
                         },
                     }},
                }},
               {"test",
                {
                    {"type", "BinaryExpression"},
                    {"operator", "<"},
                    {"left", {{"type", "Identifier"}, {"name", "i"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 10}}},
                }},
               {"update",
                {{"type", "AssignmentExpression"},
                 {"left", {{"type", "Identifier"}, {"name", "i"}}},
                 {"operator", "+="},
                 {"right", {{"type", "NumericLiteral"}, {"value", 1}}}}},
               {"body",
                {{"type", "BlockStatement"},
                 {
                     "body",
                     {{
                         {"type", "ExpressionStatement"},
                         {"expression",
                          {
                              {"type", "AssignmentExpression"},
                              {"left", {{"type", "Identifier"}, {"name", "x"}}},
                              {"operator", "+="},
                              {"right",
                               {{"type", "Identifier"}, {"name", "i"}}},
                          }},
                     }},
                 }}},
           }}},
      });

  // assignment init
  test(
      R"(

    for (i = 0; i < 10; i += 1) {
      x += i;
    }

  )",
      {
          {"type", "Program"},
          {"body",
           {{
               {"type", "ForStatement"},
               {"init",
                {
                    {"type", "AssignmentExpression"},
                    {"operator", "="},
                    {"left", {{"type", "Identifier"}, {"name", "i"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 0}}},
                }},
               {"test",
                {
                    {"type", "BinaryExpression"},
                    {"operator", "<"},
                    {"left", {{"type", "Identifier"}, {"name", "i"}}},
                    {"right", {{"type", "NumericLiteral"}, {"value", 10}}},
                }},
               {"update",
                {
                    {"type", "AssignmentExpression"},
                    {"left", {{"type", "Identifier"}, {"name", "i"}}},
                    {"operator", "+="},
                    {"right", {{"type", "NumericLiteral"}, {"value", 1}}},
                }},
               {"body",
                {
                    {"type", "BlockStatement"},
                    {"body",
                     {{
                         {"type", "ExpressionStatement"},
                         {"expression",
                          {
                              {"type", "AssignmentExpression"},
                              {"left", {{"type", "Identifier"}, {"name", "x"}}},
                              {"operator", "+="},
                              {"right",
                               {{"type", "Identifier"}, {"name", "i"}}},
                          }},
                     }}},
                }},
           }}},
      });

  // empty
  test(
      R"(

    for (; ; ) { }

  )",
      {
          {"type", "Program"},
          {"body",
           {{
               {"type", "ForStatement"},
               {"init", nullptr},
               {"test", nullptr},
               {"update", nullptr},
               {"body", {{"body", json::array()}, {"type", "BlockStatement"}}},
           }}},
      });
}