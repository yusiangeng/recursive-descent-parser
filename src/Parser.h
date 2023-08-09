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
   *  | VariableStatement
   *  ;
   */
  AstNode *Statement();

  /**
   * VariableStatement
   *  : 'let' VariableDeclarationList ';'
   *  ;
   */
  AstNode *VariableStatement();

  /**
   * VariableDeclarationList
   *  : VariableDeclaration
   *  | VariableDeclarationList ',' VariableDeclaration
   *  ;
   */
  std::vector<AstNode *> VariableDeclarationList();

  /**
   * VariableDeclaration
   *  : Identifier OptVariableInitializer
   *  ;
   */
  AstNode *VariableDeclaration();

  /**
   * VariableInitializer
   *  : SIMPLE_ASSIGN AssignmentExpression
   *  ;
   */
  AstNode *VariableInitializer();

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
   * AssignmentExpression
   *  : AdditiveExpression
   *  | LeftHandSideExpression AssignmentOperator AssignmentExpression
   *  ;
   */
  AstNode *AssignmentExpression();

  /**
   * LeftHandSideExpression
   *  : Identifier
   *  ;
   */
  AstNode *LeftHandSideExpression();

  /**
   * Identifier
   *  : IDENTIFIER
   *  ;
   */
  AstNode *Identifier();

  /**
   * Extra check whether it's valid assignment target.
   */
  AstNode *_checkValidAssignmentTarget(AstNode *node);

  /**
   * Whether the token is an assignment operator.
   */
  bool _isAssignmentOperator(TokenType tokenType);

  /**
   * AssignmentOperator
   *  : SIMPLE_ASSIGN
   *  | COMPLEX_ASSIGN
   *  ;
   */
  Token AssignmentOperator();

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
   *  | LeftHandSideExpression
   *  ;
   */
  AstNode *PrimaryExpression();

  /**
   * Whether the token is a literal.
   */
  bool _isLiteral(TokenType tokenType);

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