#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "symbol.hpp"
#include "type.hpp"

inline std::ostream& operator<<(std::ostream &out, Type t) {
  switch (t->kind) {
    case TYPE_integer: out << "integer"; break;
    case TYPE_real: out << "real"; break;
    case TYPE_boolean: out << "boolean"; break;
    case TYPE_char: out << "char"; break;
    case TYPE_arrayI: out << "arrayI"; break;
    case TYPE_arrayII: out << "arrayII"; break;
    case TYPE_pointer: out << "pointer"; break;
    case TYPE_string: out << "string"; break;
  }
  return out;
}

class AST {
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
  virtual void sem() {}
};

inline std::ostream& operator<<(std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}


class Expr: public AST {
protected:
  Type type;
public:
  bool type_check(myType t) {
    sem();
    return (type->kind == t);
  }
  virtual int eval() const = 0;
};


class Stmt: public AST {
public:
  virtual void run() const = 0;
};


class Lvalue: public Expr {
public:
  virtual int eval() const override {
    exit(1);
  }
};

class Rvalue: public Expr {
protected:
  Type type;
//sem(), type_check(type t)->bool, eval()
public:
  virtual int eval() const override {
    exit(1);
  }
};

// class Pointer: public Lvalue {};
//
// class Array: public Lvalue {};
//
// class Stringconst: public Lvalue {};
//
// class Result: public Lvalue {};

//pif
class Id: public Lvalue {
private:
  char *var;
  int offset;
public:
  Id(char *v): var(v), offset(-1) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << var << "@" << offset << ")";
  }
  virtual int eval() const override {
    return rt_stack[offset];
  }
  virtual void sem() override {
    SymbolEntry *e = st.lookup(var);
    type = e->type;
    offset = e->offset;
  }
};

class BinOp: public Rvalue {
private:
  Expr *left;
  char *op;
  Expr *right;
public:
  BinOp(Expr *l, char *o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual void sem() override {
    if (( left->type_check(TYPE_arrayI) ) || ( left->type_check(TYPE_arrayII) ) || ( right->type_check(TYPE_arrayI) ) || ( right->type_check(TYPE_arrayII) )) {
      exit(1);
    }
    if (( left->type_check(TYPE_pointer) ) && !( right->type_check(TYPE_pointer) )) {
      exit(1);
    }
    if (( left->type_check(TYPE_char) ) && !( right->type_check(TYPE_char) )) {
      exit(1);
    }
    if (( left->type_check(TYPE_boolean) ) && !( right->type_check(TYPE_boolean) )) {
      exit(1);
    }
    if ((( left->type_check(TYPE_integer) ) && !( right->type_check(TYPE_integer) )) || (( left->type_check(TYPE_integer) ) && !( right->type_check(TYPE_real) ))) {
      exit(1);
    }
    if ((( left->type_check(TYPE_real) ) && !( right->type_check(TYPE_integer) )) || (( left->type_check(TYPE_real) ) && !( right->type_check(TYPE_real) ))) {
      exit(1);
    }

    if (!std::strcmp(op, "+") || !std::strcmp(op, "-") || !std::strcmp(op, "*")) {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type->kind=TYPE_integer;
      }
      else if (( left->type_check(TYPE_real) ) || ( right->type_check(TYPE_real) )) {
        type->kind=TYPE_real;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "/")) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type->kind=TYPE_real;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "div") || !std::strcmp(op, "mod")) {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type->kind=TYPE_integer;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "=") || !std::strcmp(op, "<>")) {
      type->kind=TYPE_boolean;
    }
    else if (!std::strcmp(op, "or") || !std::strcmp(op, "and")) {
      if (( left->type_check(TYPE_boolean) )) {
        type->kind=TYPE_boolean;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "<") || !std::strcmp(op, ">") || !std::strcmp(op, ">=") || !std::strcmp(op, "<=")) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type->kind=TYPE_boolean;
      }
      else {
        exit(1);
      }
    }
    else {
      exit(1);
    }

  }
  virtual int eval() const override {
    if (!std::strcmp(op, "+")) {
      return left->eval() + right->eval();
    }
    if (!std::strcmp(op, "-")) {
      return left->eval() - right->eval();
    }
    if (!std::strcmp(op, "*")) {
      return left->eval() * right->eval();
    }
    if (!std::strcmp(op, "/")) {
      return left->eval() / right->eval();
    }
    if (!std::strcmp(op, "div")) {
      return left->eval() / right->eval();
    }
    if (!std::strcmp(op, "mod")) {
      return left->eval() % right->eval();
    }
    if (!std::strcmp(op, "=")) {
      return left->eval() == right->eval();
    }
    if (!std::strcmp(op, "<>")) {
      return left->eval() != right->eval();
    }
    if (!std::strcmp(op, "or")) {
      return left->eval() || right->eval();
    }
    if (!std::strcmp(op, "and")) {
      return left->eval() && right->eval();
    }
    if (!std::strcmp(op, ">")) {
      return left->eval() > right->eval();
    }
    if (!std::strcmp(op, "<")) {
      return left->eval() < right->eval();
    }
    if (!std::strcmp(op, ">=")) {
      return left->eval() >= right->eval();
    }
    if (!std::strcmp(op, "<=")) {
      return left->eval() <= right->eval();
    }
    exit(1);  // this will never be reached.
  }
};

