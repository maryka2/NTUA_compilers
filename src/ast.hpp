#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "symbol.hpp"
#include "type.hpp"

extern std::unordered_map<string, SymbolEntry*> globals;

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
  virtual Value eval() const {
    Value value;
    value.integer_value = 0;
    return value;
  }
};


class Stmt: public AST {
public:
  virtual void run() const = 0;
};


class Header: public AST {
  private:
    Type header_type;
    string name;
    std::vector<Formal *> formal_list;
  public:
    // Procedure
    Header(string n, std::vector<Formal*> fl) {
      header_type = type_procedure(false);
      for (Formal *f: fl){
        header_type->u.t_procedure.arg_types.push_back(fl->get_formal_type());
        header_type->u.t_procedure.is_by_ref_arr.push_back(fl->get_is_by_ref());
      }
      name = n;
      formal_list = fl;
    }
    // Function
    Header(string n, std::vector<Formal*> fl, Type rt) {
      header_type = type_function(rt, false);
      for (Formal *f: fl){
        header_type->u.t_function.arg_types.push_back(fl->get_formal_type());
        header_type->u.t_function.is_by_ref_arr.push_back(fl->get_is_by_ref());
      }
      name = n;
      formal_list = fl;
    }
    // Procedure
    Header(string n) {
      header_type = type_procedure(false);
      name = n;
      formal_list = fl;
    }
    // Function
    Header(string n, Type rt) {
      header_type = type_function(rt, false);
      name = n;
      formal_list = fl;
    }
    ~Header() {
      for (Formal *f : formal_list) delete f;
      formal_list.clear();
      delete header_type;
    }
    void printOn(std::ostream &out){
      if (header_type->kind==TYPE_procedure){
        out << "Procedure " << name ;
      }
      else if (header_type->kind==TYPE_function){
        out << "Function" << name << ", result type: ";
        print_type(header_type->u.t_function.result_type);
      }
      if (!formal_list.empty()){
        out << " with arguments "  ;
      }
      for (Formal *f : formal_list){
        out << f->printOn() << " ";
      }
    }
    void set_forward(){
      if (header_type->kind == TYPE_function){
        header_type->u.t_function.is_forward = true;
      }
      else {
        header_type->u.t_procedure.is_forward = true;
      }
    }
    void sem() {
      // first check if in SymbolTable
      SymbolEntry *e = st.lookup(name);
      if (e != nullptr){  //already declared
        if (e->type->kind != header_type->kind) {
          ERROR("Declared both as function and procedure.");
          exit(1);
        }
        if (header_type->kind == TYPE_procedure && (header_type->u.t_procedure.is_forward || !e->type->u.t_procedure.is_forward)){
          ERROR("Procedure " + name + " already declared.");
          exit(1);
        }
        if (header_type->kind == TYPE_function && (header_type->u.t_function.is_forward || !e->type->u.t_function.is_forward)){
          ERROR("Function " + name + " already declared.");
          exit(1);
        }
        if (!equal_types(e->type, header_type)){
          if (header_type->kind == TYPE_function){
            ERROR("Function " + name + " differs from forward declaration");
            exit(1);
          }
          else {
            ERROR("Procedure " + name + " differs from forward declaration");
            exit(1);
          }
        }
        if (e->type->kind == TYPE_function){
          e->type->u.t_function.is_forward = false;
        }
        else {
          e->type->u.t_procedure.is_forward = false;
        }
      }
      // not declared, insert
      else{
        st.insert(name, header_type);
      }
      if ((header_type->kind == TYPE_procedure && header_type->u.t_procedure.is_forward) || (header_type->kind == TYPE_function && header_type->u.t_function.is_forward)) {
        for (Formal* f: formal_list) {
          f->sem();
        }
      }
    }
};


