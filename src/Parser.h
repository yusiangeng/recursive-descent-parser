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
  /**
   * Parses a string into an AST.
   */
  Ast parse(std::string str);

 private:
  std::string _string = "";
  Tokenizer _tokenizer;

  // The lookahead is used for predictive parsing.
  std::optional<Token> _lookahead;

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
      TokenType stopLookahead = TokenType::IgnoreToken);

  /**
   * Statement
   *  : ExpressionStatement
   *  | BlockStatement
   *  | EmptyStatement
   *  | VariableStatement
   *  | IfStatement
   *  | IterationStatement
   *  ;
   */
  AstNode *Statement();

  /**
   * IterationStatement
   *  : WhileStatement
   *  | DoWhileStatement
   *  | ForStatement
   *  ;
   */
  AstNode *IterationStatement();

  /**
   * WhileStatement
   *  : 'while' '(' Expression ')' Statement
   *  ;
   */
  AstNode *WhileStatement();

  /**
   * DoWhileStatement
   *  : 'do' Statement 'while' '(' Expression ')' ';'
   *  ;
   */
  AstNode *DoWhileStatement();

  /**
   * IfStatement
   *  : 'if' '(' Expression ')' Statement
   *  | 'if' '(' Expression ')' Statement 'else' Statement
   *  ;
   */
  AstNode *IfStatement();

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
   *  : LogicalORExpression
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
   * AssignmentOperator
   *  : SIMPLE_ASSIGN
   *  | COMPLEX_ASSIGN
   *  ;
   */
  Token AssignmentOperator();

  /**
   * Logical OR expression.
   *
   *  x || y
   *
   * LogicalORExpression
   *  : LogicalANDExpression LOGICAL_OR LogicalORExpression
   *  | LogicalANDExpression
   *  ;
   *
   */
  AstNode *LogicalORExpression();

  /**
   * Logical AND expression.
   *
   *  x && y
   *
   * LogicalANDExpression
   *  : EqualityExpression LOGICAL_AND LogicalANDExpression
   *  | EqualityExpression
   *  ;
   *
   */
  AstNode *LogicalANDExpression();

  /**
   * EQUALITY_OPERATOR: ==, !=
   *
   *  x == y
   *  x != y
   *
   * EqualityExpression
   *  : RelationalExpression EQUALITY_OPERATOR EqualityExpression
   *  | RelationalExpression
   *  ;
   */
  AstNode *EqualityExpression();

  /**
   * RELATIONAL_OPERATOR: >, >=, <, <=
   *
   *  x > y
   *  x >= y
   *  x < y
   *  x <= y
   *
   * RelationalExpression
   *  : AdditiveExpression
   *  | AdditiveExpression RELATIONAL_OPERATOR RelationalExpression
   *  ;
   */
  AstNode *RelationalExpression();

  /**
   * AdditiveExpression
   *  : MultiplicativeExpression
   *  | AdditiveExpression ADDITIVE_OPERATOR MultiplicativeExpression
   *  ;
   */
  AstNode *AdditiveExpression();

  /**
   * MultiplicativeExpression
   *  : UnaryExpression
   *  | MultiplicativeExpression MULTIPLICATIVE_OPERATOR UnaryExpression
   *  ;
   */
  AstNode *MultiplicativeExpression();

  /**
   * UnaryExpression
   *  : LeftHandSideExpression
   *  | ADDITIVE_OPERATOR UnaryExpression
   *  | LOGICAL_NOT UnaryExpression
   *  ;
   */
  AstNode *UnaryExpression();

  /**
   * PrimaryExpression
   *  : Literal
   *  | ParenthesizedExpression
   *  | Identifier
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
   *  | BooleanLiteral
   *  | NullLiteral
   *  ;
   */
  AstNode *Literal();

  /**
   * BooleanLiteral
   *  : 'true'
   *  | 'false'
   *  ;
   */
  AstNode *BooleanLiteral(bool value);

  /**
   * NullLiteral
   *  : 'null'
   *  ;
   */
  AstNode *NullLiteral();

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

  /**
   * Whether the token is a literal.
   */
  bool _isLiteral(TokenType tokenType);

  /**
   * Whether the token is an assignment operator.
   */
  bool _isAssignmentOperator(TokenType tokenType);

  /**
   *  Generic binary expression.
   */
  AstNode *_BinaryExpression(std::function<AstNode *()> builder,
                             TokenType operatorToken);

  /**
   * Extra check whether it's valid assignment target.
   */
  AstNode *_checkValidAssignmentTarget(AstNode *node);

  /**
   * Generic helper for LogicalExpression nodes.
   */
  AstNode *_LogicalExpression(std::function<AstNode *()> builder,
                              TokenType operatorToken);
};

// This function is required by nlohmann/json library to construct json from
// Ast.
void to_json(json &j, const Ast &parseResult);