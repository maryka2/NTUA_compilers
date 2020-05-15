/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.ypp" /* yacc.c:339  */

#include <cstdio>
#include <stdlib.h>
#include "ast.hpp"
#include "lexer.hpp"
#include "predefined_functions.hpp"

SymbolTable st;
std::unordered_map<string, SymbolEntry*> globals;  // Global hash table

#line 77 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_and = 258,
    T_array = 259,
    T_begin = 260,
    T_boolean = 261,
    T_char = 262,
    T_dispose = 263,
    T_div = 264,
    T_do = 265,
    T_else = 266,
    T_end = 267,
    T_false = 268,
    T_forward = 269,
    T_function = 270,
    T_goto = 271,
    T_if = 272,
    T_integer = 273,
    T_label = 274,
    T_mod = 275,
    T_new = 276,
    T_nil = 277,
    T_not = 278,
    T_of = 279,
    T_or = 280,
    T_procedure = 281,
    T_program = 282,
    T_real = 283,
    T_result = 284,
    T_return = 285,
    T_then = 286,
    T_true = 287,
    T_var = 288,
    T_while = 289,
    T_eq = 290,
    T_gt = 291,
    T_lt = 292,
    T_ne = 293,
    T_ge = 294,
    T_le = 295,
    T_plus = 296,
    T_minus = 297,
    T_times = 298,
    T_rdiv = 299,
    T_dref = 300,
    T_ref = 301,
    T_assign = 302,
    T_semicolon = 303,
    T_dot = 304,
    T_lparen = 305,
    T_rparen = 306,
    T_colon = 307,
    T_comma = 308,
    T_lbracket = 309,
    T_rbracket = 310,
    T_intconst = 311,
    T_realconst = 312,
    T_charconst = 313,
    T_stringconst = 314,
    T_id = 315
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 76 "parser.ypp" /* yacc.c:355  */

	int i;
	double f;
	char c;
	char *str;
	bool b;
	Type type;
	Expr *expr;
	Lvalue *lvalue;
	Rvalue *rvalue;
	Call *call;
	Header *header;
	Formal *formal;
	Block *block;
	Stmt *stmt;
	Id_vector *var_name_type_list;
	Formal_vector *formal_list;
	Expr_vector *expr_list;
	UnOp *unop;
	BinOp *binop;
	char op[3];
	Id *id;
	Local *local;

#line 203 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 220 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   580

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  178

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   127,   130,   131,   134,   135,   136,   137,
     142,   148,   157,   158,   161,   162,   163,   164,   167,   168,
     171,   177,   185,   186,   187,   188,   189,   190,   191,   194,
     195,   199,   200,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   222,   223,   224,
     225,   226,   227,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   261,
     262,   266,   267,   270,   271
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"and\"", "\"array\"", "\"begin\"",
  "\"boolean\"", "\"char\"", "\"dispose\"", "\"div\"", "\"do\"",
  "\"else\"", "\"end\"", "\"false\"", "\"forward\"", "\"function\"",
  "\"goto\"", "\"if\"", "\"integer\"", "\"label\"", "\"mod\"", "\"new\"",
  "\"nil\"", "\"not\"", "\"of\"", "\"or\"", "\"procedure\"", "\"program\"",
  "\"real\"", "\"result\"", "\"return\"", "\"then\"", "\"true\"",
  "\"var\"", "\"while\"", "\"=\"", "\">\"", "\"<\"", "\"<>\"", "\">=\"",
  "\"<=\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"^\"", "\"@\"", "\":=\"",
  "\";\"", "\".\"", "\"(\"", "\")\"", "\":\"", "\",\"", "\"[\"", "\"]\"",
  "T_intconst", "T_realconst", "T_charconst", "T_stringconst", "T_id",
  "$accept", "program", "body", "local_list", "local",
  "var_name_type_list", "var_name_list", "header", "formal_list", "formal",
  "type", "block", "stmt_list", "stmt", "lvalue", "rvalue", "expr", "call",
  "expr_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315
};
# endif

#define YYPACT_NINF -67

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-67)))

