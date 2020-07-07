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
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

using namespace std;
using namespace llvm;

#define ERROR(msg) { std::cerr << "ERROR: " << msg << "\n"; exit(1); }

/*void ERROR (const string msg)
{
  std::cerr << "ERROR: " << msg << "\n";
  exit(1);
}*/


extern std::unordered_map<string, SymbolEntry*> globals;


inline std::ostream& operator<<(std::ostream &out, Types t) {
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


// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type *VarType) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(VarType, 0, VarName.c_str());
}


class AST {
protected:
  // Global LLVM variables related to the LLVM suite.
  static LLVMContext TheContext;
  static IRBuilder<> Builder;
  static std::unique_ptr<Module> TheModule;
  static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

  // Global LLVM variables related to the generated code.
  static GlobalVariable *TheVars;
  static GlobalVariable *TheRealVars;
  static GlobalVariable *TheNL;

  // Predefined subroutines
  static Function *TheWriteInteger;
  static Function *TheWriteBoolean;
  static Function *TheWriteChar;
  static Function *TheWriteReal;
  static Function *TheWriteString;
  static Function *TheReadInteger;
  static Function *TheReadBoolean;
  static Function *TheReadChar;
  static Function *TheReadReal;
  static Function *TheReadString;
  static Function *TheAbs;
  static Function *TheFabs;
  static Function *TheSqrt;
  static Function *TheSin;
  static Function *TheCos;
  static Function *TheTan;
  static Function *TheArctan;
  static Function *TheExp;
  static Function *TheLn;
  static Function *ThePi;
  static Function *TheTrunc;
  static Function *TheRound;
  static Function *TheOrd;
  static Function *TheChr;

  // Useful LLVM types.
  static Type *i1;
  static Type *i8;
  static Type *i32;
  static Type *i64;
  static Type *DoubleTyID;
  static Type *LabelTyID;

