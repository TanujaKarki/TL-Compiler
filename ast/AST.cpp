// #include "AST.h"

Stmt::Stmt(NodeType kind) { this->kind = kind; }
Expr::Expr(NodeType kind) : Stmt(kind) {}

Program::Program() : Stmt(NodeType::Program) {}

VarDeclaration::VarDeclaration(bool isConst, const string &id,
                               unique_ptr<Expr> val)
    : Stmt(NodeType::VarDeclaration), constant(isConst), identifier(id),
      value(move(val)) {}

BinaryExpr::BinaryExpr(unique_ptr<Expr> left, unique_ptr<Expr> right,
                       const string &op)
    : Expr(NodeType::BinaryExpr), left(move(left)),
      right(move(right)), binaryOperator(op) {}

UnaryExpr::UnaryExpr(unique_ptr<Expr> right, const string &op)
    : Expr(NodeType::UnaryExpr), right(move(right)), op(op) {}

IdentifierExpr::IdentifierExpr(const string &symbol)
    : Expr(NodeType::Identifier), symbol(symbol) {}

NumericLiteral::NumericLiteral(double value)
    : Expr(NodeType::NumericLiteral), value(value) {}

FloatLiteral::FloatLiteral(double value)
    : Expr(NodeType::NumericLiteral), value(value) {}

StrLiteral::StrLiteral(string value)
    : Expr(NodeType::StrLiteral), value(value) {}

NullLiteral::NullLiteral(const string &value)
    : Expr(NodeType::Null), value(value) {}

AssignmentExpr::AssignmentExpr(unique_ptr<Expr> assigne,
                               unique_ptr<Expr> val)
    : Expr(NodeType::AssignmentExpr), assigne(move(assigne)),
      value(move(val)) {}

CallExpr::CallExpr(unique_ptr<Expr> caller,
                   vector<unique_ptr<Expr>> args)
    : Expr(NodeType::CallExpr), caller(move(caller)),
      args(move(args)) {}

MemberAccessExpr::MemberAccessExpr(unique_ptr<Expr> obj,
                                   const string &member)
    : Expr(NodeType::MemberAccessExpr), object(move(obj)),
      memberName(member) {}

FunctionDeclaration::FunctionDeclaration(
    vector<string> param, string n, vector<Stmt *> b,
    unique_ptr<ReturnStatement> retStmt)
    : Stmt(NodeType::FunctionDeclaration), parameters(param), name(n), body(b),
      returnStatement(move(retStmt)) {}

FunctionDeclaration::~FunctionDeclaration() {
  for (auto &stmt : body) {
    delete stmt;
  }
}

IfStatement::IfStatement(unique_ptr<Expr> cond,
                         vector<unique_ptr<Stmt>> ifB,
                         vector<unique_ptr<Stmt>> elseB)
    : Stmt(NodeType::IfStatement), condition(move(cond)),
      ifBody(move(ifB)), elseBody(move(elseB)) {}

WhileLoop::WhileLoop(unique_ptr<Expr> cond,
                     vector<unique_ptr<Stmt>> bd)
    : Stmt(NodeType::WhileLoop), condition(move(cond)),
      loopBody(move(bd)) {}

ReturnStatement::ReturnStatement(unique_ptr<Stmt> value)
    : Stmt(NodeType::ReturnStatement), returnValue(move(value)) {}

StructDeclaration::StructDeclaration(const string &name,
                                     vector<unique_ptr<Stmt>> body)
    : Stmt(NodeType::StructDeclaration), structName(name),
      structBody(move(body)) {}

LogicalExpr::LogicalExpr(unique_ptr<Expr> left, unique_ptr<Expr> right, const string& logicalOperator)
        : Expr(NodeType::LogicalExpr), left(move(left)), right(move(right)), logicalOperator(logicalOperator) {}