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

typedef struct Types_tag * Types;


struct Types_tag {
    enum myType kind;
    union {
        struct {
          int dim;
          Types type;
        } t_arrayI;
        struct {
          Types type;
        } t_arrayII;
        struct {
          Types type;
          bool is_null;
        } t_pointer;
        struct {
          std::vector<Types> arg_types;
          Types result_type;
          bool is_forward;
          std::vector<bool> is_by_ref_arr;
        } t_function;
        struct {
          std::vector<Types> arg_types;
          bool is_forward;
          std::vector<bool> is_by_ref_arr;
        } t_procedure;
        struct {
          bool is_defined;  // Used next to statement
          bool is_called;  // Has been called by goto
        } t_label;
    } u;
};

union Value {
  int integer_value;
  double real_value;
  bool boolean_value;
  char char_value;
  void *arrayI_value;
  void *arrayII_value;
  void *pointer_value;
};

void delete_type(Types t);
bool equal_strings(Types t1, Types t2, Value v1, Value v2);
bool is_string(Types t);
bool equal_types(Types t1, Types t2);
bool assignable_types(Types t1, Types t2);
void print_type(Types t);
Types type_integer();
Types type_real();
Types type_boolean();
Types type_char();
Types type_arrayI(int dim, Types type);
Types type_arrayII(Types type);
Types type_pointer(Types type);
Types type_pointer();
Types type_label();
Types type_function(Types result_type, bool is_forward);
Types type_procedure(bool is_forward);
Types type_procedure(std::vector<Types> arg_types);
Types type_function(std::vector<Types> arg_types, Types result_type);
