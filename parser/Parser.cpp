// #include "Parser.h"
#include <string>

unique_ptr<Program> Parser::produceAST(vector<Token> tokens) {
  this->tokens = tokens;

  unique_ptr<Program> program = make_unique<Program>();
  program->kind = NodeType::Program;

  while (!eof()) {
    program->body.push_back(parse_stmt());
  }

  return program;
}

bool Parser::eof() { return tokens[0].getType() == TokenType::EOFToken; }

Token Parser::at() { return tokens[0]; }

Token Parser::eat() {
  Token prev = tokens[0];
  tokens.erase(tokens.begin());
  return prev;
}

Token Parser::lookahead(size_t num) {
  if (num >= tokens.size()) {
    return Token("", TokenType::EOFToken);
  }
  return tokens[num];
}

Token Parser::expect(TokenType type, const string &err) {
  Token prev = this->eat();
  if (prev.getType() != type) {
    throw ParserError(err + prev.getValue() + " - Expecting: " + to_string(static_cast<int>(type)));
  }
  return prev;
}

bool Parser::is_comparison_operator(TokenType type) {
  return type == TokenType::LessThan || type == TokenType::LessEqual ||
         type == TokenType::GreaterThan || type == TokenType::GreaterEqual ||
         type == TokenType::EqualEqual || type == TokenType::NotEqual;
}

bool Parser::is_additive_operator(string value) {
  return value == "+" || value == "-";
}

bool Parser::is_multiplicative_operator(string value) {
  return value == "*" || value == "/" || value == "%";
}

bool Parser::is_logical_operator(TokenType type) {
    return type == TokenType::And || type == TokenType::Or;
}