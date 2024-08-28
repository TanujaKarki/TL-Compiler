#include<iostream>
#include<fstream>
using namespace std;

#include "ast/AST.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include "lexer/Lexer.cpp"
#include "ast/AST.cpp"
#include "ast/PrinterAST.cpp"
#include "parser/Parser.cpp"
#include "parser/ParserExpr.cpp"
#include "parser/ParserStml.cpp"

int main(){

    // const  string str=" int main() { \nint a=10,b=20;\n a = a+b;\n printf(a); \n} #this is commment.\n";
     ifstream file("code.tl");

    if (!file.is_open()) {
         cout << "Error: Unable to open the file." <<  endl;
        return 0; 
    }
     string sourceCode="";
     string line="";
    while( getline(file,line)){
        //  cout<<"inside"<< endl;
        sourceCode+=line;
        sourceCode+='\n';
    }

    Lexer *lex=new Lexer(sourceCode);
    lex->printTokens();

    lex->saveTokensInFile("Tokenized.txt");
    // ofstream output("output.txt");
    // if(!output){
    //     cerr<< "Error opening files... ";
    // }
    // for(int i=0;i<){

    // }

    Parser *parse = new Parser();

    printProgram(parse->produceAST(lex->getTokens()),"      ");
    return 0;
}