#include "type.hpp"

void print_type(Type t){
  switch (t->kind) {
    case TYPE_integer:
      out << "integer";
      break;
    case TYPE_real:
      out << "real";
      break;
    case TYPE_boolean:
      out << "boolean";
      break;
    case TYPE_char:
      out << "char";
      break;
    case TYPE_arrayI:
      out << "arrayI with dimension " << t->u.t_arrayI.dim << " and type ";
      print_type(t->u.t_arrayI.type);
      break;
    case TYPE_arrayII:
      out << "arrayII with type ";
      print_type(t->u.t_arrayII.type);
      break;
    case TYPE_pointer:
      out << "pointer with type ";
      print_type(t->u.t_pointer.type);
      break;
    case TYPE_string:
      out << "string"
      break;
    case TYPE_label:
      out << "label"
      break;
    case TYPE_function:
      if (t->u.t_function.is_forward){
        out << "Forward ";
      }
      out << "function";
      if (!t->u.t_function.arg_types.empty()){
        out << "with arguments of type ";
        for (Type t : t->u.t_function.arg_types){
          print_type(t);
        }
      }
      out << " and result type";
      print_type(t->u.t_function.result_type);
      break;
    case TYPE_procedure:
      if (t->u.t_procedure.is_forward){
        out << "Forward ";
      }
      out << "procedure"
      if (!t->u.t_procedure.arg_types.empty()){
        out << "with arguments of type ";
        for (Type t : t->u.t_procedure.arg_types){
          print_type(t);
        }
      }
  }
}

bool equal_types(Type t1, Type t2){
  if (t1->kind != t2->kind){
    return false;
  }
  switch (t1->kind) {
    case TYPE_arrayI:
      return t1->u.t_arrayI.dim == t2->u.t_arrayI.dim && equal_types(t1->u.t_arrayI.type, t2->u.t_arrayI.type);
    case TYPE_arrayII:
      return equal_types(t1->u.t_arrayII.type, t2->u.t_arrayII.type);
    case TYPE_pointer:
      return equal_types(t1->u.t_pointer.type, t2->u.t_pointer.type);
    case TYPE_function:
      if (t1->u.t_function.arg_types.size() !=  t2->u.t_function.arg_types.size()){
        return false;
      }
      for (int type_idx = 0; type_idx<t1->u.t_function.arg_types.size(); type_idx++){
        if (equal_types(t1->u.t_function.arg_types[type_idx], t2->u.t_function.arg_types[type_idx])){
          return false;
        }
      }
      return equal_types(t1->u.t_function.result_type, t2->u.t_function.result_type);
    case TYPE_procedure:
      if (t1->u.t_procedure.arg_types.size() !=  t2->u.t_procedure.arg_types.size()){
        return false;
      }
      for (int type_idx = 0; type_idx<t1->u.t_procedure.arg_types.size(); type_idx++){
        if (equal_types(t1->u.t_procedure.arg_types[type_idx], t2->u.t_procedure.arg_types[type_idx])){
          return false;
        }
      }
      return true;
  }
  return true;
}

Type type_integer()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_integer;
   return result;
}

Type type_real()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_real;
   return result;
}

Type type_boolean()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_boolean;
   return result;
}

Type type_char()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_char;
   return result;
}

Type type_arrayI(int dim, Type type)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_arrayI;
   result->u.t_arrayI.dim = dim;
   result->u.t_arrayI.type = type;
   return result;
}

Type type_arrayII(Type type)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_arrayII;
   result->u.t_arrayII.type = type;
   return result;
}

Type type_pointer(Type type)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_pointer;
   result->u.t_pointer.type = type;
   return result;
}

Type type_string()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_string;
   return result;
}

Type type_label()
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_label;
   return result;
}

Type type_function(Type result_type, bool is_forward)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_function;
   result->result_type = result_type;
   result->is_forward = is_forward;
   return result;
}

Type type_procedure(bool is_forward)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_procedure;
   result->is_forward = is_forward;
   return result;
}
