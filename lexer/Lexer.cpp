// #include "Lexer.h"
#include <cctype>
#include <exception>
#include <iostream>
#include <unordered_map>
#include<fstream>
using namespace std;

unordered_map< string, TokenType> KEYWORDS = {
    {"null", Null},   {"let", Let},       {"const", Const},
    {"func", Func},   {"if", If},         {"else", Else},
    {"while", While}, {"return", Return}, {"struct", StructToken}};

Token::Token(const  string &value, TokenType type)
    : value(value), type(type) {}

 string Token::getValue() const { return value; }

TokenType Token::getType() const { return type; }

Lexer::Lexer(const  string &sourceCode)
    : sourceCode(sourceCode), currentChar(sourceCode[0]) {
  try {
    this->tokenize();
  }
  catch (const  exception& e) {
      throw;
  }
}

void Lexer::createNumberToken() {
   string num;
  num += currentChar;

  int amountOfDots = 0;

  while (!this->src.empty() &&
         ( isdigit(this->peek()) || this->peek() == '.')) {
    if (this->peek() == '.')
      amountOfDots++;
    if (amountOfDots > 1)
      this->unrecognizedChar(currentChar);
    num += this->eat();
  }
  if (amountOfDots == 0) {
    tokens.push_back(Token(num, NumberLiteral));
  } else {
    tokens.push_back(Token(num, FloatLiteral));
  }
}