#define YYTABLE_NINF -47

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-47)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -19,   -37,    38,    11,   -67,   -67,    12,   170,   -67,    91,
      17,    27,    42,    55,    42,   -67,    69,   -67,   254,   -67,
     -67,    62,   364,   293,   -67,   364,   -67,   -67,   -67,   364,
     364,   364,   364,   364,   -67,   -67,   -67,   -67,   -13,   -67,
      -6,   -67,   -25,   -67,   492,     8,    83,    10,   -67,    -3,
      80,    42,   -22,   -67,    79,    95,    -1,   -67,   -67,    92,
     406,   364,     1,   110,   449,   115,   115,   -67,     0,    89,
     332,   149,   -67,   149,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     -67,   -67,   -16,   -67,    96,    68,    45,    82,   113,   364,
     149,   177,   149,   -67,   -67,   -67,   492,    -5,   -67,   -67,
     492,   220,   110,   110,   110,   115,   535,   535,   535,   535,
     535,   535,   115,   115,   110,   110,    42,   111,    86,    58,
     -67,   -67,   -67,    78,    82,   -21,   -67,   -67,   -67,   -67,
      82,   116,   -67,     4,   156,   364,   -67,   -67,   364,   -67,
      90,    82,    82,     7,   117,   -67,   120,    82,   118,   -67,
     -67,   149,    57,   492,    82,   -67,   -67,   -67,    82,   -67,
     -67,   121,   -67,   -67,   -67,   153,    82,   -67
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     4,     0,     0,     2,    33,
       0,     0,     0,     0,     0,     5,     0,     3,     0,    30,
      55,     0,     0,     0,    59,     0,    48,    42,    54,     0,
       0,     0,     0,     0,    53,    56,    57,    49,    47,    35,
       0,    31,    79,    80,     0,    60,     0,     0,    13,     0,
       0,     6,     0,     4,     0,    47,    79,    60,    41,    79,
       0,     0,    79,    62,     0,    63,    64,    61,    79,    80,
       0,    33,    29,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     9,     0,     7,     0,     0,     0,     0,     0,     0,
      33,     0,    33,    52,    58,    81,    83,     0,    40,    32,
      34,     0,    72,    69,    70,    71,    73,    77,    75,    74,
      78,    76,    65,    66,    67,    68,     0,     0,     0,     0,
      18,    12,    16,     0,     0,     0,    24,    25,    22,    23,
       0,     0,     8,    79,    38,     0,    39,    82,     0,    50,
       0,     0,     0,     0,     0,    14,     0,     0,     0,    28,
      10,    33,    79,    84,     0,    17,    21,    19,     0,    11,
      27,     0,    37,    20,    15,     0,     0,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   129,   -67,   -67,   -67,   -10,   163,    93,    34,
     208,   185,   -67,   -66,    -9,   160,    -4,    -8,   -67
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     6,     7,    15,    51,   128,    16,   129,   130,
     141,    39,    40,    41,    59,    43,    44,    57,   107
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,    45,    49,   157,    52,   108,    72,   109,     1,    56,
     -45,   -45,   -43,   -43,    62,   -46,   -46,   126,    60,   -36,
     -36,    63,    74,     3,    68,    64,    65,    66,    67,    75,
      97,    94,    11,   158,   144,   127,   146,    70,     4,    71,
     126,    96,    73,    13,    48,    93,   147,   -45,   148,   -43,
      94,   103,   -46,    75,    75,    75,   -36,   101,    75,     5,
      92,     8,    42,    45,    42,    45,   106,    48,   -44,   -44,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   135,    47,   136,   137,
     143,    42,    45,    42,    45,   172,     9,   134,    94,    18,
     138,   126,    48,    19,    20,   -44,   153,    21,    22,   154,
     139,    75,    23,    24,    25,    50,   150,    53,    76,   132,
      26,    27,    58,    28,    77,    29,   153,   140,    48,   155,
      95,    91,    30,    31,    99,    78,   162,    32,   152,    94,
     104,    33,   164,    94,   163,    70,    75,    34,    35,    36,
      37,    38,    42,    45,     9,    90,   131,    18,    88,    89,
      90,   142,    20,   151,   160,    21,    22,   161,   169,   168,
      23,    24,    25,    46,   171,     9,   175,   176,    26,    27,
      76,    28,    98,    29,    10,    11,    77,   167,   133,    12,
      30,    31,    17,    69,     0,    32,    13,    78,     0,    33,
       0,     0,    79,    14,     0,    34,    35,    36,    37,    38,
       0,     0,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    76,     0,     0,     0,     0,     0,    77,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,     0,    20,     0,     0,
       0,     0,     0,     0,     0,   149,    24,    25,     0,     0,
       0,     0,     0,    26,     0,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,     0,     0,     0,
      32,     0,     0,     0,    33,     0,    20,     0,    54,     0,
      34,    35,    36,    37,    55,    24,    25,     0,     0,     0,
       0,     0,    26,     0,     0,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    30,    31,     0,     0,     0,    32,
       0,     0,   156,    33,     0,    20,     0,    61,   159,    34,
      35,    36,    37,    55,    24,    25,     0,     0,     0,   165,
     166,    26,     0,     0,    28,   170,     0,     0,     0,     0,
       0,     0,   173,    30,    31,     0,   174,    20,    32,     0,
       0,     0,    33,   105,   177,     0,    24,    25,    34,    35,
      36,    37,    55,    26,     0,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,     0,     0,    76,
      32,     0,     0,     0,    33,    77,     0,     0,     0,     0,
      34,    35,    36,    37,    55,     0,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,   100,     0,     0,
       0,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    76,     0,     0,     0,     0,     0,    77,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    76,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    76,     0,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -47,   -47,   -47,   -47,   -47,   -47,    86,    87,    88,    89,
      90
};

