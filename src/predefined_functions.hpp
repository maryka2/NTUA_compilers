//#include "symbol.hpp"
#include "type.hpp"

extern SymbolTable st; // The symbol table

void define_predefined_functions(){
  st.openScope();
  st.insert("writeInteger", type_procedure( std::vector<Type> {type_integer()} ));
  st.insert("writeBoolean", type_procedure( std::vector<Type> {type_boolean()} ));
  st.insert("writeChar", type_procedure( std::vector<Type> {type_char()} ));
  st.insert("writeReal", type_procedure( std::vector<Type> {type_real()} ));
  st.insert("writeString", type_procedure( std::vector<Type> {type_arrayII(type_char())} ));
  st.insert("readInteger", type_function( std::vector<Type> {}, type_integer()));
  st.insert("readBoolean", type_function( std::vector<Type> {}, type_boolean()));
  st.insert("readChar", type_function( std::vector<Type> {}, type_char()));
  st.insert("readReal", type_function( std::vector<Type> {}, type_real()));
  st.insert("readString", type_procedure( std::vector<Type> {type_integer(), type_arrayII(type_char()) } ));
  st.insert("abs", type_function( std::vector<Type> {type_integer()}, type_integer() ));
  st.insert("fabs", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("sqrt", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("cos", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("sin", type_function( std::vector<Type> {type_real()}, type_real() )); 
  st.insert("tan", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("arctan", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("exp", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("ln", type_function( std::vector<Type> {type_real()}, type_real() ));
  st.insert("pi", type_function( std::vector<Type> {}, type_real() ));
  st.insert("trunc", type_function( std::vector<Type> {type_real()}, type_integer() ));
  st.insert("round", type_function( std::vector<Type> {type_real()}, type_integer() ));
  st.insert("ord", type_function( std::vector<Type> {type_char()}, type_integer() ));
  st.insert("chr", type_function( std::vector<Type> {type_integer()}, type_char() ));
}