class Block: public AST {
private:
  std::vector<Local *> local_list;
  std::vector<Stmt *> stmt_list;
  int size;
public:
  Block(): local_list(), stmt_list(), size(0) {}
  ~Block() {
    for (Local *l : local_list) delete l;
    for (Stmt *s : stmt_list) delete s;
  }
  void append_local(Local *l) { local_list.push_back(l); }
  void append_stmt(Stmt *s) { stmt_list.push_back(s); }
  void merge(Block *b) {
    stmt_list = b->stmt_list;
    b->stmt_list.clear();
    delete b;
  }
  virtual void printOn(std::ostream &out) const override {
    out << "Block(";
    bool first = true;
    for (Local *l : local_list) {
      if (!first) out << ", ";
      first = false;
      out << *l;
    }
    for (Stmt *s : stmt_list) {
      if (!first) out << ", ";
      first = false;
      out << *s;
    }
    out << ")";
  }
  virtual void sem() override {
    st.openScope();
    for (Local *l : local_list) l->sem();
    for (Stmt *s : stmt_list) s->sem();
    size = st.getSizeOfCurrentScope();
    st.closeScope();
  }
  virtual void run() const override {
    for (int i = 0; i < size; ++i) rt_stack.push_back(0);
    for (Stmt *s : stmt_list) s->run();
    for (int i = 0; i < size; ++i) rt_stack.pop_back();
  }
};


class Formal: public AST {
  private:
    std::vector<Id*> var_name_list;
    Type type;
    bool is_by_ref;
  public:
    Formal(string var_str, std::vector<Id*> vnl, Type t) {
      var_name_list = vnl;
      type = t;
      is_by_ref = true;
    }
    Formal(std::vector<Id*> vnl, Type t) {
      var_name_list = vnl;
      type = t;
      is_by_ref = false;
    }
    ~Formal() {
      for (Id* id: var_name_list) {
        delete id;
      }
      var_name_list.clear();
      delete type;
    }
    void printOn(std::ostream &out) {
      out << "Formal ";
      for (Id* id: var_name_list) {
        id->printOn();
      }
      print_type(type);
    }
    Type get_formal_type() {
      return type;
    }
    bool get_is_by_ref() {
      return is_by_ref;
    }
    void sem() {
      for (Id* id: var_name_list) {
        id->insertIntoCurrentScope();
      }
    }
}


class Local: public Block {
private:
  int local_type;
  string local_type_str;
  std::vector<Id*> name_list;
  Header *header;
  Block *body;
public:
  Local(string lts, std::vector<Id*> nl) {
    local_type = 0;
    local_type_str = lts;
    name_list = nl;
  }
  Local(Header *h, Block *b) {
    local_type = 1;
    header = h;
    body = b;
  }
  Local(Header *h) {
    local_type = 2;
    header = h;
    header->set_forward();
  }

  virtual void sem() override {
    if (local_type == 0) {
      if (local_type_str == "var") {
        for (Id* id : name_list) {
          id->insertIntoCurrentScope();
        }
      }
      else if (local_type_str == "label") {
        for (Id* id : name_list) {
          id->set_type(type_label());
          id->insertIntoCurrentScope();
        }
      }
      else {
        exit(1);
      }
    }
    else {
      st.openScope();
      header->sem();
      if (local_type == 1) {
        body->sem();
      }
      st.closeScope();
    }
  }
};


class Lvalue: public Expr {
public:
  virtual Value eval() const override {
    exit(1);
  }
};


class Rvalue: public Expr {
protected:
  Type type;
//sem(), type_check(type t)->bool, eval()
public:
  virtual Value eval() const override {
    exit(1);
  }
};