  // Useful LLVM helper functions.
  ConstantInt* c1(int c) const {
    return ConstantInt::get(TheContext, APInt(1, c, true));
  }
  ConstantInt* c8(char c) const {
    return ConstantInt::get(TheContext, APInt(8, c, true));
  }
  ConstantInt* c32(int n) const {
    return ConstantInt::get(TheContext, APInt(32, n, true));
  }
  ConstantInt* c64(int n) const {
    return ConstantInt::get(TheContext, APInt(64, n, true));
  }
  ConstantFP* fp(double n) const {
    return ConstantFP::get(TheContext, APFloat(n));
  }
 // ConstantArray* arr()
/*  ConstantPointerNull null_ptr() {
    return ConstantPointerNull::get();
  }*/
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
  Type* get_llvm_type(Types t){
    if (t->kind == TYPE_integer){
      return IntegerType::get(TheContext, 32);
    }
    if (t->kind == TYPE_real){
      return Type::getDoubleTy(TheContext);
    }
    if (t->kind == TYPE_boolean){
      return IntegerType::get(TheContext, 1);
    }
    if (t->kind == TYPE_char){
      return IntegerType::get(TheContext, 8);
    }
    if (t->kind == TYPE_arrayI){
      return ArrayType::get(get_llvm_type(t->u.t_arrayI.type), t->u.t_arrayI.dim);
    }
    if (t->kind == TYPE_arrayII){
      return PointerType::get(get_llvm_type(t->u.t_arrayII.type), 0);
    }
    if (t->kind == TYPE_pointer){
      if (t->u.t_pointer.is_null){
        return PointerType::get(Type::getVoidTy(TheContext), 0);
      }
      return PointerType::get(get_llvm_type(t->u.t_pointer.type), 0);
    }
  }
  virtual void sem() {}
  virtual Value* compile() = 0;
  void llvm_compile_and_dump() {
    // Initialize the module and the optimization passes.
    TheModule = make_unique<Module>("minibasic program", TheContext);
    TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
    TheFPM->add(createPromoteMemoryToRegisterPass());
    TheFPM->add(createInstructionCombiningPass());
    TheFPM->add(createReassociatePass());
    TheFPM->add(createGVNPass());
    TheFPM->add(createCFGSimplificationPass());
    TheFPM->doInitialization();
    // Define and initialize global symbols.
    // @vars = global [26 x i32] zeroinitializer, align 16
    ArrayType *vars_type = ArrayType::get(i32, 26);
    TheVars = new GlobalVariable(
        *TheModule, vars_type, false, GlobalValue::PrivateLinkage,
        ConstantAggregateZero::get(vars_type), "vars");
    TheVars->setAlignment(16);
    ArrayType *realvars_type = ArrayType::get(DoubleTyID, 26);
   TheRealVars = new GlobalVariable(
       *TheModule, realvars_type, false, GlobalValue::PrivateLinkage,
       ConstantAggregateZero::get(realvars_type), "realvars");
   TheRealVars->setAlignment(32);
    // @nl = private constant [2 x i8] c"\0A\00", align 1
    ArrayType *nl_type = ArrayType::get(i8, 2);
    TheNL = new GlobalVariable(
        *TheModule, nl_type, true, GlobalValue::PrivateLinkage,
        ConstantArray::get(nl_type,
          std::vector<Constant *> { c8('\n'), c8('\0') }
        ), "nl");
    TheNL->setAlignment(1);

    // declare void @writeInteger(i64)
    FunctionType *writeInteger_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *> { i64 }, false);
    TheWriteInteger =
      Function::Create(writeInteger_type, Function::ExternalLinkage,
                       "writeInteger", TheModule.get());
    // declare void @writeBoolean(i1)
    FunctionType *writeBoolean_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *> { i1 }, false);
    TheWriteBoolean =
      Function::Create(writeBoolean_type, Function::ExternalLinkage,
                       "writeBoolean", TheModule.get());

    // declare void @writeChar(i8)
    FunctionType *writeChar_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *> { i8 }, false);
    TheWriteChar =
      Function::Create(writeChar_type, Function::ExternalLinkage,
                       "writeChar", TheModule.get());

    // declare void @readInteger(i64)
    FunctionType *readInteger_type =
      FunctionType::get(i64,
                        std::vector<Type *> { }, false);
    TheReadInteger =
      Function::Create(readInteger_type, Function::ExternalLinkage,
                       "readInteger", TheModule.get());
    
    // declare void @writeString(i8*)
    FunctionType *writeString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *> { PointerType::get(i8, 0) }, false);
    TheWriteString =
      Function::Create(writeString_type, Function::ExternalLinkage,
                       "writeString", TheModule.get());

    // declare void @writeReal(DoubleTyID)
    FunctionType *writeReal_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *> { DoubleTyID }, false);
    TheWriteReal =
      Function::Create(writeReal_type, Function::ExternalLinkage,
                       "writeReal", TheModule.get());

    // declare i1 @readBoolean()
    FunctionType *readBoolean_type =
      FunctionType::get(i1,
                        std::vector<Type *> {  }, false);
    TheReadBoolean =
      Function::Create(readBoolean_type, Function::ExternalLinkage,
                       "readBoolean", TheModule.get());

    // declare i8 @readChar()
    FunctionType *readChar_type =
      FunctionType::get(i8,
                        std::vector<Type *> {  }, false);
    TheReadChar =
      Function::Create(readChar_type, Function::ExternalLinkage,
                       "readChar", TheModule.get());

    // declare DoubleTyID @readReal()
    FunctionType *readReal_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> {  }, false);
    TheReadReal =
      Function::Create(readReal_type, Function::ExternalLinkage,
                       "readReal", TheModule.get());

    // declare PointerType::get(i8, 0) @readString()
    FunctionType *readString_type =
      FunctionType::get(PointerType::get(i8, 0),
                        std::vector<Type *> {  }, false);
    TheReadString =
      Function::Create(readString_type, Function::ExternalLinkage,
                       "readString", TheModule.get());

    // declare i64 @abs(i64)
    FunctionType *abs_type =
      FunctionType::get(i64,
                        std::vector<Type *> { i64 }, false);
    TheAbs =
      Function::Create(abs_type, Function::ExternalLinkage,
                       "abs", TheModule.get());

    // declare DoubleTyID @fabs(DoubleTyID)
    FunctionType *fabs_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheFabs =
      Function::Create(fabs_type, Function::ExternalLinkage,
                       "fabs", TheModule.get());

    // declare DoubleTyID @sqrt(DoubleTyID)
    FunctionType *sqrt_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheSqrt =
      Function::Create(sqrt_type, Function::ExternalLinkage,
                       "sqrt", TheModule.get());

    // declare DoubleTyID @sin(DoubleTyID)
    FunctionType *sin_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheSin =
      Function::Create(sin_type, Function::ExternalLinkage,
                       "sin", TheModule.get());

    // declare DoubleTyID @cos(DoubleTyID)
    FunctionType *cos_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheCos =
      Function::Create(cos_type, Function::ExternalLinkage,
                       "cos", TheModule.get());

    // declare DoubleTyID @tan(DoubleTyID)
    FunctionType *tan_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheTan =
      Function::Create(tan_type, Function::ExternalLinkage,
                       "tan", TheModule.get());

    // declare DoubleTyID @arctan(DoubleTyID)
    FunctionType *arctan_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheArctan =
      Function::Create(arctan_type, Function::ExternalLinkage,
                       "arctan", TheModule.get());

    // declare DoubleTyID @exp(DoubleTyID)
    FunctionType *exp_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheExp =
      Function::Create(exp_type, Function::ExternalLinkage,
                       "exp", TheModule.get());

    // declare DoubleTyID @ln(DoubleTyID)
    FunctionType *ln_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> { DoubleTyID }, false);
    TheLn =
      Function::Create(ln_type, Function::ExternalLinkage,
                       "ln", TheModule.get());

    // declare DoubleTyID @pi()
    FunctionType *pi_type =
      FunctionType::get(DoubleTyID,
                        std::vector<Type *> {  }, false);
    ThePi =
      Function::Create(pi_type, Function::ExternalLinkage,
                       "pi", TheModule.get());

    // declare i64 @trunc(DoubleTyID)
    FunctionType *trunc_type =
      FunctionType::get(i64,
                        std::vector<Type *> { DoubleTyID }, false);
    TheTrunc =
      Function::Create(trunc_type, Function::ExternalLinkage,
                       "trunc", TheModule.get());

    // declare i64 @round(DoubleTyID)
    FunctionType *round_type =
      FunctionType::get(i64,
                        std::vector<Type *> { DoubleTyID }, false);
    TheRound =
      Function::Create(round_type, Function::ExternalLinkage,
                       "round", TheModule.get());

    // declare i8 @ord(i64)
    FunctionType *ord_type =
      FunctionType::get(i8,
                        std::vector<Type *> { i64 }, false);
    TheOrd =
      Function::Create(ord_type, Function::ExternalLinkage,
                       "ord", TheModule.get());

    // declare i64 @chr(i8)
    FunctionType *chr_type =
      FunctionType::get(i64,
                        std::vector<Type *> { i8 }, false);
    TheChr =
      Function::Create(chr_type, Function::ExternalLinkage,
                       "chr", TheModule.get());

    // Define and start the main function.
    Function *main =
      cast<Function>(TheModule->getOrInsertFunction("main", i32));
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", main);
    Builder.SetInsertPoint(BB);
    // Emit the program code.
    compile();
    Builder.CreateRet(c32(0));
    // Verify the IR.
    bool bad = verifyModule(*TheModule, &errs());
    if (bad) {
      std::cerr << "The IR is bad!" << std::endl;
      std::exit(1);
    }
    TheFPM->run(*main);
    // Print out the IR.
    TheModule->print(outs(), nullptr);
  }
};


