#include "type.hpp"


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

Type type_function(std::vector<Type> arg_types, Type result_type)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_label;
   result->arg_types = arg_types;
   result->result_type = result_type;
   return result;
}

Type type_procedure(std::vector<Type> arg_types)
{
   Type result = new(sizeof(struct Type_tag));

   result->kind = TYPE_label;
   result->arg_types = arg_types;
   return result;
}
