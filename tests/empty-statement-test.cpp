#include <functional>
#include <string>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

void EmptyStatementTest(std::function<void(std::string, json)> test) {
  test(R"(;)", json{{"type", "Program"},
                    {"body",
                     {
                         {
                             {"type", "EmptyStatement"},
                         },
                     }}});
}