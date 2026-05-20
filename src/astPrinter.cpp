#include "../include/ast/astPrinter.h"
#include <iostream>
#include <sstream>

ASTPrinter::ASTPrinter(std::ostream &out) : out(out) {}

ASTPrinter::ASTPrinter() : out(std::cout) {}

void ASTPrinter::print(const Program &program) {
  out << "Program\n";
  printBlock(program.statements, 1);
}

void ASTPrinter::printBlock(const std::vector<std::unique_ptr<Stmt>> &stmts,
                            int indent) {
  for (const auto &stmt : stmts) {
    printStmt(stmt.get(), indent);
  }
}

void ASTPrinter::printStmt(const Stmt *stmt, int indent) {
  if (stmt == nullptr) {
    writeIndent(indent);
    out << "<null stmt>\n";
    return;
  }

  if (const auto *varDecl = dynamic_cast<const VarDeclStmt *>(stmt)) {
    writeIndent(indent);
    out << "VarDecl(" << varDecl->type << " " << varDecl->name;
    if (varDecl->initializer) {
      out << " = " << exprToString(varDecl->initializer.get());
    }
    out << ")\n";
    return;
  }

  if (const auto *assign = dynamic_cast<const AssignStmt *>(stmt)) {
    writeIndent(indent);
    out << "Assign(" << assign->name << " = "
        << exprToString(assign->value.get()) << ")\n";
    return;
  }

  if (const auto *ifStmt = dynamic_cast<const ifstmt *>(stmt)) {
    writeIndent(indent);
    out << "If\n";

    writeIndent(indent + 1);
    out << "Condition: " << exprToString(ifStmt->condition.get()) << "\n";

    writeIndent(indent + 1);
    out << "Then:\n";
    printBlock(ifStmt->thenBranch, indent + 2);

    for (const auto &branch : ifStmt->elseIfBranches) {
      writeIndent(indent + 1);
      out << "ElseIf:\n";
      writeIndent(indent + 2);
      out << "Condition: " << exprToString(branch.condition.get()) << "\n";
      writeIndent(indent + 2);
      out << "Then:\n";
      printBlock(branch.body, indent + 3);
    }

    if (!ifStmt->elseBranch.empty()) {
      writeIndent(indent + 1);
      out << "Else:\n";
      printBlock(ifStmt->elseBranch, indent + 2);
    }
    return;
  }

  if (const auto *whileStmt = dynamic_cast<const whilestmt *>(stmt)) {
    writeIndent(indent);
    out << "While\n";
    writeIndent(indent + 1);
    out << "Condition: " << exprToString(whileStmt->condition.get()) << "\n";
    writeIndent(indent + 1);
    out << "Body:\n";
    printBlock(whileStmt->body, indent + 2);
    return;
  }

  if (const auto *func = dynamic_cast<const FuncStmt *>(stmt)) {
    writeIndent(indent);
    out << "Func(" << func->returnType << " " << func->name << "(";
    for (size_t i = 0; i < func->parameters.size(); ++i) {
      if (i > 0) {
        out << ", ";
      }
      out << func->parameters[i].name << ":" << func->parameters[i].type;
    }
    out << "))\n";
    writeIndent(indent + 1);
    out << "Body:\n";
    printBlock(func->body, indent + 2);
    return;
  }

  if (const auto *call = dynamic_cast<const callExpr *>(stmt)) {
    writeIndent(indent);
    out << "ExprStmt(" << exprToString(call) << ")\n";
    return;
  }

  writeIndent(indent);
  out << "<unknown stmt>\n";
}

void ASTPrinter::printExpr(const Expr *expr, int indent) {
  writeIndent(indent);
  out << exprToString(expr) << "\n";
}

std::string ASTPrinter::exprToString(const Expr *expr) {
  if (expr == nullptr) {
    return "<null expr>";
  }

  if (const auto *number = dynamic_cast<const NumberExpr *>(expr)) {
    return std::to_string(number->value);
  }

  if (const auto *identifier = dynamic_cast<const IdentifierExpr *>(expr)) {
    return identifier->name;
  }

  if (const auto *binary = dynamic_cast<const BinaryExpr *>(expr)) {
    return "(" + exprToString(binary->left.get()) + " " +
           opToString(binary->op) + " " + exprToString(binary->right.get()) +
           ")";
  }

  if (const auto *call = dynamic_cast<const callExpr *>(expr)) {
    std::ostringstream stream;
    stream << "Call(" << call->nameoffunc << "(";
    for (size_t i = 0; i < call->arguments.size(); ++i) {
      if (i > 0) {
        stream << ", ";
      }
      stream << exprToString(call->arguments[i].get());
    }
    stream << "))";
    return stream.str();
  }

  if (const auto *array = dynamic_cast<const ArrayExpr *>(expr)) {
    std::ostringstream stream;
    stream << "{";
    for (size_t i = 0; i < array->elements.size(); ++i) {
      if (i > 0) {
        stream << ", ";
      }
      stream << exprToString(array->elements[i].get());
    }
    stream << "}";
    return stream.str();
  }

  return "<unknown expr>";
}

std::string ASTPrinter::opToString(const std::string &op) {
  if (op == "PLUS")
    return "+";
  if (op == "MINUS")
    return "-";
  if (op == "STAR")
    return "*";
  if (op == "SLASH")
    return "/";
  if (op == "EQUAL")
    return "==";
  if (op == "NOT_EQUAL")
    return "!=";
  if (op == "GREATER")
    return ">";
  if (op == "LESS")
    return "<";
  if (op == "GREATER_EQUAL")
    return ">=";
  if (op == "LESS_EQUAL")
    return "<=";
  return op;
}

void ASTPrinter::writeIndent(int indent) {
  for (int i = 0; i < indent; ++i) {
    out << "  ";
  }
}
