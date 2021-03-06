#include <iostream>
#include <vector>
#include <unordered_map>
//#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>
#include "type.hpp"

using namespace llvm;

// SymbolEntry is a small box in the data stack
struct SymbolEntry {
  Types type;  // Variable's type
  AllocaInst *value;
  SymbolEntry *next;  // A pointer to the next SymbolEntry with the same name, or NULL if such a variable doesn't exist
  SymbolEntry() : value(nullptr), next(nullptr) {}
  SymbolEntry(Types t, SymbolEntry *n) : type(t), next(n) {}  // Initializer
  SymbolEntry(Types t, SymbolEntry *n, AllocaInst *v) : type(t), value(v), next(n) {}  // Initializer
};

extern std::unordered_map<std::string, SymbolEntry*> globals;

// Scope is a big box in the data stack
class Scope {
public:
  std::unordered_map<std::string, SymbolEntry> locals;  // Hash-map matching variable names to SymbolEntries

  Scope() {}  // Initializer: hash map locals is empty
  SymbolEntry *insert(std::string c, Types t, SymbolEntry *n) {
    if (locals.find(c) != locals.end()) {  // Check if there is already a variable with name equal to c in this scope
      // If we are here there is already a variable with the name with that name
      std::cerr << ("Duplicate variable " + c);  // Print error message
      exit(1);
    }
    locals[c] = SymbolEntry(t, n);  // Create new variable
    return &(locals[c]);  // Return pointer to the new variable
  }
  SymbolEntry *insert(std::string c, Types t, SymbolEntry *n, AllocaInst *v) {
    if (locals.find(c) != locals.end()) {  // Check if there is already a variable with name equal to c in this scope
      // If we are here there is already a variable with the name with that name
      std::cerr << ("Duplicate variable " + c);  // Print error message
      exit(1);
    }
    locals[c] = SymbolEntry(t, n, v);  // Create new variable

    return &(locals[c]);  // Return pointer to the new variable
  }
};

class SymbolTable {
public:
  void openScope() {
   scopes.push_back(new Scope());  // Push new scope on the top of data stack
  }
  void closeScope() {  // Removes top scope
    for (std::unordered_map<std::string, SymbolEntry>::iterator it = scopes.back()->locals.begin(); it != scopes.back()->locals.end(); it++) {
      // For every variable (SymbolEntry) of the top scope
      SymbolEntry e = it->second;
      if (e.type->kind == TYPE_label && e.type->u.t_label.is_called && !e.type->u.t_label.is_defined) {
        std::cerr << ("Label '" + it->first + "' used but not defined.");
        exit(1);
      }
      if (e.type->kind == TYPE_procedure && e.type->u.t_procedure.is_forward) {
        std::cerr << ("Called procedure '" + it->first + "' has only been forward declared.");
        exit(1);
      }
      if (e.type->kind == TYPE_function && e.type->u.t_function.is_forward) {
        std::cerr << ("Called function '" + it->first + "' has only been forward declared.");
        exit(1);
      }
      if (it->second.next == nullptr){
        globals.erase(it->first);
      }
      else{
        globals[it->first] = it->second.next;  // Make the global hash table point the next occurance of this variable name
      }
    }
    scopes.pop_back();  // Remove top scope
  };
  SymbolEntry *lookup(std::string c) {
    if (globals.find(c) == globals.end()) {
      return nullptr;
    }
    return globals[c];  // If variable exists return pointer to its SymbolEntry
  }
  void insert(std::string c, Types t) {
    SymbolEntry *n;  // Pointer to next variable with the same name
    if (globals.find(c) == globals.end()) n = nullptr;  // If it doesn't exist point to nullptr
    else n = globals[c];  // else point to it
    globals[c] = scopes.back()->insert(c, t, n); // Insert SymbolEntry to top Scope
  }
  void insert(std::string c, Types t, AllocaInst *v) {
    SymbolEntry *n;  // Pointer to next variable with the same name
    if (globals.find(c) == globals.end()) n = nullptr;  // If it doesn't exist point to nullptr
    else n = globals[c];  // else point to it
    globals[c] = scopes.back()->insert(c, t, n, v); // Insert SymbolEntry to top Scope
  }
  int getSizeOfCurrentScope() {
    if (scopes.empty()) {
      std::cerr << "Scopes empty (symbol.hpp).";
      exit(1);
    }
    return scopes.back()->locals.size();
  }
private:
  std::vector<Scope*> scopes;  // The data stack
};

extern SymbolTable st; // The symbol table
