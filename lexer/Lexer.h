#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
using namespace std;


enum TokenType {
  // Name of variable,
  Identifier,

  // Types
  NumberLiteral,
  StringLiteral,
  FloatLiteral,

  // KEYWORDS
  Null,
  Let,
  Const,
  Func,
  If,
  Else,
  While,
  Return,
  StructToken,

  // Assigment
  Equals, // =

  // Comapare
  EqualEqual,   // ==
  NotEqual,     // !=
  LessThan,     // <
  LessEqual,    // <=
  GreaterThan,  // >
  GreaterEqual, // >=

  // Operations
  BinaryOperator, // + - * / %

  // Logical operators
  And, // &&
  Or,  // ||
  Not, // !

  // Other characters
  Semicolon,    // ;
  OpenParen,    // (
  CloseParen,   // )
  OpenBrace,    // {
  CloseBrace,   // }
  OpenBracket,  // [
  CloseBracket, // ]
  Comma,        // ,
  Dot,          // .

  // End of file token
  EOFToken,
};

class Token {
public:
  Token(const  string &value, TokenType type);
   string getValue() const;
  TokenType getType() const;
   string getTokenTypeName  ();

private:
   string value;
  TokenType type;
};

class Lexer {
public:
  Lexer(const  string &sourceCode);
  void tokenize();
  vector<Token> getTokens();
  void printTokens();
  void saveTokensInFile(string filename);

private:
  string sourceCode;
  char currentChar;
  vector<Token> tokens;
  vector<char> src;

  bool isAlpha(char c) const;
  bool isSkippable(char c) const;
  bool isInt(char c) const;
  void unrecognizedChar(char c) const;
  char peek() const;
  char eat();

  void createNumberToken();
  void createStringToken();
  void createAndToken();
  void createOrToken();
  void createBinaryOperatorToken();
  void createCompareToken( string firstChar,  string secondChar,
                          TokenType firstToken, TokenType secondToken);
  void createOneCharToken( string tokenChar, TokenType charType);
  void createIdentifierToken();

  void skipComments();
};

class LexerError : public  runtime_error {
  public:
      LexerError(const  string& message) :  runtime_error(message) {}
};

#endif