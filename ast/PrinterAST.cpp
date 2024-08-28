// #include "AST.h"
// #include<fstream>
// using namespace std;

ofstream out("Parsed_AST.txt");
void printProgram (unique_ptr<Program> program, const string &indent) {
  
  
  if(!out.is_open()){
    cerr << "Error in File Opening...";
    return;
  }

  out << '{' << endl;
  out << indent << " \"Program\": [\n";
  for (const auto &stmt : program->body) {
    printStatement(*stmt, indent + "  ");
    if (stmt != program->body.back()) {
      out << ',';
    }
    out << '\n';
  }
  out << indent << ']';
  out << "\n}\n";
}

void printStatement(const Stmt &stmt, const string &indent) {
  out << indent << "{\n";
  out << indent << "  \"Statement\": \"" << NodeTypeToString(stmt.kind)
            << "\",\n";

  switch (stmt.kind) {
  case NodeType::Identifier: {
    const auto &id = static_cast<const IdentifierExpr &>(stmt);
    out << indent << "  \"Symbol\": \"" << id.symbol << "\"";
    break;
  }
  case NodeType::NumericLiteral: {
    const auto &numLit = static_cast<const NumericLiteral &>(stmt);
    out << indent << "  \"Value\": " << numLit.value;
    break;
  }
  case NodeType::StrLiteral: {
    const auto &strLit = static_cast<const StrLiteral &>(stmt);
    out << indent << "  \"Value\": \"" << strLit.value << "\"";
    break;
  }
  case NodeType::BinaryExpr: {
    const auto &binaryExpr = static_cast<const BinaryExpr &>(stmt);
    out << indent << "  \"BinaryOperator\": \""
              << binaryExpr.binaryOperator << "\",\n";
    out << indent << "  \"Left\": ";
    printStatement(*binaryExpr.left, indent + "    ");
    out << ",\n";
    out << indent << "  \"Right\": ";
    printStatement(*binaryExpr.right, indent + "    ");
    break;
  }
  case NodeType::VarDeclaration: {
    const auto &varDecl = static_cast<const VarDeclaration &>(stmt);
    out << indent
              << "  \"Constant\": " << (varDecl.constant ? "true" : "false")
              << ",\n";
    out << indent << "  \"Identifier\": \"" << varDecl.identifier
              << "\",\n";
    out << indent << "  \"Value\": ";
    if (varDecl.value) {
      printStatement(*varDecl.value, indent + "    ");
    } else {
      out << "null";
    }
    break;
  }
  case NodeType::CallExpr: {
    const auto &callExpr = static_cast<const CallExpr &>(stmt);
    out << indent << "  \"Caller\": ";
    printStatement(*callExpr.caller, indent + "    ");
    out << ",\n";
    out << indent << "  \"Arguments\": [\n";
    for (const auto &arg : callExpr.args) {
      printStatement(*arg, indent + "    ");
      if (&arg != &callExpr.args.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ]";
    break;
  }
  case NodeType::FunctionDeclaration: {
    const auto &funcDecl = static_cast<const FunctionDeclaration &>(stmt);
    out << indent << "  \"Name\": \"" << funcDecl.name << "\",\n";
    out << indent << "  \"Parameters\": [\n";
    for (const auto &param : funcDecl.parameters) {
      out << indent << "    \"" << param << "\"";
      if (&param != &funcDecl.parameters.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ],\n";
    out << indent << "  \"Body\": [\n";
    for (const auto &bodyStmt : funcDecl.body) {
      printStatement(*bodyStmt, indent + "    ");
      if (&bodyStmt != &funcDecl.body.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ]";
    break;
  }
  case NodeType::IfStatement: {
    const auto &ifStmt = static_cast<const IfStatement &>(stmt);
    out << indent << "  \"Condition\": ";
    printStatement(*ifStmt.condition, indent + "    ");
    out << ",\n";
    out << indent << "  \"IfBody\": [\n";
    for (const auto &ifBodyStmt : ifStmt.ifBody) {
      printStatement(*ifBodyStmt, indent + "    ");
      if (&ifBodyStmt != &ifStmt.ifBody.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ],\n";
    out << indent << "  \"ElseBody\": [\n";
    for (const auto &elseBodyStmt : ifStmt.elseBody) {
      printStatement(*elseBodyStmt, indent + "    ");
      if (&elseBodyStmt != &ifStmt.elseBody.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ]";
    break;
  }
  case NodeType::WhileLoop: {
    const auto &whileLoop = static_cast<const WhileLoop &>(stmt);
    out << indent << "  \"Condition\": ";
    printStatement(*whileLoop.condition, indent + "    ");
    out << ",\n";
    out << indent << "  \"LoopBody\": [\n";
    for (const auto &loopBodyStmt : whileLoop.loopBody) {
      printStatement(*loopBodyStmt, indent + "    ");
      if (&loopBodyStmt != &whileLoop.loopBody.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ]";
    break;
  }
  case NodeType::StructDeclaration: {
    const StructDeclaration &structDecl =
        static_cast<const StructDeclaration &>(stmt);
    out << indent << "  \"StructName\": \"" << structDecl.structName
              << "\"";
    for (const auto &stmt : structDecl.structBody) {
      printStatement(*stmt, indent + "  ");
    }
    break;
  }
  case NodeType::MemberAccessExpr: {
    const auto &memberAccessExpr = static_cast<const MemberAccessExpr &>(stmt);
    out << indent << "  \"Object\": ";
    printStatement(*memberAccessExpr.object, indent + "    ");
    out << ",\n";
    out << indent << "  \"MemberName\": \"" << memberAccessExpr.memberName
              << "\"";
    break;
  }
  case NodeType::LogicalExpr: {
	const auto &logicalExpr = static_cast<const LogicalExpr &>(stmt);
	out << indent << "  \"LogicalOperator\": \"" << logicalExpr.logicalOperator << "\",\n";
	out << indent << "  \"Left\": ";
	printStatement(*logicalExpr.left, indent + "    ");
	out << ",\n";
	out << indent << "  \"Right\": ";
	printStatement(*logicalExpr.right, indent + "    ");
	break;
  }
  case NodeType::ReturnStatement: {
    const auto &returnStmt = static_cast<const ReturnStatement &>(stmt);
    out << indent << "  \"ReturnValue\": ";
    printStatement(*returnStmt.returnValue, indent + "    ");
    break;
  }
  case NodeType::AssignmentExpr: {
    const auto &assignmentExpr = static_cast<const AssignmentExpr &>(stmt);
    out << indent << "  \"Assignee\": ";
    printStatement(*assignmentExpr.assigne, indent + "    ");
    out << ",\n";
    out << indent << "  \"Value\": ";
    printStatement(*assignmentExpr.value, indent + "    ");
    break;
  }
  case NodeType::Program: {
    const auto &program = static_cast<const Program &>(stmt);
    out << indent << "  \"Body\": [\n";
    for (const auto &bodyStmt : program.body) {
      printStatement(*bodyStmt, indent + "    ");
      if (&bodyStmt != &program.body.back()) {
        out << ",";
      }
      out << "\n";
    }
    out << indent << "  ]";
    break;
  }
  case NodeType::Null: {
    const auto &nullNode = static_cast<const NullLiteral &>(stmt);
    out << indent << "  \"Value\": \"" << nullNode.value << "\"";
    break;
  }
  case NodeType::UnaryExpr: {
    const auto &unaryExpr = static_cast<const UnaryExpr &>(stmt);
    out << indent << "  \"Operator\": \"" << unaryExpr.op << "\",\n";
    out << indent << "  \"Right\": ";
    printStatement(*unaryExpr.right, indent + "    ");
    break;
  }
  }

  out << "\n" << indent << "}";
}

string NodeTypeToString(NodeType type) {
  switch (type) {
  case NodeType::Program:
    return "Program";
  case NodeType::NumericLiteral:
    return "NumericLiteral";
  case NodeType::StrLiteral:
    return "StrLiteral";
  case NodeType::Identifier:
    return "Identifier";
  case NodeType::BinaryExpr:
    return "BinaryExpr";
  case NodeType::AssignmentExpr:
    return "AssigmentExpr";
  case NodeType::VarDeclaration:
    return "VarDeclaration";
  case NodeType::CallExpr:
    return "CallExpr";
  case NodeType::MemberAccessExpr:
    return "MemberAccessExpr";
  case NodeType::LogicalExpr:
    return "LogicalExpr";
  case NodeType::FunctionDeclaration:
    return "FunctionDeclaration";
  case NodeType::IfStatement:
    return "IfStatement";
  case NodeType::WhileLoop:
    return "WhileLoop";
  case NodeType::StructDeclaration:
    return "StructDeclaration";
  case NodeType::ReturnStatement:
    return "ReturnStatement";
  case NodeType::Null:
    return "Null";
  default:
    return "Unknown";
  }
}