#include "type.hpp"
#include <stdlib.h>
#include <iostream>

void delete_type(Type t) {
  if ( t->kind == TYPE_arrayI ){
    delete_type(t->u.t_arrayI.type);
  }
  else if ( t->kind == TYPE_arrayII ){
    delete_type(t->u.t_arrayII.type);
  }
  else if ( t->kind == TYPE_pointer ){
    if (!t->u.t_pointer.is_null){
      delete_type(t->u.t_pointer.type);
    }
  }
  else if ( t->kind == TYPE_function ){
    for (Type t_arg: t->u.t_function.arg_types) {
      delete_type(t_arg);
    }
    t->u.t_function.arg_types.clear();
    t->u.t_function.is_by_ref_arr.clear();
    delete_type(t->u.t_function.result_type);
  }
  else if ( t->kind == TYPE_procedure ){
    for (Type t_arg: t->u.t_procedure.arg_types) {
      delete_type(t_arg);
    }
    t->u.t_procedure.arg_types.clear();
    t->u.t_procedure.is_by_ref_arr.clear();
  }
  //free(t);   FIXME
}

bool equal_strings(Type t1, Type t2, Value v1, Value v2){
  if (t1->u.t_arrayI.dim != t1->u.t_arrayI.dim){
    return false;
  }
  for (int index = 0; index < t1->u.t_arrayI.dim; index++){
    if (((char *)v1.arrayI_value)[index] != ((char *)v2.arrayI_value)[index]){
      return false;
    }
  }
  return true;
}

bool is_string(Type t){
  return (t->kind == TYPE_arrayI && t->u.t_arrayI.type->kind == TYPE_char);
}

void print_type(Type t){
  if ( t->kind == TYPE_integer ){
    std::cout << "integer" << std::flush;
  }
  else if ( t->kind == TYPE_real ){
    std::cout << "real" << std::flush;
  }
  else if ( t->kind == TYPE_boolean ){
    std::cout << "boolean" << std::flush;
  }
  else if ( t->kind == TYPE_char ){
    std::cout << "char" << std::flush;
  }
  else if ( t->kind == TYPE_arrayI){
    std::cout << "arrayI with dimension " << t->u.t_arrayI.dim << " and type " << std::flush;
    print_type(t->u.t_arrayI.type);
  }
  else if ( t->kind == TYPE_arrayII ){
    std::cout << "arrayII with type " << std::flush;
    print_type(t->u.t_arrayII.type);
  }
  else if ( t->kind == TYPE_pointer ){
    std::cout << "pointer " << std::flush;
    if (t->u.t_pointer.is_null){
      std::cout << "is null " << std::flush;
    }
    else {
      std::cout << "with type " << std::flush;
      print_type(t->u.t_pointer.type);
    }
  }
  else if ( t->kind == TYPE_label ){
    std::cout << "label" << std::flush;
    if (t->u.t_label.is_defined) {
      std::cout << " used" << std::flush;
    }
    else {
      std::cout << " unused" << std::flush;
    }
    if (t->u.t_label.is_called) {
      std::cout << " and has been called" << std::flush;
    }
    else {
      std::cout << " and has not been called" << std::flush;
    }
  }
  else if ( t->kind == TYPE_function ){
    if (t->u.t_function.is_forward){
      std::cout << "Forward " << std::flush;
    }
    std::cout << "function" << std::flush;
    if (!t->u.t_function.arg_types.empty()){
      std::cout << "with arguments of type " << std::flush;
      for (Type t1 : t->u.t_function.arg_types){
        print_type(t1);
      }
    }
    std::cout << " and result type" << std::flush;
    print_type(t->u.t_function.result_type);
  }
  else if ( t->kind == TYPE_procedure ){
    if (t->u.t_procedure.is_forward){
      std::cout << "Forward " << std::flush;
    }
    std::cout << "procedure" << std::flush;
    if (!t->u.t_procedure.arg_types.empty()){
      std::cout << "with arguments of type " << std::flush;
      for (Type t1 : t->u.t_procedure.arg_types){
        print_type(t1);
      }
    }
  }
}

