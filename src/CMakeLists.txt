add_library(Parser AstNode.cpp Parser.cpp SyntaxError.cpp Tokenizer.cpp)
add_executable(parse main.cpp)
target_link_libraries(parse Parser)
