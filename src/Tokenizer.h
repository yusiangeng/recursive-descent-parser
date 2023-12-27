#pragma once
#include <ctype.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {
  IgnoreToken,
  Semicolon,
  CurlyOpen,
  CurlyClose,
  ParenthesesOpen,
  ParenthesesClose,
  AdditiveOperator,
  MultiplicativeOperator,
  Number,
  String,
  Identifier,
  AssignSimple,
  AssignComplex,
  Let,
  Comma,
  If,
  Else,
  RelationalOperator,
  EqualityOperator,
  True,
  False,
  NullSymbol,
  LogicalAnd,
  LogicalOr,
  LogicalNot,
  While,
  Do,
  For
};

class Token {
 public:
  TokenType type;
  std::string value;

  Token(TokenType type, std::string value);
};

struct SpecItem {
  std::string regexString;
  TokenType tokenType;
  std::string debugString;
};

extern std::vector<SpecItem> Spec;

/**
 * Tokenizer class.
 *
 * Lazily pulls a token from a stream.
 */
class Tokenizer {
 public:
  /**
   * Initializes the string.
   */
  void init(std::string str);

  /**
   * Obtains next token.
   */
  std::optional<Token> getNextToken();

 private:
  std::string _string;
  size_t _cursor;

  /**
   * Whether the tokenizer has reached EOF.
   */
  bool isEOF();

  /**
   * Whether we still have more tokens.
   */
  bool hasMoreTokens();

  /**
   * Matches a token for a regular expression.
   */
  std::optional<std::string> _match(std::string regexpStr, std::string string);
};