//pif
class Id: public Lvalue {
private:
  string var;
public:
  Id(string v): var(v) {

  }
  void set_type(Type t) {
    type->kind = t;
  }
  void insertIntoCurrentScope() {
    st.insert(var, type);
  }
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << var << ")";
  }
  virtual Value eval() const override {
    return globals[var]->value;
  }
  virtual void sem() override {
    SymbolEntry *e = st.lookup(var);
    if (e != null) {
      type = e->type;
    }
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
        type->kind = TYPE_integer;
      }
      else if (( left->type_check(TYPE_real) ) || ( right->type_check(TYPE_real) )) {
        type->kind = TYPE_real;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "/")) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type->kind = TYPE_real;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "div") || !std::strcmp(op, "mod")) {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type->kind = TYPE_integer;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "=") || !std::strcmp(op, "<>")) {
      type->kind = TYPE_boolean;
    }
    else if (!std::strcmp(op, "or") || !std::strcmp(op, "and")) {
      if (( left->type_check(TYPE_boolean) )) {
        type->kind = TYPE_boolean;
      }
      else {
        exit(1);
      }
    }
    else if (!std::strcmp(op, "<") || !std::strcmp(op, ">") || !std::strcmp(op, ">=") || !std::strcmp(op, "<=")) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type->kind = TYPE_boolean;
      }
      else {
        exit(1);
      }
    }
    else {
      exit(1);
    }

  }
  virtual Value eval() const override {
    Value value;
    if (!std::strcmp(op, "+")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.integer_value = left->eval().integer_value + right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().integer_value + right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.real_value = left->eval().real_value + right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().real_value + right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "-")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.integer_value = left->eval().integer_value - right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().integer_value - right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.real_value = left->eval().real_value - right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().real_value - right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "*")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.integer_value = left->eval().integer_value * right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().integer_value * right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.real_value = left->eval().real_value * right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().real_value * right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "/")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.real_value = left->eval().integer_value / right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().integer_value / right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.real_value = left->eval().real_value / right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.real_value = left->eval().real_value / right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "div")) {
      value.integer_value = left->eval().integer_value / right->eval().integer_value;
    }
    if (!std::strcmp(op, "mod")) {
      value.integer_value = left->eval().integer_value % right->eval().integer_value;
    }
    if (!std::strcmp(op, "=")) {
      if ( left->type_check(TYPE_integer) ) {
        if ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value == right->eval().integer_value;
        }
        else {
          value.boolean_value = left->eval().integer_value == right->eval().real_value;
        }
      }
      if ( left->type_check(TYPE_real) ) {
        if ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value == right->eval().integer_value;
        }
        else {
          value.boolean_value = left->eval().real_value == right->eval().real_value;
        }
      }
      if ( left->type_check(TYPE_boolean) ) {
        value.boolean_value = left->eval().boolean_value == right->eval().boolean_value;
      }
      if ( left->type_check(TYPE_char) ) {
        value.boolean_value = left->eval().char_value == right->eval().char_value;
      }
      if ( left->type_check(TYPE_pointer) ) {
        value.boolean_value = left->eval().pointer_value == right->eval().pointer_value;
      }
      if ( left->type_check(TYPE_string) ) {
        value.boolean_value = !std::strcmp(left->eval().string_value, right->eval().string_value);
      }
    }
    if (!std::strcmp(op, "<>")) {
      if ( left->type_check(TYPE_integer) ) {
        if ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value != right->eval().integer_value;
        }
        else {
          value.boolean_value = left->eval().integer_value != right->eval().real_value;
        }
      }
      if ( left->type_check(TYPE_real) ) {
        if ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value != right->eval().integer_value;
        }
        else {
          value.boolean_value = left->eval().real_value != right->eval().real_value;
        }
      }
      if ( left->type_check(TYPE_boolean) ) {
        value.boolean_value = left->eval().boolean_value != right->eval().boolean_value;
      }
      if ( left->type_check(TYPE_char) ) {
        value.boolean_value = left->eval().char_value != right->eval().char_value;
      }
      if ( left->type_check(TYPE_pointer) ) {
        value.boolean_value = left->eval().pointer_value != right->eval().pointer_value;
      }
      if ( left->type_check(TYPE_string) ) {
        value.boolean_value = std::strcmp(left->eval().string_value, right->eval().string_value);
      }
    }
    if (!std::strcmp(op, "or")) {
      value.boolean_value = left->eval().boolean_value || right->eval().boolean_value;
    }
    if (!std::strcmp(op, "and")) {
      value.boolean_value = left->eval().boolean_value && right->eval().boolean_value;
    }
    if (!std::strcmp(op, ">")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value > right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().integer_value > right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value > right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().real_value > right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "<")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value < right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().integer_value < right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value < right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().real_value < right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, ">=")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value >= right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().integer_value >= right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value >= right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().real_value >= right->eval().real_value;
        }
      }
    }
    if (!std::strcmp(op, "<=")) {
      if ( left->type_check(TYPE_integer) ) {
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().integer_value <= right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().integer_value <= right->eval().real_value;
        }
      }
      else if ( left->type_check(TYPE_real) ){
        if  ( right->type_check(TYPE_integer) ) {
          value.boolean_value = left->eval().real_value <= right->eval().integer_value;
        }
        else if ( right->type_check(TYPE_real) ){
          value.boolean_value = left->eval().real_value <= right->eval().real_value;
        }
      }
    }
    return value;
  }
};