void Lexer::createStringToken() {
   string stringLiteral;

  while (!src.empty() && peek() != '"') {
    stringLiteral += eat();
  }

  if (!src.empty() && peek() == '"') {
    eat();
    tokens.push_back(Token(stringLiteral, TokenType::StringLiteral));
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createAndToken() {
  if (!src.empty() && this->peek() == '&') {
    tokens.push_back(Token("&&", TokenType::And));
    this->eat();
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createOrToken() {
  if (!src.empty() && this->peek() == '|') {
    tokens.push_back(Token("||", TokenType::Or));
    this->eat();
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createBinaryOperatorToken() {
  tokens.push_back(Token( string(1, currentChar), BinaryOperator));
}

void Lexer::createCompareToken( string firstChar,  string secondChar,
                               TokenType firstToken, TokenType secondToken) {
   string z = firstChar + secondChar;
  char c = secondChar[0];
  if (!src.empty() && peek() == c) {
    tokens.push_back(Token(z, firstToken));
    this->eat();
  } else {
    tokens.push_back(Token(firstChar, secondToken));
  }
}

void Lexer::createOneCharToken( string tokenChar, TokenType charType) {
  tokens.push_back(Token(tokenChar, charType));
}

void Lexer::createIdentifierToken() {
   string ident;
  ident += currentChar;
  while (!src.empty() && ( isalnum(peek()) || peek() == '_')) {
    ident += this->eat();
  }

  auto it = KEYWORDS.find(ident);
  if (it != KEYWORDS.end()) {
    tokens.push_back(Token(ident, it->second));
  } else {
    tokens.push_back(Token(ident, Identifier));
  }
}

void Lexer::skipComments() {
    while (!src.empty() && this->peek() != '\n') {
        this->eat();
    }
}

void Lexer::tokenize() {
  src =  vector<char>(sourceCode.begin(), sourceCode.end());

  try {

  
  while (!src.empty()) {
    currentChar = this->eat();

    if (this->isSkippable(currentChar)) {
      continue;
    }
    if ( isdigit(currentChar)) {
      createNumberToken();
    } else if (this->isAlpha(currentChar)) {
      createIdentifierToken();
    } else {
      switch (currentChar) {
      case '#':
        skipComments();
        break;
      case '(':
        createOneCharToken("(", TokenType::OpenParen);
        break;
      case ')':
        createOneCharToken(")", TokenType::CloseParen);
        break;
      case '{':
        createOneCharToken("{", TokenType::OpenBrace);
        break;
      case '}':
        createOneCharToken("}", TokenType::CloseBrace);
        break;
      case '[':
        createOneCharToken("[", TokenType::OpenBracket);
        break;
      case ']':
        createOneCharToken("]", TokenType::CloseBracket);
        break;
      case ',':
        createOneCharToken(",", TokenType::Comma);
        break;
      case '.':
        createOneCharToken(".", TokenType::Dot);
        break;
      case '-':
        if ( isdigit(this->peek())) {
          createNumberToken();
        } else {
          createBinaryOperatorToken();
        }
        break;
      case '+':
      case '*':
      case '/':
      case '%':
        createBinaryOperatorToken();
        break;
      case ';':
        createOneCharToken(";", TokenType::Semicolon);
        break;
      case '&':
        createAndToken();
        break;
      case '|':
        createOrToken();
        break;
      case '!':
        createCompareToken("!", "=", TokenType::NotEqual, TokenType::Not);
        break;
      case '=':
        createCompareToken("=", "=", TokenType::EqualEqual, TokenType::Equals);
        break;
      case '<':
        createCompareToken("<", "=", TokenType::LessEqual, TokenType::LessThan);
        break;
      case '>':
        createCompareToken(">", "=", TokenType::GreaterEqual,
                           TokenType::GreaterThan);
        break;
      case '"':
        if (isAlpha(this->peek())) {
          createStringToken();
        }
        break;
      default:
        this->unrecognizedChar(currentChar);
        break;
      }
    }
  }

  createOneCharToken("EndOfFile", TokenType::EOFToken);
  }
  catch (const  exception& e) {        
    throw;
  }
}

char Lexer::eat() {
  char currentChar = this->src[0];
  this->src.erase(this->src.begin());
  return currentChar;
}

char Lexer::peek() const { return this->src[0]; }

bool Lexer::isAlpha(char c) const { return  isalpha(c) || c == '_'; }

bool Lexer::isSkippable(char c) const {
  return c == ' ' || c == '\n' || c == '\t';
}

bool Lexer::isInt(char c) const { return  isdigit(c); }

void Lexer::unrecognizedChar(char c) const {
   string message = "Unrecognized character found in source: ";
  message += c;
  throw LexerError(message);
}

 string Token::getTokenTypeName  () {
  static const  map<TokenType,  string> tokenTypeNames = {
      {Null, "Null"},
      {NumberLiteral, "NumberLiteral"},
      {Identifier, "Identifier"},
      {StringLiteral, "StringLiteral"},
      {FloatLiteral, "FloatLiteral"},
      {Let, "Let"},
      {Const, "Const"},
      {Func, "Func"},
      {StructToken, "Struct"},
      {If, "If"},
      {Else, "Else"},
      {While, "While"},
      {Return, "Return"},
      {EqualEqual, "EqualEqual"},
      {NotEqual, "NotEqual"},
      {LessThan, "LessThan"},
      {LessEqual, "LessEqual"},
      {GreaterThan, "GreaterThan"},
      {GreaterEqual, "GreaterEqual"},
      {And, "And"},
      {Or, "Or"},
      {Not, "Not"},
      {BinaryOperator, "BinaryOperator"},
      {Equals, "Equals"},
      {Semicolon, "Semicolon"},
      {OpenParen, "OpenParen"},
      {CloseParen, "CloseParen"},
      {OpenBrace, "OpenBrace"},
      {CloseBrace, "CloseBrace"},
      {OpenBracket, "OpenBracket"},
      {CloseBracket, "CloseBracket"},
      {Comma, "Comma"},
      {EOFToken, "EOFToken"}};

  auto it = tokenTypeNames.find(this->getType());
  if (it != tokenTypeNames.end()) {
    return it->second;
  } else {
    return "Unknown";
  }
}

 vector<Token> Lexer::getTokens() { return this->tokens; }

void Lexer::printTokens() {
   cout << "[ " <<  endl;
   vector<Token> tokens(this->getTokens());
  for (int i=0;i<tokens.size();i++) {
     cout<<" ["<< tokens[i].getValue()<<", " << tokens[i].getTokenTypeName  () <<"]";
    if(i != tokens.size()-1 )  cout<<",";
     cout<< endl;
  }
   cout << "]" <<  endl;
}

void Lexer::saveTokensInFile(string filename){
  ofstream out(filename);
  if(!out.is_open()){
    cerr << "Error in File Opening...";
    
  }
  for(int i=0;i<tokens.size();i++){
    out << tokens[i].getValue() << " " << tokens[i].getTokenTypeName  ()<<endl;
  }
}