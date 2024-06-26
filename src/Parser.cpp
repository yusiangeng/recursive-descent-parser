
#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "AstNode.h"
#include "Error.h"
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
    case TokenType::If:
      return IfStatement();
    case TokenType::CurlyOpen:
      return BlockStatement();
    case TokenType::Let:
      return VariableStatement();
    case TokenType::While:
    case TokenType::Do:
    case TokenType::For:
      return IterationStatement();
    default:
      return ExpressionStatement();
  }
}

AstNode *Parser::IterationStatement() {
  switch (_lookahead->type) {
    case TokenType::While:
      return WhileStatement();
    case TokenType::Do:
      return DoWhileStatement();
    case TokenType::For:
      return ForStatement();
    default:
      std::cerr << "Parser error: unkown IterationStatement type" << std::endl;
      throw;
  }
}

AstNode *Parser::WhileStatement() {
  _eat(TokenType::While);

  _eat(TokenType::ParenthesesOpen);
  AstNode *test = Expression();
  _eat(TokenType::ParenthesesClose);

  AstNode *body = Statement();
  return new WhileStatementNode(test, body);
}

AstNode *Parser::DoWhileStatement() {
  _eat(TokenType::Do);

  AstNode *body = Statement();

  _eat(TokenType::While);

  _eat(TokenType::ParenthesesOpen);
  AstNode *test = Expression();
  _eat(TokenType::ParenthesesClose);

  _eat(TokenType::Semicolon);

  return new DoWhileStatementNode(test, body);
}

AstNode *Parser::ForStatement() {
  _eat(TokenType::For);
  _eat(TokenType::ParenthesesOpen);

  AstNode *init =
      _lookahead->type != TokenType::Semicolon ? ForStatementInit() : nullptr;
  _eat(TokenType::Semicolon);

  AstNode *test =
      _lookahead->type != TokenType::Semicolon ? Expression() : nullptr;
  _eat(TokenType::Semicolon);

  AstNode *update =
      _lookahead->type != TokenType::ParenthesesClose ? Expression() : nullptr;
  _eat(TokenType::ParenthesesClose);

  AstNode *body = Statement();

  return new ForStatementNode(init, test, update, body);
}

AstNode *Parser::ForStatementInit() {
  if (_lookahead->type == TokenType::Let) {
    return VariableStatementInit();
  }
  return Expression();
}

AstNode *Parser::IfStatement() {
  _eat(TokenType::If);

  _eat(TokenType::ParenthesesOpen);
  AstNode *test = Expression();
  _eat(TokenType::ParenthesesClose);

  AstNode *consequent = Statement();

  AstNode *alternate = nullptr;
  if (_lookahead && _lookahead->type == TokenType::Else) {
    _eat(TokenType::Else);
    alternate = Statement();
  }

  return new IfStatementNode(test, consequent, alternate);
}

AstNode *Parser::VariableStatementInit() {
  _eat(TokenType::Let);
  std::vector<AstNode *> declarations = VariableDeclarationList();
  return new VariableStatementNode(declarations);
}

AstNode *Parser::VariableStatement() {
  AstNode *variableStatement = VariableStatementInit();
  _eat(TokenType::Semicolon);
  return variableStatement;
}

std::vector<AstNode *> Parser::VariableDeclarationList() {
  std::vector<AstNode *> declarations;

  declarations.push_back(VariableDeclaration());
  while (_lookahead->type == TokenType::Comma) {
    _eat(TokenType::Comma);
    declarations.push_back(VariableDeclaration());
  }

  return declarations;
}

AstNode *Parser::VariableDeclaration() {
  IdentifierNode *id = Identifier();

  // OptVariableInitializer
  AstNode *init = _lookahead->type != TokenType::Semicolon &&
                          _lookahead->type != TokenType::Comma
                      ? VariableInitializer()
                      : nullptr;

  return new VariableDeclarationNode(id, init);
}

