#ifndef PARSER_H
#define PARSER_H

// #include "../ast/AST.cpp"
// #include "../lexer/Lexer.cpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Parser {
private:
  vector<Token> tokens;
  bool eof();

  Token at();
  Token eat();
  Token expect(TokenType type, const string &err);
  Token lookahead(size_t num);

  unique_ptr<Stmt> parse_stmt();
  unique_ptr<Stmt> parse_if_statement();
  unique_ptr<Stmt> parse_while_statement();
  unique_ptr<Stmt> parse_return_statement();
  unique_ptr<Stmt> parse_var_declaration();
  unique_ptr<Stmt> parse_function_declaration();
  unique_ptr<Stmt> parse_struct_declaration();

  unique_ptr<Expr> parse_expr();
  unique_ptr<Expr> parse_additive_expr();
  unique_ptr<Expr> parse_multiplicative_expr();
  unique_ptr<Expr> parse_primary_expr();
  unique_ptr<Expr> parse_assignment_expr();
  unique_ptr<Expr> parse_comparision_expr();
  unique_ptr<Expr> parse_logical_expr();
  unique_ptr<Expr> parse_call_member_expr();
  unique_ptr<Expr> parse_member_access(unique_ptr<Expr> left);

  vector<unique_ptr<Expr>> parse_arguments_list();
  vector<unique_ptr<Expr>> parse_args();

  bool is_comparison_operator(TokenType type);
  bool is_additive_operator(string type);
  bool is_multiplicative_operator(string value);
  bool is_logical_operator(TokenType type);

public:
  unique_ptr<Program> produceAST(vector<Token> tokens);
};

class ParserError : public runtime_error {
  public:
      ParserError(const string& message) : runtime_error(message) {}
};

#endif