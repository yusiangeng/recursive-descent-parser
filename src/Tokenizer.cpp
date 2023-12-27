#include "Tokenizer.h"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#include "SyntaxError.h"

/**
 * Tokenizer spec.
 *
 * In order of precedence.
 */
static std::vector<std::pair<std::string, TokenType>> Spec{
    // ----------------------------------------------
    // Whitespace:
    {"^[ \t\r\n\f]+", TokenType::IgnoreToken},

    // ----------------------------------------------
    // Comments:

    // Skip single-line comments:
    {"^//.*", TokenType::IgnoreToken},

    // Skip multi-line comments:
    {"^/\\*(.|\n)*\\*/", TokenType::IgnoreToken},

    // ----------------------------------------------
    // Symbols, delimiters:
    {"^;", TokenType::Semicolon},
    {"^\\{", TokenType::CurlyOpen},
    {"^\\}", TokenType::CurlyClose},
    {"^\\(", TokenType::ParenthesesOpen},
    {"^\\)", TokenType::ParenthesesClose},
    {"^,", TokenType::Comma},

    // ----------------------------------------------
    // Keywords:
    {"^\\blet\\b", TokenType::Let},
    {"^\\bif\\b", TokenType::If},
    {"^\\belse\\b", TokenType::Else},
    {"^\\btrue\\b", TokenType::True},
    {"^\\bfalse\\b", TokenType::False},
    {"^\\bnull\\b", TokenType::NullSymbol},
    {"^\\bwhile\\b", TokenType::While},
    {"^\\bdo\\b", TokenType::Do},
    {"^\\bfor\\b", TokenType::For},

    // ----------------------------------------------
    // Numbers:
    {"^[0-9]+", TokenType::Number},

    // ----------------------------------------------
    // Identifiers:
    {"^[a-zA-Z0-9_]+", TokenType::Identifier},

    // ----------------------------------------------
    // Equality operators: ==, !=
    {"^[=!]=", TokenType::EqualityOperator},

    // ----------------------------------------------
    // Assignment operators: =, *=, /=, +=, -=
    {"^=", TokenType::AssignSimple},
    {"^[\\*/\\+-]=", TokenType::AssignComplex},

    // ----------------------------------------------
    // Math operators: +, -, *, /
    {"^[+-]", TokenType::AdditiveOperator},
    {"^[*/]", TokenType::MultiplicativeOperator},

    // ----------------------------------------------
    // Relational operators: >, >=, <, <=
    {"^[><]=?", TokenType::RelationalOperator},

    // ----------------------------------------------
    // Logical operators: &&, ||, !
    {"^&&", TokenType::LogicalAnd},
    {"^\\|\\|", TokenType::LogicalOr},
    {"^!", TokenType::LogicalNot},

    // ----------------------------------------------
    // Strings:
    {"^\"[^\"]*\"", TokenType::String},
    {"^'[^']*'", TokenType::String}};

std::unordered_map<TokenType, std::string> tokenTypeStringMap{
    {TokenType::Semicolon, ";"},
    {TokenType::CurlyOpen, "{"},
    {TokenType::CurlyClose, "}"},
    {TokenType::ParenthesesOpen, "("},
    {TokenType::ParenthesesClose, ")"},
    {TokenType::Comma, ","},
    {TokenType::AdditiveOperator, "ADDITIVE_OPERATOR"},
    {TokenType::MultiplicativeOperator, "MULTIPLICATIVE_OPERATOR"},
    {TokenType::Number, "NUMBER"},
    {TokenType::String, "STRING"},
    {TokenType::Identifier, "IDENTIFIER"},
    {TokenType::AssignSimple, "SIMPLE_ASSIGN"},
    {TokenType::AssignComplex, "COMPLEX_ASSIGN"},
    {TokenType::Let, "let"},
    {TokenType::If, "if"},
    {TokenType::Else, "else"},
    {TokenType::RelationalOperator, "RELATIONAL_OPERATOR"},
};

Token::Token(TokenType type, std::string value) {
  this->type = type;
  this->value = value;
}

void Tokenizer::init(std::string str) {
  _string = str;
  _cursor = 0;
}

bool Tokenizer::isEOF() { return _cursor == _string.length(); }

bool Tokenizer::hasMoreTokens() { return _cursor < _string.length(); }

std::optional<Token> Tokenizer::getNextToken() {
  if (!hasMoreTokens()) {
    return {};
  }

  std::string string = _string.substr(_cursor);

  for (auto [regexStr, tokenType] : Spec) {
    std::optional<std::string> tokenValue = _match(regexStr, string);

    // Couldn't match this rule, continue.
    if (!tokenValue) {
      continue;
    }

    // Should skip token, e.g. whitespace.
    if (tokenType == TokenType::IgnoreToken) {
      return getNextToken();
    }

    return Token(tokenType, *tokenValue);
  }

  throw SyntaxError(std::string("Unexpected token: \"") + string[0] +
                    std::string("\""));
}

std::optional<std::string> Tokenizer::_match(std::string regexpStr,
                                             std::string string) {
  std::smatch match;
  if (std::regex_search(string, match, std::regex(regexpStr))) {
    _cursor += match[0].length();
    return match[0];
  }
  return {};
}