class UnOp: public Rvalue {
  virtual Value eval() const override {
    Value value;
    if (!std::strcmp(op, "+")) {
      if (right->type_check(TYPE_integer)) {
        value.integer_value = right->eval().integer_value;
      }
      if (right->type_check(TYPE_real)) {
        value.real_value = right->eval().real_value;
      }
    }
    else if (!std::strcmp(op, "-")) {
      if (right->type_check(TYPE_integer)) {
        value.integer_value = (-1)*right->eval().integer_value;
      }
      if (right->type_check(TYPE_real)) {
        value.real_value = (-1)*right->eval().real_value;
      }
    }
    else if (!std::strcmp(op, "not")) {
      value.boolean_value = !right->eval().boolean_value;
    }
    else if (!std::strcmp(op, "@"){
      value.pointer_value = &right
    }
    return value;
  }
};

//pif
class Nil: public Rvalue {
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "Nil";
  }
  virtual Value eval() const override {
    Value value;
    value.pointer_value = nullptr;
    return value;
  }
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
  virtual Value eval() const override {
    Value value;
    value.char_value = char_const;
    return value;
  }
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
  virtual Value eval() const override {
    Value value;
    value.real_value = num;
    return value;
  }
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
  virtual Value eval() const override {
    Value value;
    value.boolean_value = (boolean == "true");
    return value;
  }
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
  virtual Value eval() const override {
    Value value;
    value.integer_value = num;
    return value;
  }
  virtual void sem() override { type->kind=TYPE_integer; }
};


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
    while (expr->eval().boolean_value){
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
    if (cond->eval().boolean_value)
      stmt1->run();
    else if (stmt2 != nullptr)
      stmt2->run();
  }
};


class Call: public Stmt {
  private:
    string name;
    std::vector<Expr*> expr_list;
  public:
    Call(string n) {
      name = n;
    }
    Call(string n, std::vector<Expr*> el) {
      name = n;
      expr_list = el;
    }
    ~Call() {
      for (Expr *r : expr_list) delete e;
      expr_list.clear();
    }
    void printOn(std::ostream &out){
      out << "Call of procedure or function '" << name << "'";
      if (!expr_list.empty()){
        out << " with arguments ";
      }
      for (Expr *expr : expr_list){
        expr->printOn();
        out << " ";
      }
    }
    void sem() {
      // first check if in SymbolTable
      Symbolentry *e = st.lookup(name);
      if (e == nullptr) {
        ERROR("Called function or procedure '" + name + "' was not defined.");
        exit(1);
      }
      if (e->type->kind == TYPE_procedure){
        if (e->type->u.t_procedure.is_forward) {
          ERROR("Called procedure '" + name + "' has only been forward declared.");
          exit(1);
        }
        if (expr_list.size() != e->type->u.t_procedure.arg_types.size()) {
          ERROR("Different amount of arguments given for procedure '" + name + "'.");
          exit(1);
        }
        for (int arg_idx = 0; arg_idx < expr_list.size(); ++arg_idx) {
          if (!equal_types(expr_list[arg_idx]->type, e->type->u.t_procedure.arg_types[arg_idx])) {
            ERROR("Not right type of argument in call of procedure '" + name + "'.");
            exit(1);
          }
        }
      }
      else if (e->type->kind == TYPE_function){
        if (e->type->u.t_function.is_forward) {
          ERROR("Called function '" + name + "' has only been forward declared.");
          exit(1);
        }
        if (expr_list.size() != e->type->u.t_function.arg_types.size()) {
          ERROR("Different amount of arguments given for function '" + name + "'.");
          exit(1);
        }
        for (int arg_idx = 0; arg_idx < expr_list.size(); ++arg_idx) {
          if (!equal_types(expr_list[arg_idx]->type, e->type->u.t_function.arg_types[arg_idx])) {
            ERROR("Not right type of argument in call of function '" + name + "'.");
            exit(1);
          }
        }
      }
      else {
        ERROR("Call of '" + name + "' with type different than function or procedure.");
        exit(1);
      }
    }
};

//
// class Assignment: public Stmt {};
//
// class Pointer: public Lvalue {};
//
// class Array: public Lvalue {};
//
// class Stringconst: public Lvalue {};
//
// class Result: public Lvalue {};
//
// class Dispose: public Stmt {};
//
// class New: public Stmt {};
//
// class Return: public Stmt {};
//
// class Goto: public Stmt {};
//
// class Label: public Stmt {};
