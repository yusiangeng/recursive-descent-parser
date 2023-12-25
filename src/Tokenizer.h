#pragma once
#include <ctype.h>

#include <optional>
#include <string>
#include <unordered_map>

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
  LogicalNot
};

extern std::unordered_map<TokenType, std::string> tokenTypeStringMap;

class Token {
 public:
  TokenType type;
  std::string value;

  Token(TokenType type, std::string value);
};

/**
 * Tokenizer class.
 *
 * Lazily pulls a token from a stream.
 */
class Tokenizer {
 public:
  std::string _string;
  long long _cursor;

  /**
   * Initializes the string.
   */
  void init(std::string str);

  /**
   * Whether the tokenizer has reached EOF.
   */
  bool isEOF();

  /**
   * Whether we still have more tokens.
   */
  bool hasMoreTokens();

  /**
   * Obtains next token.
   */
  std::optional<Token> getNextToken();

  /**
   * Matches a token for a regular expression.
   */
  std::optional<std::string> _match(std::string regexpStr, std::string string);
};