inline std::ostream& operator<<(std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}


// compile done
class Stmt: public AST {
  //this is intentionally left empty
};


// compile done
class Expr: public AST {
protected:
  Types type;
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
  Types get_expr_type() {
    return type;
  }
};


// compile done
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


// compile done
class Lvalue: public Expr {
public:
  virtual Value* compile_store() = 0;
};


// compile done
class Rvalue: public Expr {
  //this is intentionally left empty
};


// compile done
class Id: public Lvalue {
private:
  string var;
public:
  Id(string v): var(v) {}
  ~Id() {
    //this is intentionally left empty
  }
  void set_type(Types t) {
    type = t;
  }
  void insertIntoCurrentScope() {
    st.insert(var, type);
  }
  string get_name() {
    return var;
  }
  void printOn(std::ostream &out) const override {
    out << "Id(" << var << ")\n";
  }
  void sem() override {
    SymbolEntry *e = st.lookup(var);
    if (e != nullptr) {
      type = e->type;
    }
  }
  Value* compile() override{
    SymbolEntry *e = st.lookup(var);
    if (e != nullptr) {
      type = e->type;
      return Builder.CreateLoad(e->value, var);
    }
    return nullptr;
  }
  Value* compile_store() override{
    SymbolEntry *e = st.lookup(var);
    AllocaInst *Alloca = e->value;
    return Alloca;
  }
};


