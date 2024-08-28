// #include "Parser.cpp"

using ExprPtr = unique_ptr<Expr>;
using StmtPtr = unique_ptr<Stmt>;

StmtPtr Parser::parse_stmt() {
  try {
    if (at().getType() == TokenType::Let || at().getType() == TokenType::Const) {
      return parse_var_declaration();
    }

    if (at().getType() == TokenType::StructToken) {
      return parse_struct_declaration();
    }

    if (at().getType() == TokenType::Func) {
      return parse_function_declaration();
    }

    if (at().getType() == TokenType::If) {
      return parse_if_statement();
    }

    if (at().getType() == TokenType::While) {
      return parse_while_statement();
    }

    if (at().getType() == TokenType::Return) {
      return parse_return_statement();
    }

    return parse_expr();
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_while_statement() {
  try {
    eat();

    expect(TokenType::OpenParen, "Expected '(' after 'while'");

    ExprPtr condition = parse_logical_expr();

    expect(TokenType::CloseParen, "Expected ')' after 'while' condition");

    expect(TokenType::OpenBrace, "Expected '{' open 'while' body");

    vector<StmtPtr> loopBody;
    while (at().getType() != TokenType::CloseBrace) {
      loopBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'while' body");

    return make_unique<WhileLoop>(move(condition), move(loopBody));
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_return_statement() {
  try {
    eat(); // Consume the "return" keyword

    if (at().getType() == TokenType::Semicolon) {
      // Return statement without a value
      eat(); // Consume the semicolon
      return make_unique<ReturnStatement>(nullptr);
    } else {
      // Return statement with a value
      unique_ptr<Stmt> value = parse_stmt();
      expect(TokenType::Semicolon, "Return statement must end with a semicolon.");
      return make_unique<ReturnStatement>(move(value));
    }
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_if_statement() {
  try {
    eat(); // Consume the "if" keyword
    expect(TokenType::OpenParen, "Expected '(' after 'if'");

    ExprPtr condition = parse_logical_expr();

    expect(TokenType::CloseParen, "Expected ')' after 'if' condition");

    vector<StmtPtr> ifBody;

    expect(TokenType::OpenBrace, "Expected '{' open 'if' body");

    while (at().getType() != TokenType::CloseBrace) {
      ifBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'if' body");

    vector<StmtPtr> elseBody;

    if (at().getType() == TokenType::Else) {
      eat();

      expect(TokenType::OpenBrace, "Expected '{' open 'else' body");
      while (at().getType() != TokenType::CloseBrace) {
        elseBody.push_back(parse_stmt());
      }
      expect(TokenType::CloseBrace, "Expected '}' close 'else' body");
    }

    return make_unique<IfStatement>(move(condition), move(ifBody),
                                         move(elseBody));
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_var_declaration() {
  try {
  bool isConstant = this->eat().getType() == TokenType::Const;

  string identifier =
      expect(TokenType::Identifier,
             "Expected identifier name following let | const keywords.")
          .getValue();

  if (this->at().getType() == TokenType::Semicolon) {
    this->eat();
    if (isConstant) {
      cerr
          << "Must assign value to constant expression. No value provided."
          << endl;
      exit(1);
    }

    return make_unique<VarDeclaration>(false, identifier);
  }

  expect(TokenType::Equals,
         "Expected equals token following identifier in var declaration.");

  ExprPtr value = this->parse_expr();

  expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");

  return make_unique<VarDeclaration>(isConstant, identifier,
                                          move(value));
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_function_declaration() {
  try {
    this->eat();
    string name = this->expect(TokenType::Identifier,
                                    "Expected function name following fn keyword")
                           .getValue();
    vector<ExprPtr> args = this->parse_args();

    vector<string> params;

    for (auto &arg : args) {
      if (arg->kind == NodeType::Identifier) {
        params.push_back(static_cast<IdentifierExpr *>(arg.get())->symbol);
      } else {
        cerr << "Inside function declaration expected parameters to be of "
                     "type Identifier."
                  << endl;
        exit(1);
      }
    }

    expect(TokenType::OpenBrace, "Expected function body following declaration");

    vector<Stmt *> body;

    while (this->at().getType() != TokenType::EOFToken &&
           this->at().getType() != TokenType::CloseBrace) {
      body.push_back(parse_stmt().release());
    }

    expect(TokenType::CloseBrace,
           "Closing brace expected inside function declaration");

    return make_unique<FunctionDeclaration>(move(params), name, body);
  }
  catch (const ParserError& e) {
    throw;
  }
}

StmtPtr Parser::parse_struct_declaration() {
  try {
    eat(); // Consume the "struct" keyword

    string structName =
        expect(TokenType::Identifier,
               "Expected struct name following 'struct' keyword")
            .getValue();

    expect(TokenType::OpenBrace, "Expected '{' after struct name");

    vector<unique_ptr<Stmt>> structBody;

    while (at().getType() != TokenType::CloseBrace) {
      structBody.push_back(parse_var_declaration());
    }

    expect(TokenType::CloseBrace, "Expected '}' after struct body");

    return make_unique<StructDeclaration>(structName, move(structBody));
  }
  catch (const ParserError& e) {
    throw;
  }
}