bool equal_types(Type t1, Type t2){
  if (t1->kind != t2->kind){
    return false;
  }
  if (t1->kind == TYPE_arrayI) {
    return t1->u.t_arrayI.dim == t2->u.t_arrayI.dim && equal_types(t1->u.t_arrayI.type, t2->u.t_arrayI.type);
  }
  if (t1->kind == TYPE_arrayII){
    return equal_types(t1->u.t_arrayII.type, t2->u.t_arrayII.type);
  }
  if (t1->kind == TYPE_pointer){
    if (t1->u.t_pointer.is_null){
      return (t2->u.t_pointer.is_null);
    }
    return ( !t2->u.t_pointer.is_null && equal_types(t1->u.t_pointer.type, t2->u.t_pointer.type));
  }
  if (t1->kind == TYPE_function){
    if (t1->u.t_function.arg_types.size() !=  t2->u.t_function.arg_types.size()){
      return false;
    }
    for (unsigned int type_idx = 0; type_idx<t1->u.t_function.arg_types.size(); type_idx++){
      if (equal_types(t1->u.t_function.arg_types[type_idx], t2->u.t_function.arg_types[type_idx])){
        return false;
      }
    }
    return equal_types(t1->u.t_function.result_type, t2->u.t_function.result_type);
  }
  if (t1->kind == TYPE_procedure){
    if (t1->u.t_procedure.arg_types.size() !=  t2->u.t_procedure.arg_types.size()){
      return false;
    }
    for (unsigned int type_idx = 0; type_idx<t1->u.t_procedure.arg_types.size(); type_idx++){
      if (equal_types(t1->u.t_procedure.arg_types[type_idx], t2->u.t_procedure.arg_types[type_idx])){
        return false;
      }
    }
    return true;
  }
  return true;
}

bool assignable_types (Type t1, Type t2){
  return  (equal_types(t1, t2)
  || (t1->kind == TYPE_integer && t2->kind == TYPE_real)
  || (t1->kind == TYPE_arrayI && t2->kind == TYPE_arrayII && assignable_types(t1->u.t_arrayI.type, t2->u.t_arrayII.type))
  || (t1->kind == TYPE_real && t2->kind == TYPE_integer)
  || (t1->kind == TYPE_arrayII && t2->kind == TYPE_arrayI && assignable_types(t1->u.t_arrayII.type, t2->u.t_arrayI.type))
  || (t1->kind == TYPE_pointer && t2->kind == TYPE_pointer && ( t1->u.t_pointer.is_null && t2->u.t_pointer.is_null || (!t1->u.t_pointer.is_null && !t2->u.t_pointer.is_null && assignable_types(t1->u.t_pointer.type, t2->u.t_pointer.type)))));
}


Type type_integer()
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_integer;
   return result;
}

Type type_real()
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_real;
   return result;
}

Type type_boolean()
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_boolean;
   return result;
}

Type type_char()
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_char;
   return result;
}

Type type_arrayI(int dim, Type type)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_arrayI;
   result->u.t_arrayI.dim = dim;
   result->u.t_arrayI.type = type;
   return result;
}

Type type_arrayII(Type type)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_arrayII;
   result->u.t_arrayII.type = type;
   return result;
}

Type type_pointer(Type type){
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_pointer;
   result->u.t_pointer.type = type;
   result->u.t_pointer.is_null = false;
   return result;
}

Type type_pointer(){
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_pointer;
   result->u.t_pointer.is_null = true;
   return result;
}

Type type_label()
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_label;
   result->u.t_label.is_defined = false;
   result->u.t_label.is_called = false;
   return result;
}

Type type_function(Type result_type, bool is_forward)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_function;
   result->u.t_function.result_type = result_type;
   result->u.t_function.is_forward = is_forward;
   return result;
}

Type type_procedure(bool is_forward)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_procedure;
   result->u.t_procedure.is_forward = is_forward;
   return result;
}

// only for predefined functions
Type type_procedure(std::vector<Type> arg_types)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_procedure;
   result->u.t_procedure.is_forward = false;
   result->u.t_procedure.arg_types = arg_types;
   for (Type t : arg_types){
     result->u.t_procedure.is_by_ref_arr.push_back(false);
   }
   return result;
}

Type type_function(std::vector<Type> arg_types, Type result_type)
{
   Type result = (Type) malloc(sizeof(Type_tag));

   result->kind = TYPE_function;
   result->u.t_function.is_forward = false;
   result->u.t_function.result_type = result_type;
   result->u.t_function.arg_types = arg_types;
   for (Type t : arg_types){
     result->u.t_function.is_by_ref_arr.push_back(false);
   }
   return result;
}
