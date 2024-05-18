add_library(Parser AstNode.cpp Parser.cpp Tokenizer.cpp Environment.cpp)
add_executable(parse main.cpp)
target_link_libraries(parse Parser)
