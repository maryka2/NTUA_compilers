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
          bool is_forward;
          std::vector<bool> is_by_ref;
        } t_function;
        struct {
          std::vector<Type> arg_types;
          bool is_forward;
          std::vector<bool> is_by_ref_arr;
        } t_procedure;
    } u;
};

bool equal_strings(Type t1, Type t2, Value v1, Value v2);
bool is_string(Type t);
bool equal_types(Type t1, Type t2);
void print_type(Type t);
Type type_integer();
Type type_real();
Type type_boolean();
Type type_char();
Type type_arrayI(int dim, Type type);
Type type_arrayII(Type type);
Type type_pointer(Type type);
Type type_label();
Type type_function(Type result_type, bool is_forward);
Type type_procedure(bool is_forward);
