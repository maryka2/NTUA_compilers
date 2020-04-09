#pragma once

typedef struct Type_tag * Type;


struct Type_tag {
    enum {
      TYPE_integer,
      TYPE_real,
      TYPE_boolean,
      TYPE_char,
      TYPE_arrayI,
      TYPE_arrayII,
      TYPE_pointer,
      TYPE_string
    } kind;
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