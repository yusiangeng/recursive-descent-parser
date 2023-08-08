
#include "Parser.h"

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "AstNode.h"
#include "SyntaxError.h"
#include "Tokenizer.h"

Ast Parser::parse(std::string str) {
  _string = str;
  _tokenizer.init(str);

  // Prime the tokenizer to obtain the first token which is our lookahead.
  _lookahead = _tokenizer.getNextToken();

  // Parse recursively starting from the main entry point, the Program:
  return Ast{Program()};
}

AstNode *Parser::Program() { return new ProgramNode(StatementList()); }

std::vector<AstNode *> Parser::StatementList(TokenType stopLookahead) {
  std::vector<AstNode *> statementList{Statement()};

  while (_lookahead && _lookahead->type != stopLookahead) {
    statementList.push_back(Statement());
  }

  return statementList;
}

AstNode *Parser::Statement() {
  switch (_lookahead->type) {
    case TokenType::Semicolon:
      return EmptyStatement();
    case TokenType::CurlyOpen:
      return BlockStatement();
    default:
      return ExpressionStatement();
  }
}

AstNode *Parser::EmptyStatement() {
  _eat(TokenType::Semicolon);
  return new EmptyStatementNode();
}

AstNode *Parser::BlockStatement() {
  _eat(TokenType::CurlyOpen);

  std::vector<AstNode *> statementList{};

  if (_lookahead->type != TokenType::CurlyClose) {
    statementList = StatementList(TokenType::CurlyClose);
  }

  _eat(TokenType::CurlyClose);

  return new BlockStatementNode(statementList);
}

AstNode *Parser::ExpressionStatement() {
  AstNode *expression = Expression();
  _eat(TokenType::Semicolon);
  return new ExpressionStatementNode(expression);
}

AstNode *Parser::Expression() { return AdditiveExpression(); }

AstNode *Parser::AdditiveExpression() {
  return _BinaryExpression(std::bind(&Parser::MultiplicativeExpression, this),
                           TokenType::AdditiveOperator);
}

AstNode *Parser::MultiplicativeExpression() {
  return _BinaryExpression(std::bind(&Parser::PrimaryExpression, this),
                           TokenType::MultiplicativeOperator);
}

AstNode *Parser::_BinaryExpression(std::function<AstNode *()> builder,
                                   TokenType operatorToken) {
  AstNode *left = builder();

  while (_lookahead->type == operatorToken) {
    // Operator: *, /
    std::string op = _eat(operatorToken).value;

    AstNode *right = builder();

    left = new BinaryExpressionNode(op, left, right);
  }

  return left;
}

AstNode *Parser::PrimaryExpression() {
  switch (_lookahead->type) {
    case TokenType::ParenthesesOpen:
      return ParenthesizedExpression();
    default:
      return Literal();
  }
}

AstNode *Parser::ParenthesizedExpression() {
  _eat(TokenType::ParenthesesOpen);
  AstNode *expression = Expression();
  _eat(TokenType::ParenthesesClose);
  return expression;
}

AstNode *Parser::Literal() {
  switch (_lookahead->type) {
    case TokenType::Number:
      return NumericLiteral();
    case TokenType::String:
      return StringLiteral();
    default:
      throw SyntaxError("Literal: unexpected literal production");
  }
}

AstNode *Parser::StringLiteral() {
  Token token = _eat(TokenType::String);

  return new StringLiteralNode(token.value.substr(1, token.value.length() - 2));
}

AstNode *Parser::NumericLiteral() {
  Token token = _eat(TokenType::Number);

  return new NumericLiteralNode(std::stoll(token.value));
}

void to_json(json &j, const Ast &parseResult) {
  j = parseResult.root->toJson();
}

Token Parser::_eat(TokenType tokenType) {
  std::optional<Token> token = _lookahead;

  if (!token) {
    throw SyntaxError("Unexpected end of input, expected: \"" +
                      tokenTypeStringMap[tokenType] + "\"");
  }

  if (token->type != tokenType) {
    throw SyntaxError("Unexpected token: \"" + token->value +
                      "\", expected: \"" + tokenTypeStringMap[tokenType] +
                      "\"");
  }

  // std::cout << "ate token " << token->value << std::endl;

  // Advance to next token.
  _lookahead = _tokenizer.getNextToken();

  return *token;
}