AstNode *Parser::VariableInitializer() {
  _eat(TokenType::AssignSimple);
  return AssignmentExpression();
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

AstNode *Parser::Expression() { return AssignmentExpression(); }

AstNode *Parser::AssignmentExpression() {
  AstNode *left = LogicalORExpression();

  if (!_isAssignmentOperator(_lookahead->type)) {
    return left;
  }

  return new AssignmentExpressionNode(AssignmentOperator().value,
                                      _checkValidAssignmentTarget(left),
                                      AssignmentExpression());
}

AstNode *Parser::LeftHandSideExpression() { return PrimaryExpression(); }

IdentifierNode *Parser::Identifier() {
  std::string name = _eat(TokenType::Identifier).value;
  return new IdentifierNode(name);
}

IdentifierNode *Parser::_checkValidAssignmentTarget(AstNode *node) {
  if (IdentifierNode *v = dynamic_cast<IdentifierNode *>(node)) {
    return v;
  }
  throw SyntaxError("Invalid left-hand side in assignment expression");
}

AstNode *Parser::_LogicalExpression(std::function<AstNode *()> builder,
                                    TokenType operatorToken) {
  AstNode *left = builder();

  while (_lookahead->type == operatorToken) {
    std::string op = _eat(operatorToken).value;

    AstNode *right = builder();

    left = new LogicalExpressionNode(op, left, right);
  }

  return left;
}

bool Parser::_isAssignmentOperator(TokenType tokenType) {
  return tokenType == TokenType::AssignSimple ||
         tokenType == TokenType::AssignComplex;
}

Token Parser::AssignmentOperator() {
  if (_lookahead->type == TokenType::AssignSimple) {
    return _eat(TokenType::AssignSimple);
  }
  return _eat(TokenType::AssignComplex);
}

AstNode *Parser::LogicalORExpression() {
  return _LogicalExpression(std::bind(&Parser::LogicalANDExpression, this),
                            TokenType::LogicalOr);
}

AstNode *Parser::LogicalANDExpression() {
  return _LogicalExpression(std::bind(&Parser::EqualityExpression, this),
                            TokenType::LogicalAnd);
}

AstNode *Parser::EqualityExpression() {
  return _BinaryExpression(std::bind(&Parser::RelationalExpression, this),
                           TokenType::EqualityOperator);
}

AstNode *Parser::RelationalExpression() {
  return _BinaryExpression(std::bind(&Parser::AdditiveExpression, this),
                           TokenType::RelationalOperator);
}

AstNode *Parser::AdditiveExpression() {
  return _BinaryExpression(std::bind(&Parser::MultiplicativeExpression, this),
                           TokenType::AdditiveOperator);
}

AstNode *Parser::MultiplicativeExpression() {
  return _BinaryExpression(std::bind(&Parser::UnaryExpression, this),
                           TokenType::MultiplicativeOperator);
}

AstNode *Parser::UnaryExpression() {
  std::string op = "";
  switch (_lookahead->type) {
    case TokenType::AdditiveOperator:
      op = _eat(TokenType::AdditiveOperator).value;
      break;
    case TokenType::LogicalNot:
      op = _eat(TokenType::LogicalNot).value;
      break;
    default:
      break;
  }

  if (op != "") {
    return new UnaryExpressionNode(op, UnaryExpression());
  }

  return LeftHandSideExpression();
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
  if (_isLiteral(_lookahead->type)) {
    return Literal();
  }
  switch (_lookahead->type) {
    case TokenType::ParenthesesOpen:
      return ParenthesizedExpression();
    case TokenType::Identifier:
      return Identifier();
    default:
      return LeftHandSideExpression();
  }
}

bool Parser::_isLiteral(TokenType tokenType) {
  std::vector<TokenType> literalTokenTypes{TokenType::Number, TokenType::String,
                                           TokenType::True, TokenType::False,
                                           TokenType::NullSymbol};
  return std::find(literalTokenTypes.begin(), literalTokenTypes.end(),
                   tokenType) != literalTokenTypes.end();
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
    case TokenType::True:
      return BooleanLiteral(true);
    case TokenType::False:
      return BooleanLiteral(false);
    case TokenType::NullSymbol:
      return NullLiteral();

    default:
      throw SyntaxError("Literal: unexpected literal production");
  }
}

AstNode *Parser::BooleanLiteral(bool value) {
  _eat(value ? TokenType::True : TokenType::False);
  return new BooleanLiteralNode(value);
}

AstNode *Parser::NullLiteral() {
  _eat(TokenType::NullSymbol);
  return new NullLiteralNode();
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

  if (!token || token->type != tokenType) {
    std::string errorMessage =
        !token ? "Unexpected end of input"
               : "Unexpected token: \"" + token->value + "\"";

    auto itr = std::find_if(Spec.begin(), Spec.end(),
                            [tokenType](const SpecItem &specItem) {
                              return specItem.tokenType == tokenType;
                            });
    errorMessage += ", expected: \"" + itr->debugString + "\"";
    throw SyntaxError(errorMessage);
  }

  // Advance to next token.
  _lookahead = _tokenizer.getNextToken();

  return *token;
}