// compile done
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


// compile done
class Formal: public AST {
private:
  std::vector<Id*> var_name_list;
  Types type;
  bool is_by_ref;
public:
  Formal() {}
  Formal(string var_str, Id_vector *vnv, Types t) : type(t), is_by_ref(true){
    var_name_list = vnv->get_vector();
  }
  Formal(Id_vector *vnv, Types t) : type(t), is_by_ref(false){
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
  Types get_formal_type() {
    return type;
  }
  bool get_is_by_ref() {
    return is_by_ref;
  }
  std::vector<string> get_arg_names() {
    std::vector<string> arg_names;
    for (Id *id: var_name_list) {
      arg_names.push_back(id->get_name());
    }
    return arg_names;
  }
  void sem() override {
    for (Id* id: var_name_list) {
      id->insertIntoCurrentScope();
    }
  }
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
 }
};


// compile done
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


// compile almost done std::vector<bool> is_by_ref_arr;
class Header: public AST {
private:
  Types header_type;
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
  Header(string n, Formal_vector *fv, Types rt) : name(n) {
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
  Header(string n, Types rt) : name(n) {
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
  std::vector<Types> get_arg_types(){
    std::vector<Types> res;
    for (Formal *f: formal_list){
      res.push_back(f->get_formal_type());
    }
    return res;
  }
  void set_forward(){
    if (header_type->kind == TYPE_function){
      header_type->u.t_function.is_forward = true;
    }
    else {
      header_type->u.t_procedure.is_forward = true;
    }
  }
  void sem_outter_scope(bool bool_set_forward=false) {
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
      if (bool_set_forward){
        if (header_type->kind == TYPE_procedure){
          header_type->u.t_procedure.is_forward = false;
        }
        else {
          header_type->u.t_function.is_forward = false;
        }
      }
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
  Function* compile() override{
    Function *F = TheModule->getFunction(name);
    if (F) { // function has already been declared (forward)
      return F;
    }
    Type* res_type;
    std::vector<Type*> arg_types;
    if (header_type->kind == TYPE_procedure){
      res_type = Type::getVoidTy(TheContext);
      for (Types arg: header_type->u.t_procedure.arg_types){
        arg_types.push_back(get_llvm_type(arg));
      }
    }
    else {
      res_type = get_llvm_type(header_type->u.t_function.result_type);
      for (Types arg: header_type->u.t_function.arg_types){
        arg_types.push_back(get_llvm_type(arg));
      }
    }
    FunctionType *FT = FunctionType::get(res_type, arg_types, false);
    F = Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());
    vector<string> arg_names;
    for (Formal *f: formal_list) {
      for (string arg_name: f->get_arg_names()) {
        arg_names.push_back(arg_name);
      }
    }
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
      Arg.setName(arg_names[Idx++]);
    }
    return F;
  }
};


class Block;


// compile done
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
  virtual Value* compile() override{
    sem();
    if (local_type==1){
      return header->compile();
    }
    return nullptr;
 }
};


