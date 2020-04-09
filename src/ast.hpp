#pragma once
#include <cstdlib>
#include <iostream>
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
public:
  bool type_check(myType t) {
    sem();
    return (type->kind == t);
  }
protected:
  Type type;
};

// class Stmt: public AST {};
//
// class Lvalue: public Expr {};

class Rvalue: public Expr {
//sem(), type_check(type t)->bool, eval()
public:
  virtual int eval() const = 0;
protected:
  Type type;
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
private:
  char *var;
  int offset;
};

class Binop: public Rvalue {
public:
  BinOp(Expr *l, char *o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual void sem() override {

//sem(), type_check(type t)->bool, eval()
//+-* 2 int->int, αλλιως real,/ παντα real, div mod δεχονται μονο ιντ->ιντ
//= <> ->bool, παιρνει 2αριθμ, αλλιως ίδιου τυπου (οχι πινακα) συγκρ οι δυαδ αναπ
//or and  bool->bool με βραχυκυκλ
// < > <= >= real/integer->boolean
//ERROR αντι για exit(1)

    if (( left->type_check(TYPE_arrayI) ) || ( left->type_check(TYPE_arrayII) ) || ( right->type_check(TYPE_arrayI) ) || ( right->type_check(TYPE_arrayII) )){
	exit(1);
	}
    if (( left->type_check(TYPE_pointer) ) && !( right->type_check(TYPE_pointer) )){
	exit(1);
	}
    if (( left->type_check(TYPE_char) ) && !( right->type_check(TYPE_char) )){
	exit(1);
	}
    if (( left->type_check(TYPE_boolean) ) && !( right->type_check(TYPE_boolean) )){
	exit(1);
	}
    if ((( left->type_check(TYPE_integer) ) && !( right->type_check(TYPE_integer) )) || (( left->type_check(TYPE_integer) ) && !( right->type_check(TYPE_real) ))){
	exit(1);
	}
    if ((( left->type_check(TYPE_real) ) && !( right->type_check(TYPE_integer) )) || (( left->type_check(TYPE_real) ) && !( right->type_check(TYPE_real) ))){
	exit(1);
	}

    switch (op) {
    case "+": case "-": case "*":
	if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) ){
		type = TYPE_integer; break;
	}
	else if (( left->type_check(TYPE_real) ) || ( right->type_check(TYPE_real) ){
		type = TYPE_real; break;
	}
	else {
		exit(1);
	}
    case "/":
	if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real)){
		type = TYPE_real; break;
	}
	else {
		exit(1);
	}
    case "div": case "mod":
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer)){
		type = TYPE_integer; break;
	}
	else {
		exit(1);
	}
    case "=": case "<>":
      type = TYPE_boolean; break;
    case "or": case "and":
      if (( left->type_check(TYPE_boolean)){
		type = TYPE_boolean; break;
	}
	else {
		exit(1);
	}
    case "<": case ">": case ">=": case "<=":
	if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real)){
		type = TYPE_boolean; break;
	}
	else {
		exit(1);
	}
    }

  }
  virtual int eval() const override {
    switch (op) {
    case "+": return left->eval() + right->eval();
    case "-": return left->eval() - right->eval();
    case "*": return left->eval() * right->eval();
    case "/": return left->eval() / right->eval();
    case "div": return left->eval() / right->eval();
    case "mod": return left->eval() % right->eval();
    case "=": return left->eval() == right->eval();
    case "<>": return left->eval() != right->eval();
    case "or": return left->eval() || right->eval();
    case "and": return left->eval() && right->eval();
    case ">": return left->eval() > right->eval();
    case "<": return left->eval() < right->eval();
    case ">=": return left->eval() >= right->eval();
    case "<=": return left->eval() <= right->eval();
    }
    return 0;  // this will never be reached.
  }
private:
  Expr *left;
  char *op;
  Expr *right;
};

class Unop: public Rvalue {
//με ένα τελούμενο + και - αριθμητικού τύπου, αποτέλεσμα ίδιου τύπου
//not bool->bool
//@ επιστρέφει τη διεύθυνση ενός αντικειμένου. Αν l είναι μια l-value τύπου t, τότε @l
// είναι μια r-value τύπου ^t. Η τιμή της είναι η διεύθυνση του αντικειμένου που αντιστοιχεί στην l
// και είναι πάντα διαφορετική του μηδενικού δείκτη.---> αλλη κλαση;

public:
  UnOp(char *o, Expr *r): op(o), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *right << ")";
  }
  virtual void sem() override {
    if (op=="+") || (op=="-")){
	if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )){
		type = TYPE_integer; break;
	}
	else if (( left->type_check(TYPE_real) ) && ( right->type_check(TYPE_real) )){
		type = TYPE_integer; break;
	}
	else {
		exit(1);
	}
    }
    else if (op=="not"){
	if (( left->type_check(TYPE_boolean) ) && ( right->type_check(TYPE_boolean) )){
		type = TYPE_boolean; break;
	}
	else {
		exit(1);
	}
    }
  virtual int eval() const override {
    switch (op) {
    case "+": return right->eval();
    case "-": return (-1)*right->eval();
    case "not": return !right->eval();
    }
    return 0;  // this will never be reached.
  }
private:
  char *op;
  Expr *right;
};


//pif
class Nil: public Rvalue {
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "Nil";
  }
  virtual void* eval() const override { return nullptr; }
  virtual void sem() override { type = TYPE_pointer; }
};


//pif
class Charconst: public Rvalue {
public:
  Charconst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Charconst(" << char_const << ")";
  }
  virtual char eval() const override { return char_const; }
  virtual void sem() override { type = TYPE_char; }
private:
  char char_const;
};


//pif
class Realconst: public Rvalue {
public:
  Realconst(real r): num(r) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Realconst(" << num << ")";
  }
  virtual real eval() const override { return num; }
  virtual void sem() override { type = TYPE_real; }
private:
  real num;
};


//pif
class Bool: public Rvalue {
public:
  Bool(string s): boolean(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Bool(" << boolean << ")";
  }
  virtual bool eval() const override { return (boolean=="true") }
  virtual void sem() override { type = TYPE_boolean; }
private:
  string boolean;
};


//pif
class Intconst: public Rvalue {
public:
  Intconst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Intconst(" << num << ")";
  }
  virtual int eval() const override { return num; }
  virtual void sem() override { type = TYPE_integer; }
private:
  int num;
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
private:
  Expr *expr;
  Stmt *stmt;
};


//pif
class If: public Stmt {
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
    if (cond->type_check(TYPE_bool)){
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
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
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
