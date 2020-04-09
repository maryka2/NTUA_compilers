#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

enum Type { TYPE_integer, TYPE_real, TYPE_boolean, TYPE_char, TYPE_arrayI, TYPE_arrayII, TYPE_pointer, TYPE_string };

// SymbolEntry is a small box in the data stack
struct SymbolEntry {
  Type type;  // Variable's type
  SymbolEntry *next;  // A pointer to the next SymbolEntry with the same name, or NULL if such a variable doesn't exist
  SymbolEntry() {}
  SymbolEntry(Type t, SymbolEntry *n) : type(t), next(n) {}  // Initializer
};

// Scope is a big box in the data stack
class Scope {
public:
  unordered_map<string, SymbolEntry> locals;  // Hash-map matching variable names to SymbolEntries

  Scope() : locals() {}  // Initializer: hash map locals is empty
  SymbolEntry *insert(string c, Type t, SymbolEntry *n) {
    if (locals.find(c) != locals.end()) {  // Check if there is already a variable with name equal to c in this scope
      // If we are here there is already a variable with the name with that name
      cerr << "Duplicate variable " << c << endl;  // Print error message
      exit(1);  // Exit compiler
    }
    locals[c] = SymbolEntry(t, n);  // Create new variable
    return &(locals[c]);  // Return pointer to the new variable
  }
};

class SymbolTable {
public:
  void openScope() {
   scopes.push_back(Scope());  // Push new scope on the top of data stack
  }
  void closeScope() {  // Removes top scope
    for (unordered_map<string, SymbolEntry>::iterator it = scopes.back().locals.begin(); it != scopes.back().locals.end(); it++) {
      // For every variable (SymbolEntry) of the top scope
      globals[it->first] = (it->second).next;  // Make the global hash table point the next occurance of this variable name
    }
    scopes.pop_back();  // Remove top scope
  };
  SymbolEntry *lookup(string c) {
     if (globals.find(c) == globals.end()) {
      std::cerr << "Unknown variable " << c << std::endl;
      exit(1);  // If variable name isn't used exit compiler
     }
      return globals[c];  // If variable exists return pointer to its SymbolEntry
  }
  void insert(string c, Type t) {
    SymbolEntry *n;  // Pointer to next variable with the same name
    if (globals.find(c) == globals.end()) n = nullptr;  // If it doesn't exist point to nullptr
    else n = globals[c];  // else point to it
    globals[c] = scopes.back().insert(c, t, n); // Insert SymbolEntry to top Scope
  }
private:
  std::vector<Scope> scopes;  // The data stack
  std::unordered_map<string, SymbolEntry*> globals;  // Global hash table
};

extern SymbolTable st; // The symbol table