#pragma once

#include <cstdlib>
#include <vector>

enum Type { TYPE_integer, TYPE_real, TYPE_boolean, TYPE_char, TYPE_arrayI, TYPE_arrayII, TYPE_pointer };

struct SymbolEntry {
  Type type;
  // int offset;
  SymbolEntry *next;
  SymbolEntry() {}
  //SymbolEntry(Type t, int ofs) : type(t), offset(ofs) {}
  SymbolEntry(Type t, SymbolEntry *n) : type(t), next(n) {}
};

class Scope {
public:
  //Scope() : locals(), offset(-1), size(0) {}
  Scope() : locals() {}
  //Scope(int ofs) : locals(), offset(ofs), size(0) {}
  //int getOffset() const { return offset; }
  //int getSize() const { return size; }
/*  SymbolEntry *lookup(string c) {
     if (locals.find(c) == locals.end()) return nullptr;
      return &(locals[c]);
    }*/ 
/*  void insert(string c, Type t) {
    if (locals.find(c) != locals.end()) {
      std::cerr << "Duplicate variable " << c << std::endl;
      exit(1);
    }
    locals[c] = SymbolEntry(t, offset++);
    ++size;
  }*/
  SymbolEntry *insert(string c, Type t, SymbolEntry *n) {
    if (locals.find(c) != locals.end()) {
      std::cerr << "Duplicate variable " << c << std::endl;
      exit(1);
    }
    locals[c] = SymbolEntry(t, n);
    return &(locals[c]);
  }
private:
  std::unordered_map<string, SymbolEntry> locals;
  //int offset;
  //int size;
};

class SymbolTable {
public:
  void openScope() {
   // int ofs = scopes.empty() ? 0 : scopes.back().getOffset();
   // scopes.push_back(Scope(ofs));
   scopes.push_back(Scope());
  }
  void closeScope() { scopes.pop_back(); };
  SymbolEntry *lookup(string c) {
/*    for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
      SymbolEntry *e = i->lookup(c);
      if (e != nullptr) return e;
    }*/
     if (globals.find(c) == globals.end()) {
      std::cerr << "Unknown variable " << c << std::endl;
      exit(1);
     }
      return globals[c];
  }
  //int getSizeOfCurrentScope() const { return scopes.back().getSize(); }
  void insert(string c, Type t) {
    SymbolEntry *n;
    if (globals.find(c) == globals.end()) n=nullptr;
    else n=globals[c];
    globals[c] = scopes.back().insert(c, t, n); 
  }
private:
  std::vector<Scope> scopes;
  std::unordered_map<string, SymbolEntry*> globals;
};

extern SymbolTable st;
