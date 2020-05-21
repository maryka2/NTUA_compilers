#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "symbol.hpp"
#include "type.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
//#include <llvm/Transforms/Scalar/GVN.h>

using namespace llvm;

#define ERROR(msg) { std::cerr << "ERROR: " << msg << "\n"; exit(1); }

/*void ERROR (const string msg)
{
  std::cerr << "ERROR: " << msg << "\n";
  exit(1);
}*/

extern std::unordered_map<string, SymbolEntry*> globals;


inline std::ostream& operator<<(std::ostream &out, Type t) {
  if ( t->kind == TYPE_integer ) {
    out << "integer \n";
  }
  else if ( t->kind == TYPE_real ) {
    out << "real \n";
  }
  else if ( t->kind == TYPE_boolean ){
     out << "boolean \n";
   }
  else if ( t->kind == TYPE_char ){
     out << "char \n";
   }
  else if ( t->kind == TYPE_arrayI ){
     out << "arrayI \n";
   }
  else if ( t->kind == TYPE_arrayII ){
     out << "arrayII \n";
   }
  else if ( t->kind == TYPE_pointer ){
     out << "pointer \n";
  }
  else if (t->kind == TYPE_function) {
    out << "function \n";
  }
  else if (t->kind == TYPE_procedure) {
    out << "procedure \n";
  }
  else if (t->kind == TYPE_label) {
    out << "label \n";
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


class Stmt: public AST {
  //this is intentionally left empty
};


class Expr: public AST {
protected:
  Type type;
private:
  bool is_sem_called = false;
public:
  bool type_check(myType t) {
    if (!is_sem_called){
      is_sem_called = true;
      sem();
    }
    return (type->kind == t);
  }
  virtual Value eval() {
    Value value;
    value.integer_value = 0;
    return value;
  }
  Type get_expr_type() {
    return type;
  }
};


class Expr_vector{
private:
  std::vector<Expr *> expr_list;
public:
  Expr_vector(Expr *e){
    expr_list.push_back(e);
  }
  void append_expr(Expr *e){
    expr_list.push_back(e);
  }
  std::vector<Expr *> get_vector(){
    return expr_list;
  }
};


class Lvalue: public Expr {
  //this is intentionally left empty
};


class Rvalue: public Expr {
  //this is intentionally left empty
};


class Id: public Lvalue {
private:
  string var;
public:
  Id(string v): var(v) {}
  ~Id() {
    //this is intentionally left empty
  }
  void set_type(Type t) {
    type = t;
  }
  void insertIntoCurrentScope() {
    st.insert(var, type);
  }
  void printOn(std::ostream &out) const override {
    out << "Id(" << var << ")\n";
  }
  Value eval() override {
    return globals[var]->value;
  }
  void sem() override {
    SymbolEntry *e = st.lookup(var);
    if (e != nullptr) {
      type = e->type;
    }
  }
};


class Id_vector{
private:
  std::vector<Id *> id_list;
public:
  Id_vector(Id *id){
    id_list.push_back(id);
  }
  void append_id(Id *id){
    id_list.push_back(id);
  }
  std::vector<Id *> get_vector(){
    return id_list;
  }
};


class Formal: public AST {
private:
  std::vector<Id*> var_name_list;
  Type type;
  bool is_by_ref;
public:
  Formal() {}
  Formal(string var_str, Id_vector *vnv, Type t) : type(t), is_by_ref(true){
    var_name_list = vnv->get_vector();
  }
  Formal(Id_vector *vnv, Type t) : type(t), is_by_ref(false){
    var_name_list = vnv->get_vector();
  }
  ~Formal() {
    for (Id* id: var_name_list) {
      delete id;
    }
    var_name_list.clear();
    delete_type(type);
  }
  void printOn(std::ostream &out) const override {
    out << "Formal ";
    for (Id* id: var_name_list) {
      out << *id << ' ';
    }
    print_type(type);
    out << "\n";
  }
  Type get_formal_type() {
    return type;
  }
  bool get_is_by_ref() {
    return is_by_ref;
  }
  void sem() override {
    for (Id* id: var_name_list) {
      id->insertIntoCurrentScope();
    }
  }
};


class Formal_vector{
private:
  std::vector<Formal *> formal_list;
public:
  Formal_vector(Formal *f){
    formal_list.push_back(f);
  }
  void append_formal(Formal *f){
    formal_list.push_back(f);
  }
  std::vector<Formal *> get_vector(){
    return formal_list;
  }
};


class Header: public AST {
private:
  Type header_type;
  string name;
  std::vector<Formal *> formal_list;
public:
  // Procedure
  Header(string n, Formal_vector *fv) : name(n) {
    header_type = type_procedure(false);
    formal_list = fv->get_vector();
    for (Formal *f: formal_list){
      header_type->u.t_procedure.arg_types.push_back(f->get_formal_type());
      header_type->u.t_procedure.is_by_ref_arr.push_back(f->get_is_by_ref());
    }
  }
  // Function
  Header(string n, Formal_vector *fv, Type rt) : name(n) {
    header_type = type_function(rt, false);
    formal_list = fv->get_vector();
    for (Formal *f: formal_list){
      header_type->u.t_function.arg_types.push_back(f->get_formal_type());
      header_type->u.t_function.is_by_ref_arr.push_back(f->get_is_by_ref());
    }
  }
  // Procedure
  Header(string n) : name(n) {
    header_type = type_procedure(false);
  }
  // Function
  Header(string n, Type rt) : name(n) {
    header_type = type_function(rt, false);
  }
  ~Header() {
    for (Formal *f : formal_list) delete f;
    formal_list.clear();
    delete_type(header_type);
  }
  void printOn(std::ostream &out) const override{
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
      out << *f << " ";
    }
    out << "\n";
  }
  void set_forward(){
    if (header_type->kind == TYPE_function){
      header_type->u.t_function.is_forward = true;
    }
    else {
      header_type->u.t_procedure.is_forward = true;
    }
  }
  void sem_outter_scope() {
    // first check if in SymbolTable
    SymbolEntry *e = st.lookup(name);
    if (e != nullptr){  //already declared
      if (e->type->kind != header_type->kind) {
        ERROR("Declared both as function and procedure.");
      }
      if (header_type->kind == TYPE_procedure && (header_type->u.t_procedure.is_forward || !e->type->u.t_procedure.is_forward)){
        ERROR("Procedure " + name + " already declared.");
      }
      if (header_type->kind == TYPE_function && (header_type->u.t_function.is_forward || !e->type->u.t_function.is_forward)){
        ERROR("Function " + name + " already declared.");
      }
      if (!equal_types(e->type, header_type)){
        if (header_type->kind == TYPE_function){
          ERROR("Function " + name + " differs from forward declaration");
        }
        else {
          ERROR("Procedure " + name + " differs from forward declaration");
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
  }
  void sem() override {
    if ((header_type->kind == TYPE_procedure && !header_type->u.t_procedure.is_forward) || (header_type->kind == TYPE_function && !header_type->u.t_function.is_forward)) {
      if (header_type->kind == TYPE_function){
        st.insert("result", header_type->u.t_function.result_type);
      }
      for (Formal* f: formal_list) {
        f->sem();
      }
    }
  }
};


class Block;


class Local: public AST {
private:
  int local_type = -1;
  string local_type_str;
  std::vector<Id*> name_list;
  Header *header;
public:
  Local(string lts, Id_vector *nv) : local_type(0), local_type_str(lts) {
    name_list = nv->get_vector();
  }
  Local(Header *h) : local_type(1), header(h){
    header->set_forward();
  }
  Local(Header *h, Block *b){}
  Local(){}
  ~Local(){
    if (local_type == 0){
      for (Id* id : name_list){
        delete id;
      }
    }
    else if (local_type == 1){
      delete header;
    }
  }
  virtual void printOn(std::ostream &out) const override {
    out << "Local ";
    if (local_type == 0){
      out << local_type_str << " ";
      for (Id* id : name_list){
        out << *id << " ";
      }
    }
    else {
      out << *header << " ";
    }
    out << "\n";
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
    }
    else {
      header->sem_outter_scope();
    }
  }
};


class Block: public Stmt {
private:
  std::vector<Local *> local_list;
  std::vector<Stmt *> stmt_list;
  int size;
  bool is_program = false;
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
    out << ")\n";
  }
  void block_is_program(){
    is_program = true;
  }
  virtual void sem() override {
    st.openScope();
    if (is_program){
      Id *id = new Id("n");
      id->set_type(type_integer());
      (new Header("writeInteger",  new Formal_vector(new Formal(new Id_vector(id), type_integer())) ))->sem_outter_scope();
      id = new Id("b");
      id->set_type(type_boolean());
      (new Header("writeBoolean",  new Formal_vector(new Formal(new Id_vector(id), type_boolean())) ))->sem_outter_scope();
      id = new Id("c");
      id->set_type(type_char());
      (new Header("writeChar",  new Formal_vector(new Formal(new Id_vector(id), type_char())) ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("writeReal",  new Formal_vector(new Formal(new Id_vector(id), type_real())) ))->sem_outter_scope();
      id = new Id("s");
      id->set_type(type_arrayII(type_char()));
      (new Header("writeString",  new Formal_vector(new Formal("var", new Id_vector(id), type_arrayII(type_char()))) ))->sem_outter_scope();
      (new Header("readInteger",  type_integer() ))->sem_outter_scope();
      (new Header("readBoolean",  type_boolean() ))->sem_outter_scope();
      (new Header("readChar",  type_char() ))->sem_outter_scope();
      (new Header("readReal",  type_real() ))->sem_outter_scope(); 
      Id *id2 = new Id("s");
      id2->set_type(type_arrayII(type_char()));
      Formal_vector *fv = new Formal_vector(new Formal("var", new Id_vector(id2), type_arrayII(type_char())));    
      Id *id1 = new Id("size");
      id1->set_type(type_integer());
      fv->append_formal(new Formal( new Id_vector(id1), type_integer()));
      (new Header("readString", fv))->sem_outter_scope();
      id = new Id("n");
      id->set_type(type_integer());
      (new Header("abs",  new Formal_vector(new Formal(new Id_vector(id), type_integer())), type_integer() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("fabs",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("sqrt",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("sin",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("cos",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("tan",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("arctan",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("exp",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      id = new Id("r");
      id->set_type(type_real());
      (new Header("ln",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope();
      (new Header("pi",  type_real() ))->sem_outter_scope();

    }
    for (Local *l : local_list) l->sem();
    for (Stmt *s : stmt_list) s->sem();
    size = st.getSizeOfCurrentScope();
    st.closeScope();
  }
  // virtual void run() override {
    //   for (int i = 0; i < size; ++i) rt_stack.push_back(0);
    //   for (Stmt *s : stmt_list) s->run();
    //   for (int i = 0; i < size; ++i) rt_stack.pop_back();
    // }
  };


class Local_after_block: public Local {
private:
  Header *header;
  Block *body;
public:
  Local_after_block(Header *h, Block *b) : header(h), body(b) {}
  ~Local_after_block(){
    delete header;
    delete body;
  }
  void printOn(std::ostream &out) const override {
    out << "Local " << *header << " " << *body << "\n";
  }
  void sem() override {
    header->sem_outter_scope();
    st.openScope();
    header->sem();
    body->sem();
    st.closeScope();
  }
};


class Dereference: public Lvalue {
private:
  Expr *expr;
public:
  Dereference(Expr *e) : expr(e) {}
  ~Dereference(){
    delete expr;
  }
  void printOn(std::ostream &out) const override {
    out << "Dereference " << *expr << "\n";
  }
  void sem() override {
    if (!expr->type_check(TYPE_pointer)){
      ERROR("Attempt of dereferencing non-pointer expression");
    }
    type = expr->get_expr_type()->u.t_pointer.type;
  }
};


class Array: public Lvalue {
private:
  Lvalue *lvalue;
  Expr *expr;
public:
  Array(Lvalue *lv, Expr *e) : lvalue(lv), expr(e) {}
  ~Array(){
    delete lvalue;
    delete expr;
  }
  void printOn(std::ostream &out) const override {
    out << "Array " << *lvalue << " [" << *expr << "]\n";
  }
  void sem() override {
    if (!expr->type_check(TYPE_integer)){
      ERROR("Non-integer value used for array indexing.");
    }
    if (lvalue->type_check(TYPE_arrayI)){
      type = lvalue->get_expr_type()->u.t_arrayI.type;
    }
    else if (lvalue->type_check(TYPE_arrayII)){
      type = lvalue->get_expr_type()->u.t_arrayII.type;
    }
    else {
      ERROR("Indexing non-array lvalue.");
    }
  }
};


class Stringconst: public Lvalue {
private:
  string str;
public:
  Stringconst(string s) : str(s) {}
  ~Stringconst(){
    // this is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Stringconst " << str;
  }
  void sem() override {
    type = type_arrayI(str.length() + 1, type_char());
  }
};


class BinOp: public Rvalue {
private:
  Expr *left;
  string op;
  Expr *right;
public:
  BinOp(Expr *l, string o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")\n";
  }
  void sem() override {
    if (( left->type_check(TYPE_arrayI) ) || ( left->type_check(TYPE_arrayII) ) || ( right->type_check(TYPE_arrayI) ) || ( right->type_check(TYPE_arrayII) )) {
      ERROR("1 BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_pointer) !=  right->type_check(TYPE_pointer) ) {
      ERROR("2 BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_char) != right->type_check(TYPE_char) ) {
      ERROR("3 BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_boolean) != right->type_check(TYPE_boolean) ) {
      ERROR("4 BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( (left->type_check(TYPE_integer) || left->type_check(TYPE_real)) != (right->type_check(TYPE_integer) || right->type_check(TYPE_real) )){
      ERROR("5 BinOp operands have incompatible type for operation '" + op + "'");
    }
    if (op == "+" || op == "-" || op == "*") {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type = type_integer();
      }
      else if (( left->type_check(TYPE_real) ) || ( right->type_check(TYPE_real) )) {
        type = type_real();
      }
      else {
        ERROR("6 BinOp operands have incompatible type for operation '" + op + "'");
      }
    }
    else if ( op == "/" ) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type = type_real();
      }
      else {
        ERROR("BinOp operands have incompatible type for operation '" + op + "'");
      }
    }
    else if ( op == "div" ||  op == "mod" ) {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type = type_integer();
      }
      else {
        ERROR("BinOp operands have incompatible type for operation '" + op + "'");
      }
    }
    else if ( op == "=" ||  op == "<>") {
      type = type_boolean();
    }
    else if ( op == "or" ||  op == "and") {
      if (( left->type_check(TYPE_boolean) )) {
        type = type_boolean();
      }
      else {
        ERROR("BinOp operands have incompatible type for operation '" + op + "'");
      }
    }
    else if ( op == "<" ||  op == ">" ||  op == ">=" ||  op == "<=" ) {
      if (( left->type_check(TYPE_integer) ) || ( left->type_check(TYPE_real) )) {
        type = type_boolean();
      }
      else {
        ERROR("BinOp operands have incompatible type for operation '" + op + "'");
      }
    }
  }
  Value eval() override {
    Value value;
    if ( op == "+") {
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
    if ( op == "-") {
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
    if ( op == "*" ) {
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
    if ( op == "/" ) {
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
    if ( op == "div") {
      value.integer_value = left->eval().integer_value / right->eval().integer_value;
    }
    if ( op == "mod") {
      value.integer_value = left->eval().integer_value % right->eval().integer_value;
    }
    if ( op == "=") {
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
      if (is_string(left->get_expr_type())) {
        value.boolean_value = !equal_strings(left->get_expr_type(), right->get_expr_type(), left->eval(), right->eval());
      }
    }
    if ( op == "<>" ) {
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
      if ( is_string(left->get_expr_type() ) ) {
        value.boolean_value = equal_strings(left->get_expr_type(), right->get_expr_type(), left->eval(), right->eval());
      }
    }
    if ( op == "or" ) {
      value.boolean_value = left->eval().boolean_value || right->eval().boolean_value;
    }
    if ( op == "and" ) {
      value.boolean_value = left->eval().boolean_value && right->eval().boolean_value;
    }
    if ( op == ">" ) {
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
    if ( op == "<" ) {
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
    if ( op == ">=" ) {
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
    if ( op == "<=" ) {
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
private:
  string op;
  Expr *right;
public:
  UnOp(string o, Expr *r): op(o), right(r) {}
  ~UnOp() {
    delete right;
  }
  void printOn(std::ostream &out) const override {
    out << op << "(" << *right << ")\n";
  }
  void sem() override {
    if ( op == "+" ||  op == "-" ) {
      if (right->type_check(TYPE_integer)) {
        type = type_integer();
      }
      else if (right->type_check(TYPE_real)) {
        type = type_real();
      }
      else{
        ERROR("UnOp operand has incompatible type for operation '" + op + "'");
      }
    }
    else if ( op == "not") {
      if (right->type_check(TYPE_boolean)) {
        type = type_boolean();
      }
      else{
        ERROR("UnOp operand has incompatible type for operation '" + op + "'");
      }
    }
  }
  Value eval() override {
    Value value;
    if ( op == "+" ) {
      if (right->type_check(TYPE_integer)) {
        value.integer_value = right->eval().integer_value;
      }
      if (right->type_check(TYPE_real)) {
        value.real_value = right->eval().real_value;
      }
    }
    else if ( op == "-" ) {
      if (right->type_check(TYPE_integer)) {
        value.integer_value = (-1)*right->eval().integer_value;
      }
      if (right->type_check(TYPE_real)) {
        value.real_value = (-1)*right->eval().real_value;
      }
    }
    else if ( op == "not" ) {
      value.boolean_value = !right->eval().boolean_value;
    }
    else if ( op == "@" ) {
      value.pointer_value = right;
    }
    return value;
  }
};


class Nil: public Rvalue {
public:
  Nil() {}
  void printOn(std::ostream &out) const override {
    out << "Nil\n";
  }
  Value eval() override {
    Value value;
    value.pointer_value = nullptr;
    return value;
  }
  void sem() override {
    type = type_pointer();
  }
};


class Charconst: public Rvalue {
private:
  char char_const;
public:
  Charconst(char c): char_const(c) {}
  ~Charconst(){
    // This is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Charconst(" << char_const << ")\n";
  }
  Value eval() override {
    Value value;
    value.char_value = char_const;
    return value;
  }
  void sem() override {
    type = type_char();
  }
};


class Realconst: public Rvalue {
private:
  double num;
public:
  Realconst(double r): num(r) {}
  ~Realconst(){
    // This is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Realconst(" << num << ")\n";
  }
  Value eval() override {
    Value value;
    value.real_value = num;
    return value;
  }
  void sem() override {
    type = type_real();
  }
};


class Bool: public Rvalue {
private:
  string boolean;
public:
  Bool(string s): boolean(s) {}
  ~Bool(){
    // This is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Bool(" << boolean << ")\n";
  }
  Value eval() override {
    Value value;
    value.boolean_value = (boolean == "true");
    return value;
  }
  void sem() override {
    type = type_boolean();
  }
};


class Intconst: public Rvalue {
private:
  int num;
public:
  Intconst(int n): num(n) {}
  ~Intconst(){
    // This is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Intconst(" << num << ")\n";
  }
  Value eval() override {
    Value value;
    value.integer_value = num;
    return value;
  }
  void sem() override {
    type = type_integer();
  }
};


class EmptyStmt: public Stmt {
public:
  EmptyStmt() {}
  ~EmptyStmt() {}
  void printOn(std::ostream &out) const override {
    out << "EmptyStmt\n";
  }
  void sem() override {
    // This is intentionally left empty.
  }
};


class While: public Stmt {
private:
  Expr *expr;
  Stmt *stmt;
public:
  While(Expr *e, Stmt *s): expr(e), stmt(s) {}
  ~While() {
    delete expr;
    delete stmt;
  }
  void printOn(std::ostream &out) const override {
    out << "While(" << *expr << ", " << *stmt << ")\n";
  }
  void sem() override {
    if (!expr->type_check(TYPE_boolean)){
      ERROR("Condition of while statement is of non-boolean type");
    }
    stmt->sem();
  }
  // void run() override {
  //   while (expr->eval().boolean_value){
  //     stmt->run();
  //   }
  // }
};


class If: public Stmt {
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
public:
  If(Expr *c, Stmt *s1, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2) {}
  ~If() {
    delete cond;
    delete stmt1;
    delete stmt2;
  }
  void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")\n";
  }
  void sem() override {
    if (!cond->type_check(TYPE_boolean)){
      ERROR("Condition of if is of non-boolean type.");
    }
    stmt1->sem();
    if (stmt2 != nullptr) stmt2->sem();
  }
  // void run() override {
  //   if (cond->eval().boolean_value)
  //     stmt1->run();
  //   else if (stmt2 != nullptr)
  //     stmt2->run();
  // }
};


class Call: public Stmt, public Rvalue {
private:
  string name;
  std::vector<Expr*> expr_list;
  bool is_stmt=false;
public:
  Call(string n) : name(n) {}
  Call(string n, Expr_vector *ev) : name(n) {
    expr_list = ev->get_vector();
  }
  ~Call() {
    for (Expr *e : expr_list) {
      delete e;
    }
    expr_list.clear();
  }
  void printOn(std::ostream &out) const override{
    out << "Call of procedure or function '" << name << "'";
    if (!expr_list.empty()){
      out << " with arguments ";
    }
    for (Expr *expr : expr_list){
      out << *expr << ' ';
    }
    out << "\n";
  }
  void is_statement() {
    is_stmt = true;
  }
  void sem() override {
    // first check if in SymbolTable
    SymbolEntry *e = st.lookup(name);
    if (e == nullptr) {
      ERROR("Called function or procedure '" + name + "' was not defined.");
    }
    for (Expr* ex : expr_list){
      ex->sem();
    }
    if (e->type->kind == TYPE_procedure){
      if (!is_stmt){
        ERROR("Procedure called as rvalue.");
      }
      if (expr_list.size() != e->type->u.t_procedure.arg_types.size()) {
        ERROR("Different amount of arguments given for procedure '" + name + "'.");
      }
      for (unsigned int arg_idx = 0; arg_idx < expr_list.size(); ++arg_idx) {
        if (!assignable_types(expr_list[arg_idx]->get_expr_type(), e->type->u.t_procedure.arg_types[arg_idx])) {
          ERROR("Not right type of argument in call of procedure '" + name + "'.");
        }
      }
    }
    else if (e->type->kind == TYPE_function){
      if (is_stmt){
        ERROR("Function called as a statement.");
      }
      if (expr_list.size() != e->type->u.t_function.arg_types.size()) {
        ERROR("Different amount of arguments given for function '" + name + "'.");
      }
      for (unsigned int arg_idx = 0; arg_idx < expr_list.size(); ++arg_idx) {
        if (!assignable_types(expr_list[arg_idx]->get_expr_type(), e->type->u.t_function.arg_types[arg_idx])) {
          ERROR("Not right type of argument in call of function '" + name + "'.");
        }
      }
      type = e->type->u.t_function.result_type;
    }
    else {
      ERROR("Call of '" + name + "' with type different than function or procedure.");
    }
  }
};


class Assignment: public Stmt {
private:
  Lvalue *lvalue;
  Expr *expr;
public:
  Assignment(Lvalue *lv, Expr *e) : lvalue(lv), expr(e) {}
  ~Assignment(){
    delete lvalue;
    delete expr;
  }
  void printOn(std::ostream &out) const override {
    out << "Assignment " << *lvalue << " = " << *expr << "\n";
  }
  void sem() override{
    lvalue->sem();
    expr->sem();
    Type lvalue_type = lvalue->get_expr_type();
    Type expr_type = expr->get_expr_type();
    if (!assignable_types(lvalue_type, expr_type)){
      ERROR("Assignement of wrong type.");
    }
  }
};


class Label: public Stmt {
private:
  string label_name;
  Stmt *stmt;
public:
  Label(string ln, Stmt *s) : label_name(ln), stmt(s) {}
  ~Label() {
    delete stmt;
  }
  void printOn(std::ostream &out) const override {
    out << "Label '" << label_name << "' with statement ";
    stmt->printOn(out);
    out << "\n";
  }
  void sem() override {
    SymbolEntry *e = st.lookup(label_name);
    if (e == nullptr || e->type->kind != TYPE_label) {
      ERROR(label_name + " is not a label in this scope.");
    }
    if (e->type->u.t_label.is_defined) {
      ERROR("Label " + label_name + " already defined.");
    }
    e->type->u.t_label.is_defined = true;
    stmt->sem();
  }
};


class Goto: public Stmt {
private:
  string label_name;
public:
  Goto(string ln) : label_name(ln) {}
  ~Goto() {
    // this is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Goto '" << label_name << "'\n";
  }
  void sem() override {
    SymbolEntry *e = st.lookup(label_name);
    if (e == nullptr || e->type->kind != TYPE_label) {
      ERROR("Label '" + label_name + "' not defined.");
    }
    e->type->u.t_label.is_called = true;
  }
};


class Return: public Stmt {
public:
  Return() {
    // this is intentionally left empty
  }
  ~Return() {
    // this is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Return\n";
  }
  void sem() override {
    // this is intentionally left empty
  }
};


class New: public Stmt {
private:
  Lvalue *lvalue;
  Expr *expr;
  bool is_array;
public:
  New(Lvalue *lv) : lvalue(lv), is_array(false) {}
  New(Expr *e, Lvalue *lv) : lvalue(lv), expr(e), is_array(true) {}
  ~New() {
    delete lvalue;
    delete expr;
  }
  void printOn(std::ostream &out) const override {
    out << "New";
    if (is_array) {
      out << " [" << *expr << "]";
    }
    out << " " << *lvalue << "\n";
  }
  void sem() override {
    if (!lvalue->type_check(TYPE_pointer)) {
      ERROR("Attempt of dynamic memory allocation to non-pointer lvalue.");
    }
    if (is_array) {
      expr->sem();
      if (lvalue->get_expr_type()->u.t_pointer.type->kind != TYPE_arrayI) {
        ERROR("Lvalue of new [] is not an arrayI.");
      }
      if (expr->get_expr_type()->kind != TYPE_integer) {
        ERROR("Indexing with non-integer expression.");
      }
    }
  }
};


class Dispose: public Stmt {
private:
  Lvalue *lvalue;
  bool is_array;
public:
  Dispose(string dispose_type_str, Lvalue *lv) : lvalue(lv), is_array((dispose_type_str == "array_pointer")) {}
  ~Dispose() {
    delete lvalue;
  }
  void printOn(std::ostream &out) const override {
    out << "Dispose";
    if (is_array) {
      out << " []";
    }
    out << " " << *lvalue << "\n";
  }
  void sem() override {
    if (!lvalue->type_check(TYPE_pointer)) {
      ERROR("Attempt of dynamic memory deallocation to non-pointer lvalue.");
    }
    if (is_array && lvalue->get_expr_type()->u.t_pointer.type->kind != TYPE_arrayI) {
      ERROR("Lvalue of new [] is not an arrayI.");
    }
  }
};