// compile done
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
    out << "; ";
    first = true;
    for (Stmt *s : stmt_list) {
      if (!first) out << ", ";
      first = false;
      out << *s;
    }
    out << ")";
    out << "\n";
  }
  void block_is_program(){
    is_program = true;
  }
  virtual void sem() override {
    st.openScope();
    if (is_program){
      Id *id = new Id("n");
      id->set_type(type_integer());
      (new Header("writeInteger",  new Formal_vector(new Formal(new Id_vector(id), type_integer())) ))->sem_outter_scope(true);
      id = new Id("b");
      id->set_type(type_boolean());
      (new Header("writeBoolean",  new Formal_vector(new Formal(new Id_vector(id), type_boolean())) ))->sem_outter_scope(true);
      id = new Id("c");
      id->set_type(type_char());
      (new Header("writeChar",  new Formal_vector(new Formal(new Id_vector(id), type_char())) ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("writeReal",  new Formal_vector(new Formal(new Id_vector(id), type_real())) ))->sem_outter_scope(true);
      id = new Id("s");
      id->set_type(type_arrayII(type_char()));
      (new Header("writeString",  new Formal_vector(new Formal("var", new Id_vector(id), type_arrayII(type_char()))) ))->sem_outter_scope(true);
      (new Header("readInteger",  type_integer() ))->sem_outter_scope(true);
      (new Header("readBoolean",  type_boolean() ))->sem_outter_scope(true);
      (new Header("readChar",  type_char() ))->sem_outter_scope(true);
      (new Header("readReal",  type_real() ))->sem_outter_scope(true);
      Id *id2 = new Id("s");
      id2->set_type(type_arrayII(type_char()));
      Formal_vector *fv = new Formal_vector(new Formal("var", new Id_vector(id2), type_arrayII(type_char())));
      Id *id1 = new Id("size");
      id1->set_type(type_integer());
      fv->append_formal(new Formal( new Id_vector(id1), type_integer()));
      (new Header("readString", fv))->sem_outter_scope(true);
      id = new Id("n");
      id->set_type(type_integer());
      (new Header("abs",  new Formal_vector(new Formal(new Id_vector(id), type_integer())), type_integer() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("fabs",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("sqrt",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("sin",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("cos",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("tan",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("arctan",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("exp",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      id = new Id("r");
      id->set_type(type_real());
      (new Header("ln",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_real() ))->sem_outter_scope(true);
      (new Header("pi",  type_real() ))->sem_outter_scope(true);
      (new Header("trunc",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_integer() ))->sem_outter_scope(true);
      (new Header("round",  new Formal_vector(new Formal(new Id_vector(id), type_real())), type_integer() ))->sem_outter_scope(true);
    }
    for (Local *l : local_list) l->sem();
    for (Stmt *s : stmt_list) s->sem();
    size = st.getSizeOfCurrentScope();
    st.closeScope();
  }
  Value* compile() override{
    for (Local *l: local_list) {
      l->compile();
    }
    for (Stmt *s: stmt_list) {
      s->compile();
    }
    return nullptr;
 }
  };


// compile done
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
  Value* compile() override{
    st.openScope();
    Function *TheFunction = header->compile();
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);

    std::vector<Types> arg_types = header->get_arg_types();

    unsigned int idx = 0;
    for (auto &Arg : TheFunction->args()){
      AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), get_llvm_type(arg_types[idx++]));
      Builder.CreateStore(&Arg, Alloca);
      st.insert(Arg.getName(), arg_types[idx++], Alloca);
    }

    if (Value *RetVal = body->compile()) {
      Builder.CreateRet(RetVal);
      verifyFunction(*TheFunction);
      st.closeScope();
      return TheFunction;
    }
    TheFunction->eraseFromParent();
    st.closeScope();
    return nullptr;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
  }
  Value* compile_store() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
  }
  Value* compile_store() override{
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
  }
  Value* compile_store() override{ 
  }
};


// compile almost done (string)
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
      ERROR("BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_pointer) !=  right->type_check(TYPE_pointer) ) {
      ERROR("BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_char) != right->type_check(TYPE_char) ) {
      ERROR("BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( left->type_check(TYPE_boolean) != right->type_check(TYPE_boolean) ) {
      ERROR("BinOp operands have incompatible type for operation '" + op + "'");
    }
    if ( (left->type_check(TYPE_integer) || left->type_check(TYPE_real)) != (right->type_check(TYPE_integer) || right->type_check(TYPE_real) )){
      ERROR("BinOp operands have incompatible type for operation '" + op + "'");
    }
    if (op == "+" || op == "-" || op == "*") {
      if (( left->type_check(TYPE_integer) ) && ( right->type_check(TYPE_integer) )) {
        type = type_integer();
      }
      else if (( left->type_check(TYPE_real) ) || ( right->type_check(TYPE_real) )) {
        type = type_real();
      }
      else {
        ERROR("BinOp operands have incompatible type for operation '" + op + "'");
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
  Value* compile() override{
    Value *lv = left->compile();
    Value *rv = right->compile();
    if (!lv || !rv) {
      return nullptr;
    }
    sem();  // NOTE: We can use sem() for type checks because it does not call other sems.
    if (left->type_check(TYPE_integer) && right->type_check(TYPE_real)) {
      lv = Builder.CreateSIToFP(lv, Type::getDoubleTy(TheContext), "int2floattmp");
    }
    else if (left->type_check(TYPE_real) && right->type_check(TYPE_integer)) {
      rv = Builder.CreateSIToFP(rv, Type::getDoubleTy(TheContext), "int2floattmp");
    }
    // pif fix real - int
    if ( op == "+") {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateAdd(lv, rv);
      }
      return Builder.CreateFAdd(lv, rv);
    }
    if ( op == "-") {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateSub(lv, rv);
      }
      return Builder.CreateFSub(lv, rv);
    }
    if ( op == "*" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateMul(lv, rv);
      }
      return Builder.CreateFMul(lv, rv);
    }
    // pif int to reals always
    if ( op == "/" ) {
      return Builder.CreateFDiv(lv, rv);
    }
    if ( op == "div") {
      return Builder.CreateSDiv(lv, rv);
    }
    if ( op == "mod") {
      return Builder.CreateSRem(lv, rv);
    }
    if ( op == "=") {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_EQ, lv, rv);
      }
      if ( left->type_check(TYPE_real) || right->type_check(TYPE_real) ) {
        return Builder.CreateFCmp(CmpInst::FCMP_OEQ, lv, rv);
      }
      if ( left->type_check(TYPE_boolean) ) {
        return Builder.CreateICmp(CmpInst::ICMP_EQ, lv, rv);
      }
      if ( left->type_check(TYPE_char) ) {
        return Builder.CreateICmp(CmpInst::ICMP_EQ, lv, rv);
      }
      if ( left->type_check(TYPE_pointer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_EQ, lv, rv);
      }
    //   // pif
    //   if (is_string(left->get_expr_type())) {}
    }
    if ( op == "<>" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer)) {
        return Builder.CreateICmp(CmpInst::ICMP_NE, lv, rv);
      }
      if ( left->type_check(TYPE_real) || right->type_check(TYPE_real) ) {
        return Builder.CreateFCmp(CmpInst::FCMP_ONE, lv, rv);
      }
      if ( left->type_check(TYPE_boolean) ) {
        return Builder.CreateICmp(CmpInst::ICMP_NE, lv, rv);
      }
      if ( left->type_check(TYPE_char) ) {
        return Builder.CreateICmp(CmpInst::ICMP_NE, lv, rv);
      }
      if ( left->type_check(TYPE_pointer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_NE, lv, rv);
      }
      // pif
      //if ( is_string(left->get_expr_type() ) ) {}
    }
    if ( op == "or" ) {
      return Builder.CreateOr(lv, rv);
    }
    if ( op == "and" ) {
      return Builder.CreateAnd(lv, rv);
    }
    if ( op == ">" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_SGT, lv, rv);
      }
      return Builder.CreateFCmp(CmpInst::FCMP_OGT, lv, rv);
    }
    if ( op == "<" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_SLT, lv, rv);
      }
      return Builder.CreateFCmp(CmpInst::FCMP_OLT, lv, rv);
    }
    if ( op == ">=" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_SGE, lv, rv);
      }
      return Builder.CreateFCmp(CmpInst::FCMP_OGE, lv, rv);
    }
    if ( op == "<=" ) {
      if ( left->type_check(TYPE_integer) && right->type_check(TYPE_integer) ) {
        return Builder.CreateICmp(CmpInst::ICMP_SLE, lv, rv);
      }
      return Builder.CreateFCmp(CmpInst::FCMP_OLE, lv, rv);
    }
  }
};


