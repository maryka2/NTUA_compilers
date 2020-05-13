#include "symbol.hpp"
#include "type.hpp"

extern SymbolTable st; // The symbol table

void define_predefined_functions(){
  st.openScope();

  st.insert("writeInteger", type_procedure( vector<Type> {type_integer()} ));
  st.insert("writeBoolean", type_procedure( vector<Type> {type_boolean()} ));
  st.insert("writeChar", type_procedure( vector<Type> {type_char()} ));
  st.insert("writeReal", type_procedure( vector<Type> {type_real()} ));
  st.insert("writeString", type_procedure( vector<Type> {type_arrayII(type_char())} ));

  st.closeScope();
}

