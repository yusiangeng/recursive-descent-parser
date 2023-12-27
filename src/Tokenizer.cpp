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
std::vector<SpecItem> Spec{
    // ----------------------------------------------
    // Whitespace:
    {"^[ \t\r\n\f]+", TokenType::IgnoreToken, "IGNORE_TOKEN"},

    // ----------------------------------------------
    // Comments:

    // Skip single-line comments:
    {"^//.*", TokenType::IgnoreToken, "IGNORE_TOKEN"},

    // Skip multi-line comments:
    {"^/\\*(.|\n)*\\*/", TokenType::IgnoreToken, "IGNORE_TOKEN"},

    // ----------------------------------------------
    // Symbols, delimiters:
    {"^;", TokenType::Semicolon, ";"},
    {"^\\{", TokenType::CurlyOpen, "{"},
    {"^\\}", TokenType::CurlyClose, "}"},
    {"^\\(", TokenType::ParenthesesOpen, "("},
    {"^\\)", TokenType::ParenthesesClose, ")"},
    {"^,", TokenType::Comma, ","},

    // ----------------------------------------------
    // Keywords:
    {"^\\blet\\b", TokenType::Let, "let"},
    {"^\\bif\\b", TokenType::If, "if"},
    {"^\\belse\\b", TokenType::Else, "else"},
    {"^\\btrue\\b", TokenType::True, "true"},
    {"^\\bfalse\\b", TokenType::False, "false"},
    {"^\\bnull\\b", TokenType::NullSymbol, "null"},
    {"^\\bwhile\\b", TokenType::While, "while"},
    {"^\\bdo\\b", TokenType::Do, "do"},
    {"^\\bfor\\b", TokenType::For, "for"},

    // ----------------------------------------------
    // Numbers:
    {"^[0-9]+", TokenType::Number, "NUMBER"},

    // ----------------------------------------------
    // Identifiers:
    {"^[a-zA-Z0-9_]+", TokenType::Identifier, "IDENTIFIER"},

    // ----------------------------------------------
    // Equality operators: ==, !=
    {"^[=!]=", TokenType::EqualityOperator, "EQUALITY_OPERATOR"},

    // ----------------------------------------------
    // Assignment operators: =, *=, /=, +=, -=
    {"^=", TokenType::AssignSimple, "ASSIGN_SIMPLE"},
    {"^[\\*/\\+-]=", TokenType::AssignComplex, "ASSIGN_COMPLEX"},

    // ----------------------------------------------
    // Math operators: +, -, *, /
    {"^[+-]", TokenType::AdditiveOperator, "ADDITIVE_OPERATOR"},
    {"^[*/]", TokenType::MultiplicativeOperator, "MULTIPLICATIVE_OPERATOR"},

    // ----------------------------------------------
    // Relational operators: >, >=, <, <=
    {"^[><]=?", TokenType::RelationalOperator, "RELATIONAL_OPERATOR"},

    // ----------------------------------------------
    // Logical operators: &&, ||, !
    {"^&&", TokenType::LogicalAnd, "&&"},
    {"^\\|\\|", TokenType::LogicalOr, "||"},
    {"^!", TokenType::LogicalNot, "!"},

    // ----------------------------------------------
    // Strings:
    {"^\"[^\"]*\"", TokenType::String, "STRING"},
    {"^'[^']*'", TokenType::String, "STRING"},
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

  for (const auto& [regexString, tokenType, debugString] : Spec) {
    std::optional<std::string> tokenValue = _match(regexString, string);

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