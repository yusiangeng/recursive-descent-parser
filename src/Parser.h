#pragma once
#include <optional>
#include <string>

#include "AstNode.h"
#include "Tokenizer.h"

class Ast {
 public:
  AstNode *root;
};

class Parser {
 public:
  std::string _string = "";
  Tokenizer _tokenizer;

  // The lookahead is used for predictive parsing.
  std::optional<Token> _lookahead;

  /**
   * Parses a string into an AST.
   */
  Ast parse(std::string str);

  /**
   * Expects a token of a given type.
   */
  Token _eat(TokenType tokenType);

  /**
   * Main entry point.
   *
   * Program
   *  : StatementList
   *  ;
   */
  AstNode *Program();

  /**
   * StatementList
   *  : Statement
   *  | StatementList Statement
   *  ;
   */
  std::vector<AstNode *> StatementList(
      TokenType stopLookahead = TokenType::Null);

  /**
   * Statement
   *  : ExpressionStatement
   *  | BlockStatement
   *  | EmptyStatement
   *  ;
   */
  AstNode *Statement();

  /**
   * EmptyStatement
   *  : ';'
   *  ;
   */
  AstNode *EmptyStatement();

  /**
   * BlockStatement
   *  : '{' OptStatementList '}'
   *  ;
   */
  AstNode *BlockStatement();

  /**
   * ExpressionStatement
   *  : Expression ';'
   *  ;
   */
  AstNode *ExpressionStatement();

  /**
   * Expression
   *  : AdditiveExpression
   *  ;
   */
  AstNode *Expression();

  /**
   * AdditiveExpression
   *  : MultiplicativeExpression
   *  | AdditiveExpression ADDITIVE_OPERATOR MultiplicativeExpression
   *  ;
   */
  AstNode *AdditiveExpression();

  /**
   * MultiplicativeExpression
   *  : PrimaryExpression
   *  | MultiplicativeExpression MULTIPLICATIVE_OPERATOR PrimaryExpression
   *  ;
   */
  AstNode *MultiplicativeExpression();

  /**
   * Generic binary expression.
   */
  AstNode *_BinaryExpression(std::function<AstNode *()> builder,
                             TokenType operatorToken);

  /**
   * PrimaryExpression
   *  : Literal
   *  | ParenthesizedExpression
   *  ;
   */
  AstNode *PrimaryExpression();

  /**
   * ParenthesizedExpression
   *  : '(' Expression ')'
   *  ;
   */
  AstNode *ParenthesizedExpression();

  /**
   * Literal
   *  : NumericLiteral
   *  | StringLiteral
   *  ;
   */
  AstNode *Literal();

  /**
   * StringLiteral
   *  : STRING
   *  ;
   */
  AstNode *StringLiteral();

  /**
   * NumericLiteral
   *  : NUMBER
   *  ;
   */
  AstNode *NumericLiteral();
};

// This function is required by nlohmann/json library to construct json from
// Ast.
void to_json(json &j, const Ast &parseResult);