// compile almost done (pointer @)
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
  Value* compile() override{
    Value *rv = right->compile();
    if ( op == "+" ) {
      return rv;
    }
    if ( op == "-" ) {
      if (right->type_check(TYPE_integer)) {
        return Builder.CreateNeg(rv);
      }
      return Builder.CreateFNeg(rv);
    }
    if ( op == "not" ) {
      return Builder.CreateNot(rv);
    }
    //pif
    // if ( op == "@" ) {
    //   return Builder.CreateNeg(rv);
    // }
  }
};


class Nil: public Rvalue {
public:
  Nil() {}
  void printOn(std::ostream &out) const override {
    out << "Nil\n";
  }
  void sem() override {
    type = type_pointer();
  }
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
 }
};


// compile done
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
  void sem() override {
    type = type_char();
  }
  virtual Value* compile() override {
    return c8(char_const);
  }
};


// compile done
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
  void sem() override {
    type = type_real();
  }
  virtual Value* compile() override {
    return fp(num);
  }
};


// compile done
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
  void sem() override {
    type = type_boolean();
  }
  virtual Value* compile() override {
    return c1(boolean == "true");
  }
};


// compile done
class Intconst: public Rvalue {
private:
  int num;
public:
  Intconst(int n): num(n) {}
  ~Intconst(){
    // This is intentionally left empty
  }
  void printOn(std::ostream &out) const override {
    out << "Intconst(" << num << ")";
  }
  void sem() override {
    type = type_integer();
  }
  virtual Value* compile() override {
    return c64(num);
  }
};