class UnOp: public Rvalue {
private:
  char *op;
  Expr *right;
public:
  UnOp(char *o, Expr *r): op(o), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *right << ")";
  }
  virtual void sem() override {
    if (!std::strcmp(op, "+") || !std::strcmp(op, "-")) {
      if (right->type_check(TYPE_integer)) {
        type->kind=TYPE_integer;
      }
      else if (right->type_check(TYPE_real) ) {
        type->kind=TYPE_integer;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "not")) {
      if ( right->type_check(TYPE_boolean) ) {
        type->kind=TYPE_boolean;
      }
      else {
        exit(1);
      }
    }
    else {
      exit(1);  // Unreachable
    }
  }
  virtual int eval() const override {
    if (!std::strcmp(op, "+")) {
      return right->eval();
    }
    if (!std::strcmp(op, "-")) {
      return (-1)*right->eval();
    }
    if (!std::strcmp(op, "not")) {
      return !right->eval();
    }
    exit(1);  // this will never be reached.
  }
};

//pif
class Nil: public Rvalue {
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "Nil";
  }
  virtual void* eval() const override { return nullptr; }
  virtual void sem() override { type->kind=TYPE_pointer; }
};


//pif
class Charconst: public Rvalue {
private:
  char char_const;
public:
  Charconst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Charconst(" << char_const << ")";
  }
  virtual char eval() const override { return char_const; }
  virtual void sem() override { type->kind=TYPE_char; }
};


//pif
class Realconst: public Rvalue {
private:
  double num;
public:
  Realconst(double r): num(r) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Realconst(" << num << ")";
  }
  virtual double eval() const override { return num; }
  virtual void sem() override { type->kind=TYPE_real; }
};


//pif
class Bool: public Rvalue {
private:
  string boolean;
public:
  Bool(string s): boolean(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Bool(" << boolean << ")";
  }
  virtual bool eval() const override { return (boolean == "true"); }
  virtual void sem() override { type->kind=TYPE_boolean; }
};


//pif
class Intconst: public Rvalue {
private:
  int num;
public:
  Intconst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Intconst(" << num << ")";
  }
  virtual int eval() const override { return num; }
  virtual void sem() override { type->kind=TYPE_integer; }
};

// class Dispose1: public Stmt {};
//
// class Dispose: public Stmt {};
//
// class New1: public Stmt {};
//
// class New: public Stmt {};
//
// class Return: public Stmt {};
//
// class Goto: public Stmt {};
//
// class Label1: public Stmt {};


//pif
class While: public Stmt {
private:
  Expr *expr;
  Stmt *stmt;
public:
  While(Expr *e, Stmt *s): expr(e), stmt(s) {}
  ~While() { delete expr; delete stmt; }
  virtual void printOn(std::ostream &out) const override {
    out << "While(" << *expr << ", " << *stmt << ")";
  }
  virtual void sem() override {
    if (expr->type_check(TYPE_boolean)){
      stmt->sem();
    }
    else{
 	exit(1);
    }
  }
  virtual void run() const override {
    while (expr->eval()){
      stmt->run();
    }
  }
};


//pif
class If: public Stmt {
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
public:
  If(Expr *c, Stmt *s1, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2) {}
  ~If() { delete cond; delete stmt1; delete stmt2; }
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
  virtual void sem() override {
    if (cond->type_check(TYPE_boolean)){
      stmt1->sem();
      if (stmt2 != nullptr) stmt2->sem();
    }
    else {
      exit(1);
    }
  }
  virtual void run() const override {
    if (cond->eval())
      stmt1->run();
    else if (stmt2 != nullptr)
      stmt2->run();
  }
};

// class Call: public Stmt {};
//
// class Block: public Stmt {local_list}; ?? λογω body
//
// class Assignment: public Stmt {};
//
// class Expr_list: public Call {};
//
// class Stmt_list: public Block {};
//
// block, type, formal, formal_list, header, var_name_list, var_name_type_list, local, body, program
