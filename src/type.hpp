#pragma once

#include <vector>

enum myType{
      TYPE_integer,
      TYPE_real,
      TYPE_boolean,
      TYPE_char,
      TYPE_arrayI,
      TYPE_arrayII,
      TYPE_pointer,
      TYPE_string,
      TYPE_label,
      TYPE_function,
      TYPE_procedure
    } ;

typedef struct Type_tag * Type;


struct Type_tag {
    enum myType kind;
    union {
        struct {
          int dim;
          Type type;
        } t_arrayI;
        struct {
          Type type;
        } t_arrayII;
        struct {
          Type type;
        } t_pointer;
        struct {
          std::vector<Type> arg_types;
          Type result_type;
        } t_function;
        struct {
          std::vector<Type> arg_types;
        } t_procedure;
    } u;
};


Type type_integer ();
Type type_real ();
Type type_boolean ();
Type type_char ();
Type type_arrayI (int dim, Type type);
Type type_arrayII (Type type);
Type type_pointer (Type type);
Type type_string ();
Type type_label ();
Type type_function (std::vector<Type> arg_types, Type result_type);
Type type_procedure (std::vector<Type> arg_types);