// compile done
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
  Value* compile() override{
    return nullptr;
 }
};


// compile done
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
  Value* compile() override{
    Value *n = expr->compile();
    BasicBlock *PrevBB = Builder.GetInsertBlock();
    Function *TheFunction = PrevBB->getParent();
    BasicBlock *LoopBB =
      BasicBlock::Create(TheContext, "loop", TheFunction);
    BasicBlock *BodyBB =
      BasicBlock::Create(TheContext, "body", TheFunction);
    BasicBlock *AfterBB =
      BasicBlock::Create(TheContext, "endwhile", TheFunction);
    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(LoopBB);
    PHINode *phi_iter = Builder.CreatePHI(i1, 2, "iter");
    phi_iter->addIncoming(n, PrevBB);
    Value *loop_cond = Builder.CreateICmpNE(phi_iter, c1(0), "loop_cond");
    Builder.CreateCondBr(loop_cond, BodyBB, AfterBB);
    Builder.SetInsertPoint(BodyBB);
    stmt->compile();
    n = expr->compile();
    phi_iter->addIncoming(n, Builder.GetInsertBlock());
    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(AfterBB);
    return nullptr;
 }
};


// compile done
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
  Value* compile() override{
    Value *v = cond->compile();
    Value *cond = Builder.CreateICmpNE(v, c32(0), "if_cond");
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *ThenBB =
      BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB =
      BasicBlock::Create(TheContext, "else", TheFunction);
    BasicBlock *AfterBB =
      BasicBlock::Create(TheContext, "endif", TheFunction);
    Builder.CreateCondBr(cond, ThenBB, ElseBB);
    Builder.SetInsertPoint(ThenBB);
    stmt1->compile();
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(ElseBB);
    if (stmt2 != nullptr) {
      stmt2->compile();
    }
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(AfterBB);
    return nullptr;
  }
};


// compile done (ArgsV.back() nullptr, by ref)
class Call: public Stmt, public Rvalue {
private:
  string name;
  std::vector<Expr*> expr_list;
  bool is_stmt = false;
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
      out << *expr << ": ";
    }
    //out << "\n";
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
  Value* compile() override{
    // Look up the name in the global module table.
    Function *CalleeF = TheModule->getFunction(name);
    if (!CalleeF) {
      ERROR("Unknown function/procedure referenced.");
    }
    // If argument mismatch error.
    if (CalleeF->arg_size() != expr_list.size()) {
      ERROR("Incorrect number of arguments passed."+name+ std::to_string(CalleeF->arg_size())+' ' +std::to_string(expr_list.size()));
    }
    std::vector<Value *> ArgsV;
    for (unsigned i = 0; i < expr_list.size(); ++i) {
      ArgsV.push_back(expr_list[i]->compile());
      if (!ArgsV.back()) {
        return nullptr;
      }
    }
    //return Builder.CreateCall(CalleeF, ArgsV, "call" + name + "tmp");
    return Builder.CreateCall(CalleeF, ArgsV);
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
    Types lvalue_type = lvalue->get_expr_type();
    Types expr_type = expr->get_expr_type();
    if (!assignable_types(lvalue_type, expr_type)){
      ERROR("Assignement of wrong type.");
    }
  }
  Value* compile() override{
    Value *Val = expr->compile();
    if (!Val) {
      return nullptr;
    }
    Value *lv = lvalue->compile_store();
    if (!lv) {
      ERROR("Unknown variable name");
    }
    Builder.CreateStore(Val, lv);
    return Val;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
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
  Value* compile() override{
    Value *r;
    ERROR("Non implemented");
    exit(1);
    return r;
 }
};
