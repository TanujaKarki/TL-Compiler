// #include "Parser.cpp"

using ExprPtr = unique_ptr<Expr>;
using StmtPtr = unique_ptr<Stmt>;

ExprPtr Parser::parse_expr() { 
  try {
    return parse_assignment_expr();
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_logical_expr() {
  try {

    ExprPtr left = parse_comparision_expr();

    while (is_logical_operator(at().getType())) {
        string logicalOperator = eat().getValue();
        ExprPtr right = parse_comparision_expr();
        left = make_unique<LogicalExpr>(move(left), move(right), logicalOperator);
    }

    return left;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_comparision_expr() {
  try {
   ExprPtr left = parse_additive_expr();

    if (is_comparison_operator(at().getType())) {
        string comparisonOperator = eat().getValue();
        ExprPtr right = parse_additive_expr();
        left = make_unique<BinaryExpr>(move(left), move(right), comparisonOperator);
    }

    return left;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_primary_expr() {
  try {
  TokenType tk = at().getType();
  ExprPtr value = nullptr;

  if (tk == TokenType::Not) {
    this->eat();
    value = make_unique<UnaryExpr>(parse_primary_expr(), "!");
  } else if (tk == TokenType::BinaryOperator && at().getValue() == "-") {
    this->eat();
    value = make_unique<UnaryExpr>(parse_primary_expr(), "-");
  } else {
    switch (tk) {
    case TokenType::Identifier:
      value = parse_member_access(
          make_unique<IdentifierExpr>(eat().getValue()));
      break;
    case TokenType::NumberLiteral:
      value = make_unique<NumericLiteral>(stod(eat().getValue()));
      break;
    case TokenType::FloatLiteral:
      value = make_unique<NumericLiteral>(stod(eat().getValue()));
      break;
    case TokenType::StringLiteral:
      value = make_unique<StrLiteral>(eat().getValue());
      break;
    case TokenType::Null:
      eat();
      value = make_unique<NullLiteral>("null");
      break;
    case TokenType::OpenParen:
      eat();
      value = parse_expr();
      expect(TokenType::CloseParen,
             "Unexpected token found inside parenthesized expression. Expected "
             "closing parenthesis.");
      break;
    default:
      value = nullptr;
      cerr << "Unexpected token found during parsing! " << at().getValue()
                << endl;
      exit(1);
    }
  }

  return value;

  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_additive_expr() {
  try {
    ExprPtr left = parse_multiplicative_expr();

    while (is_additive_operator(at().getValue())) {
      string binaryOperator = eat().getValue();
      ExprPtr right = parse_multiplicative_expr();
      left = make_unique<BinaryExpr>(move(left), move(right),
                                          binaryOperator);
    }

    return left;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_multiplicative_expr() {
  try {
    ExprPtr left = parse_call_member_expr();

    while (is_multiplicative_operator(at().getValue())) {
      string binaryOperator = eat().getValue();
      ExprPtr right = parse_primary_expr();
      left = make_unique<BinaryExpr>(move(left), move(right),
                                          binaryOperator);
    }

    return left;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_assignment_expr() {
  try {
    ExprPtr left = parse_additive_expr();

    if (at().getType() == TokenType::Equals) {
      eat();
      ExprPtr value = parse_assignment_expr();
      expect(TokenType::Semicolon,
             "Expected semicolon at the end of assignment expression");
      return make_unique<AssignmentExpr>(move(left), move(value));
    }

    return left;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_call_member_expr() {
  try {
    ExprPtr caller = parse_primary_expr();

    while (at().getType() == TokenType::OpenParen) {
      eat();
      vector<ExprPtr> arguments;

      if (at().getType() != TokenType::CloseParen) {
        arguments = parse_arguments_list();
      }

      expect(TokenType::CloseParen,
             "Expected a closing parenthesis in the function call");
      caller =
          make_unique<CallExpr>(move(caller), move(arguments));
    }

    return caller;
  }
  catch (const ParserError& e) {
    throw;
  }
}

ExprPtr Parser::parse_member_access(ExprPtr left) {
  try {
    while (at().getType() == TokenType::Dot ||
           at().getType() == TokenType::OpenParen) {
      if (at().getType() == TokenType::Dot) {
        eat(); // Consume the '.'
        string memberName =
            expect(TokenType::Identifier, "Expected identifier after '.'")
                .getValue();
        left = make_unique<MemberAccessExpr>(move(left),
                                                  move(memberName));
      } else if (at().getType() == TokenType::OpenParen) {
        vector<ExprPtr> arguments = parse_args();
        left = make_unique<CallExpr>(move(left), move(arguments));
      }
    }
    return left;
  }
  catch (const ParserError& e) {
    throw;
  }

}

vector<ExprPtr> Parser::parse_args() {
  try {
    expect(TokenType::OpenParen, "Expected open parenthesis");
    vector<ExprPtr> args;

    if (at().getType() != TokenType::CloseParen) {
      args = parse_arguments_list();
    }

    expect(TokenType::CloseParen,
           "Missing closing parenthesis inside arguments list");
    return args;
  }
  catch (const ParserError& e) {
    throw;
  }
}

vector<ExprPtr> Parser::parse_arguments_list() {
  try {
    vector<ExprPtr> args;
    args.push_back(parse_assignment_expr());

    while (at().getType() == TokenType::Comma) {
      eat();
      args.push_back(parse_assignment_expr());
    }

    return args;
  }
  catch (const ParserError& e) {
    throw;
  }
}