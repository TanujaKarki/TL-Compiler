#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class NodeType {
  // Statements
  Program,
  VarDeclaration,
  FunctionDeclaration,
  StructDeclaration,
  IfStatement,
  WhileLoop,
  ReturnStatement,

  // Expressions
  AssignmentExpr,
  NumericLiteral,
  StrLiteral,
  Null,
  Identifier,
  BinaryExpr,
  CallExpr,
  MemberAccessExpr,
  UnaryExpr,
  LogicalExpr,
};

class Node {
public:
  NodeType kind;
};

class Stmt : public Node {
public:
  Stmt(NodeType kind);
  virtual ~Stmt() = default;
};

class Expr : public Stmt {
public:
  Expr(NodeType kind);
  virtual ~Expr() = default;
};

class Program : public Stmt {
public:
  vector<unique_ptr<Stmt>> body;
  Program();
};

class AssignmentExpr : public Expr {
public:
  unique_ptr<Expr> assigne;
  unique_ptr<Expr> value;
  AssignmentExpr(unique_ptr<Expr> assigne, unique_ptr<Expr> value);
};

class VarDeclaration : public Stmt {
public:
  bool constant;
  string identifier;
  unique_ptr<Expr> value;
  VarDeclaration(bool isConst, const string &id,
                 unique_ptr<Expr> val = nullptr);
};

class ReturnStatement : public Stmt {
public:
  unique_ptr<Stmt> returnValue;
  ReturnStatement(unique_ptr<Stmt> value);
};

class FunctionDeclaration : public Stmt {
public:
  vector<string> parameters;
  string name;
  vector<Stmt *> body;
  unique_ptr<ReturnStatement> returnStatement;
  FunctionDeclaration(vector<string> param, string n,
                      vector<Stmt *> b,
                      unique_ptr<ReturnStatement> retStmt = nullptr);
  ~FunctionDeclaration();
};

class BinaryExpr : public Expr {
public:
  unique_ptr<Expr> left;
  unique_ptr<Expr> right;
  string binaryOperator;
  BinaryExpr(unique_ptr<Expr> left, unique_ptr<Expr> right,
             const string &op);
};

class UnaryExpr : public Expr {
public:
  unique_ptr<Expr> right;
  string op;
  UnaryExpr(unique_ptr<Expr> right, const string &op);
};

class CallExpr : public Expr {
public:
  unique_ptr<Expr> caller;
  vector<unique_ptr<Expr>> args;
  CallExpr(unique_ptr<Expr> caller,
           vector<unique_ptr<Expr>> args);
};

class IdentifierExpr : public Expr {
public:
  string symbol;
  IdentifierExpr(const string &symbol);
};

class NumericLiteral : public Expr {
public:
  double value;
  NumericLiteral(double value);
};

class StrLiteral : public Expr {
public:
  string value;
  StrLiteral(string value);
};

class FloatLiteral : public Expr {
public:
  double value;
  FloatLiteral(double value);
};

class NullLiteral : public Expr {
public:
  string value;
  NullLiteral(const string &value);
};

class IfStatement : public Stmt {
public:
  unique_ptr<Expr> condition;
  vector<unique_ptr<Stmt>> ifBody;
  vector<unique_ptr<Stmt>> elseBody;
  IfStatement(unique_ptr<Expr> cond,
              vector<unique_ptr<Stmt>> ifB,
              vector<unique_ptr<Stmt>> elseB = {});
};

class WhileLoop : public Stmt {
public:
  unique_ptr<Expr> condition;
  vector<unique_ptr<Stmt>> loopBody;
  WhileLoop(unique_ptr<Expr> cond, vector<unique_ptr<Stmt>> bd);
};

class StructDeclaration : public Stmt {
public:
  string structName;
  vector<unique_ptr<Stmt>> structBody;
  StructDeclaration(const string &name,
                    vector<unique_ptr<Stmt>> body);
};

class MemberAccessExpr : public Expr {
public:
  unique_ptr<Expr> object;
  string memberName;
  MemberAccessExpr(unique_ptr<Expr> obj, const string &member);
};

class LogicalExpr : public Expr {
public:
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    string logicalOperator;

    LogicalExpr(unique_ptr<Expr> left, unique_ptr<Expr> right, const string& logicalOperator);
};

string NodeTypeToString(NodeType type);

void printProgram(unique_ptr<Program> program, const string &indent);

void printStatement(const Stmt &stmt, const string &indent);

#endif