static const yytype_int16 yycheck[] =
{
       9,     9,    12,    24,    14,    71,    12,    73,    27,    18,
      11,    12,    11,    12,    23,    11,    12,    33,    22,    11,
      12,    25,    47,    60,    33,    29,    30,    31,    32,    54,
      52,    53,    15,    54,   100,    51,   102,    50,     0,    52,
      33,    51,    48,    26,    60,    48,    51,    48,    53,    48,
      53,    51,    48,    54,    54,    54,    48,    61,    54,    48,
      50,    49,    71,    71,    73,    73,    70,    60,    11,    12,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,     4,    60,     6,     7,
      99,   100,   100,   102,   102,   161,     5,    52,    53,     8,
      18,    33,    60,    12,    13,    48,    48,    16,    17,    51,
      28,    54,    21,    22,    23,    60,   126,    48,     3,    51,
      29,    30,    60,    32,     9,    34,    48,    45,    60,    51,
      50,    48,    41,    42,    55,    20,   145,    46,    52,    53,
      51,    50,    52,    53,   148,    50,    54,    56,    57,    58,
      59,    60,   161,   161,     5,    45,    60,     8,    43,    44,
      45,    48,    13,    52,    48,    16,    17,    11,    48,    52,
      21,    22,    23,    10,    56,     5,    55,    24,    29,    30,
       3,    32,    53,    34,    14,    15,     9,   153,    95,    19,
      41,    42,     7,    33,    -1,    46,    26,    20,    -1,    50,
      -1,    -1,    25,    33,    -1,    56,    57,    58,    59,    60,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     3,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    22,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    13,    -1,    54,    -1,
      56,    57,    58,    59,    60,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    46,
      -1,    -1,   134,    50,    -1,    13,    -1,    54,   140,    56,
      57,    58,    59,    60,    22,    23,    -1,    -1,    -1,   151,
     152,    29,    -1,    -1,    32,   157,    -1,    -1,    -1,    -1,
      -1,    -1,   164,    41,    42,    -1,   168,    13,    46,    -1,
      -1,    -1,    50,    51,   176,    -1,    22,    23,    56,    57,
      58,    59,    60,    29,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,     3,
      46,    -1,    -1,    -1,    50,     9,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     3,    -1,    -1,    -1,    -1,    -1,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     3,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     3,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    27,    62,    60,     0,    48,    63,    64,    49,     5,
      14,    15,    19,    26,    33,    65,    68,    72,     8,    12,
      13,    16,    17,    21,    22,    23,    29,    30,    32,    34,
      41,    42,    46,    50,    56,    57,    58,    59,    60,    72,
      73,    74,    75,    76,    77,    78,    68,    60,    60,    67,
      60,    66,    67,    48,    54,    60,    75,    78,    60,    75,
      77,    54,    75,    77,    77,    77,    77,    77,    75,    76,
      50,    52,    12,    48,    47,    54,     3,     9,    20,    25,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    48,    50,    48,    53,    50,    67,    52,    63,    55,
      31,    77,    10,    51,    51,    51,    77,    79,    74,    74,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    33,    51,    67,    69,
      70,    60,    51,    69,    52,     4,     6,     7,    18,    28,
      45,    71,    48,    75,    74,    55,    74,    51,    53,    55,
      67,    52,    52,    48,    51,    51,    71,    24,    54,    71,
      48,    11,    75,    77,    52,    71,    71,    70,    52,    48,
      71,    56,    74,    71,    71,    55,    24,    71
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    64,    64,    65,    65,    65,    65,
      66,    66,    67,    67,    68,    68,    68,    68,    69,    69,
      70,    70,    71,    71,    71,    71,    71,    71,    71,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    75,    75,    75,
      75,    75,    75,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    77,
      77,    78,    78,    79,    79
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     2,     0,     2,     2,     3,     4,     3,
       4,     5,     3,     1,     5,     7,     4,     6,     1,     3,
       4,     3,     1,     1,     1,     1,     6,     3,     2,     3,
       2,     1,     3,     0,     3,     1,     1,     6,     4,     4,
       3,     2,     1,     2,     5,     2,     4,     1,     1,     1,
       4,     2,     3,     1,     1,     1,     1,     1,     3,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     4,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 124 "parser.ypp" /* yacc.c:1646  */
    { define_predefined_functions(); std::cout<<"done with predefined\n"; (yyvsp[-1].block)->sem(); (yyvsp[-1].block)->printOn(std::cout); (yyval.block) = (yyvsp[-1].block); std::cout<<"done with semantic.. ready to close scope\n"; st.closeScope(); }
#line 1502 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 127 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].block)->merge((yyvsp[0].block)); (yyval.block) = (yyvsp[-1].block); }
#line 1508 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 130 "parser.ypp" /* yacc.c:1646  */
    { (yyval.block) = new Block(); }
#line 1514 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 131 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].block)->append_local((yyvsp[0].local)); (yyval.block) = (yyvsp[-1].block); }
#line 1520 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 134 "parser.ypp" /* yacc.c:1646  */
    { (yyval.local) = new Local("var", (yyvsp[0].var_name_type_list)); }
#line 1526 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 135 "parser.ypp" /* yacc.c:1646  */
    { (yyval.local) = new Local("label", (yyvsp[-1].var_name_type_list)); }
#line 1532 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 136 "parser.ypp" /* yacc.c:1646  */
    { (yyval.local) = new Local_after_block((yyvsp[-3].header), (yyvsp[-1].block)); }
#line 1538 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 137 "parser.ypp" /* yacc.c:1646  */
    { (yyval.local) = new Local((yyvsp[-1].header)); }
#line 1544 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 142 "parser.ypp" /* yacc.c:1646  */
    {
  for (Id* id_ptr : (yyvsp[-3].var_name_type_list)->get_vector()) {
    id_ptr->set_type((yyvsp[-1].type));
  }
  (yyval.var_name_type_list) = (yyvsp[-3].var_name_type_list);
}
#line 1555 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 148 "parser.ypp" /* yacc.c:1646  */
    {
  for (Id* id_ptr : (yyvsp[-3].var_name_type_list)->get_vector()) {
    id_ptr->set_type((yyvsp[-1].type));
    (yyvsp[-4].var_name_type_list)->append_id(id_ptr);
  }
  (yyval.var_name_type_list) = (yyvsp[-4].var_name_type_list);
}
#line 1567 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 157 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].var_name_type_list)->append_id(new Id(std::string((yyvsp[0].str)))); (yyval.var_name_type_list) = (yyvsp[-2].var_name_type_list); }
#line 1573 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 158 "parser.ypp" /* yacc.c:1646  */
    { (yyval.var_name_type_list) = new Id_vector(new Id(std::string((yyvsp[0].str)))); }
#line 1579 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 161 "parser.ypp" /* yacc.c:1646  */
    { (yyval.header) = new Header(std::string((yyvsp[-3].str)), (yyvsp[-1].formal_list)); }
#line 1585 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 162 "parser.ypp" /* yacc.c:1646  */
    { (yyval.header) = new Header(std::string((yyvsp[-5].str)), (yyvsp[-3].formal_list), (yyvsp[0].type)); }
#line 1591 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 163 "parser.ypp" /* yacc.c:1646  */
    { (yyval.header) = new Header(std::string((yyvsp[-2].str))); }
#line 1597 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 164 "parser.ypp" /* yacc.c:1646  */
    { (yyval.header) = new Header(std::string((yyvsp[-4].str)), (yyvsp[0].type)); }
#line 1603 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 167 "parser.ypp" /* yacc.c:1646  */
    { (yyval.formal_list) = new Formal_vector((yyvsp[0].formal)); }
#line 1609 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 168 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].formal_list)->append_formal((yyvsp[0].formal)); (yyval.formal_list) = (yyvsp[-2].formal_list); }
#line 1615 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 171 "parser.ypp" /* yacc.c:1646  */
    {
  for (Id* id_ptr : (yyvsp[-2].var_name_type_list)->get_vector()) {
    id_ptr->set_type((yyvsp[0].type));
  }
  (yyval.formal) = new Formal("var", (yyvsp[-2].var_name_type_list), (yyvsp[0].type));
}
#line 1626 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 177 "parser.ypp" /* yacc.c:1646  */
    {
  for (Id* id_ptr : (yyvsp[-2].var_name_type_list)->get_vector()) {
    id_ptr->set_type((yyvsp[0].type));
  }
  (yyval.formal) = new Formal((yyvsp[-2].var_name_type_list), (yyvsp[0].type));
}
#line 1637 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 185 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_integer(); }
#line 1643 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 186 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_real(); }
#line 1649 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 187 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_boolean(); }
#line 1655 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 188 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_char(); }
#line 1661 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 189 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_arrayI((yyvsp[-3].i), (yyvsp[0].type)); }
#line 1667 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 190 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_arrayII((yyvsp[0].type)); }
#line 1673 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 191 "parser.ypp" /* yacc.c:1646  */
    { (yyval.type) = type_pointer((yyvsp[0].type)); }
#line 1679 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 194 "parser.ypp" /* yacc.c:1646  */
    { (yyval.block) = (yyvsp[-1].block); }
#line 1685 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 195 "parser.ypp" /* yacc.c:1646  */
    { (yyval.block) = new Block(); }
#line 1691 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 199 "parser.ypp" /* yacc.c:1646  */
    { Block *block = new Block(); block->append_stmt((yyvsp[0].stmt)); (yyval.block) = block; }
#line 1697 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 200 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].block)->append_stmt((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-2].block); }
#line 1703 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 204 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new EmptyStmt(); }
#line 1709 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 205 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Assignment((yyvsp[-2].lvalue), (yyvsp[0].expr)); }
#line 1715 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 206 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = (yyvsp[0].block); }
#line 1721 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 207 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[0].call)->is_statement(); (yyval.stmt) = (yyvsp[0].call); }
#line 1727 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 208 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new If((yyvsp[-4].expr), (yyvsp[-2].stmt), (yyvsp[0].stmt)); }
#line 1733 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 209 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new If((yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 1739 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 210 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new While((yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 1745 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 211 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Label(std::string((yyvsp[-2].str)), (yyvsp[0].stmt)); }
#line 1751 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 212 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Goto(std::string((yyvsp[0].str))); }
#line 1757 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 213 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Return(); }
#line 1763 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 214 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new New((yyvsp[0].lvalue)); }
#line 1769 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 215 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new New((yyvsp[-2].expr), (yyvsp[0].lvalue)); }
#line 1775 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 216 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Dispose("pointer", (yyvsp[0].lvalue)); }
#line 1781 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 217 "parser.ypp" /* yacc.c:1646  */
    { (yyval.stmt) = new Dispose("array_pointer", (yyvsp[0].lvalue)); }
#line 1787 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 222 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = new Id(std::string((yyvsp[0].str))); }
#line 1793 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 223 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = new Id("result"); }
#line 1799 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 224 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = new Stringconst(std::string((yyvsp[0].str))); }
#line 1805 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 225 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = new Array((yyvsp[-3].lvalue), (yyvsp[-1].expr)); }
#line 1811 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 226 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = new Dereference((yyvsp[-1].expr)); }
#line 1817 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 227 "parser.ypp" /* yacc.c:1646  */
    { (yyval.lvalue) = (yyvsp[-1].lvalue); }
#line 1823 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 231 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Intconst((yyvsp[0].i)); }
#line 1829 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 232 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Bool("true"); }
#line 1835 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 233 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Bool("false"); }
#line 1841 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 234 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Realconst((yyvsp[0].f)); }
#line 1847 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 235 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Charconst((yyvsp[0].c)); }
#line 1853 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 236 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = (yyvsp[-1].rvalue); }
#line 1859 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 237 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new Nil(); }
#line 1865 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 238 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = (yyvsp[0].call); }
#line 1871 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 239 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new UnOp(std::string("@"), (yyvsp[0].expr)); }
#line 1877 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 240 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new UnOp(std::string("not"), (yyvsp[0].expr)); }
#line 1883 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 241 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new UnOp(std::string("+"), (yyvsp[0].expr)); }
#line 1889 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 242 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new UnOp(std::string("-"), (yyvsp[0].expr)); }
#line 1895 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 243 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("+"), (yyvsp[0].expr)); }
#line 1901 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 244 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("-"), (yyvsp[0].expr)); }
#line 1907 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 245 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("*"), (yyvsp[0].expr)); }
#line 1913 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 246 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("/"), (yyvsp[0].expr)); }
#line 1919 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 247 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("div"), (yyvsp[0].expr)); }
#line 1925 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 248 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("mod"), (yyvsp[0].expr)); }
#line 1931 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 249 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("or"), (yyvsp[0].expr)); }
#line 1937 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 250 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("and"), (yyvsp[0].expr)); }
#line 1943 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 251 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("="), (yyvsp[0].expr)); }
#line 1949 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 252 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("<>"), (yyvsp[0].expr)); }
#line 1955 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 253 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("<"), (yyvsp[0].expr)); }
#line 1961 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 254 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string("<="), (yyvsp[0].expr)); }
#line 1967 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 255 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string(">"), (yyvsp[0].expr)); }
#line 1973 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 256 "parser.ypp" /* yacc.c:1646  */
    { (yyval.rvalue) = new BinOp((yyvsp[-2].expr), std::string(">="), (yyvsp[0].expr)); }
#line 1979 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 261 "parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].lvalue); }
#line 1985 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 262 "parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].rvalue); }
#line 1991 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 266 "parser.ypp" /* yacc.c:1646  */
    { (yyval.call) = new Call(std::string((yyvsp[-2].str))); }
#line 1997 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 267 "parser.ypp" /* yacc.c:1646  */
    { (yyval.call) = new Call(std::string((yyvsp[-3].str)), (yyvsp[-1].expr_list)); }
#line 2003 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 270 "parser.ypp" /* yacc.c:1646  */
    { (yyval.expr_list) = new Expr_vector((yyvsp[0].expr)); }
#line 2009 "parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 271 "parser.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].expr_list)->append_expr((yyvsp[0].expr)); (yyval.expr_list) = (yyvsp[-2].expr_list); }
#line 2015 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2019 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 274 "parser.ypp" /* yacc.c:1906  */


int main ()
{
  int result = yyparse();
  if (result == 0) {
    std::cout << "Success.\n";
  }
  return result;
}

