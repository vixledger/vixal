/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:339  */

#include <algorithm>
#include "xdrc/xdrc_internal.h"

string xdr_unbounded = "";
static int proc_compare(const void *, const void *);
static int vers_compare(const void *, const void *);
static string getnewid(string, bool repeats_bad);

#line 76 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:339  */

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
   by #include "tokens.h".  */
#ifndef YY_YY_USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED
# define YY_YY_USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED
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
    T_ID = 258,
    T_QID = 259,
    T_NUM = 260,
    T_CONST = 261,
    T_STRUCT = 262,
    T_UNION = 263,
    T_ENUM = 264,
    T_TYPEDEF = 265,
    T_PROGRAM = 266,
    T_NAMESPACE = 267,
    T_BOOL = 268,
    T_UNSIGNED = 269,
    T_INT = 270,
    T_HYPER = 271,
    T_FLOAT = 272,
    T_DOUBLE = 273,
    T_QUADRUPLE = 274,
    T_VOID = 275,
    T_VERSION = 276,
    T_SWITCH = 277,
    T_CASE = 278,
    T_DEFAULT = 279,
    T_OPAQUE = 280,
    T_STRING = 281
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 149 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:358  */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   237

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

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
      33,    34,    39,     2,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    28,
      35,    27,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    58,    58,    59,    59,    62,    63,    64,    65,    66,
      67,    68,    71,    78,    88,    97,    98,   104,   106,   109,
     112,   121,   122,   125,   131,   138,   144,   153,   154,   157,
     170,   184,   185,   193,   199,   207,   219,   234,   245,   244,
     259,   259,   263,   262,   278,   278,   281,   294,   293,   306,
     307,   312,   317,   324,   325,   328,   330,   333,   336,   339,
     342,   345,   347,   354,   354,   357,   358,   364,   365,   371,
     371,   374,   375,   376,   377,   378,   383,   384,   385,   386,
     389,   389,   392,   395,   398,   398
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ID", "T_QID", "T_NUM", "T_CONST",
  "T_STRUCT", "T_UNION", "T_ENUM", "T_TYPEDEF", "T_PROGRAM", "T_NAMESPACE",
  "T_BOOL", "T_UNSIGNED", "T_INT", "T_HYPER", "T_FLOAT", "T_DOUBLE",
  "T_QUADRUPLE", "T_VOID", "T_VERSION", "T_SWITCH", "T_CASE", "T_DEFAULT",
  "T_OPAQUE", "T_STRING", "'='", "';'", "','", "'{'", "'}'", "':'", "'('",
  "')'", "'<'", "'>'", "'['", "']'", "'*'", "$accept", "file", "$@1",
  "definition", "def_type", "def_const", "enum_tag", "enum_tag_list",
  "enum_body", "def_enum", "comma_warn", "declaration_list", "struct_body",
  "def_struct", "union_case", "union_case_list", "union_decl",
  "union_case_spec", "union_case_spec_list", "union_body", "def_union",
  "def_program", "$@2", "version_list", "version_decl", "$@3", "proc_list",
  "proc_decl", "def_namespace", "$@4", "type_specifier", "vec_len",
  "declaration", "type_or_void", "void_or_arg_list", "arg_list", "type",
  "base_type", "value", "number", "newid", "qid", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    61,    59,    44,
     123,   125,    58,    40,    41,    60,    62,    91,    93,    42
};
# endif

#define YYPACT_NINF -139

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-139)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -139,     9,  -139,   204,    26,    26,    26,    26,   144,    26,
      26,  -139,  -139,  -139,  -139,  -139,  -139,  -139,  -139,  -139,
      20,    12,    27,    24,  -139,  -139,    96,    27,    24,  -139,
      76,  -139,  -139,  -139,  -139,  -139,    58,    73,     4,  -139,
    -139,  -139,  -139,    43,    48,    23,   164,    61,    62,    69,
      99,    78,    12,  -139,  -139,  -139,  -139,  -139,  -139,   -22,
      16,    42,   105,  -139,  -139,  -139,    90,  -139,    49,  -139,
    -139,    68,  -139,    92,  -139,    91,  -139,    14,    23,    97,
    -139,   102,  -139,    23,   104,   113,   121,  -139,  -139,  -139,
    -139,   146,    23,    99,   119,  -139,   118,   117,  -139,  -139,
     126,  -139,  -139,    26,    -9,  -139,   125,   131,  -139,  -139,
    -139,  -139,   138,   147,   156,   149,  -139,  -139,   157,  -139,
    -139,  -139,   169,    15,   181,  -139,   160,    23,   159,  -139,
     120,  -139,    70,  -139,    17,  -139,    26,  -139,  -139,   185,
    -139,   190,  -139,  -139,  -139,  -139,  -139,   192,  -139,   187,
    -139,  -139,   169,   189,   193,  -139,   188,  -139,   194,  -139,
     197,    68,   169,  -139,   198,  -139
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     3,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     8,     5,     6,     7,     9,    10,    11,    83,
       0,     0,     0,     0,    84,    85,     0,     0,     0,    79,
      75,    71,    73,    76,    77,    78,     0,     0,     0,    12,
      49,    69,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    13,    52,    50,    72,    74,     0,
       0,     0,     0,    38,    47,    81,     0,    80,     0,    23,
      26,     0,    37,    16,    17,    21,    20,     0,     0,     0,
      62,     0,    55,     0,     0,     0,     0,     2,    14,    25,
      24,     0,     0,    22,     0,    53,     0,     0,    59,    60,
       0,    58,    61,     0,     0,    40,     3,     0,    15,    18,
      19,    54,     0,     0,     0,     0,    41,    48,     0,    57,
      56,    42,     0,     0,     0,    82,     0,     0,     0,    29,
       0,    34,     0,    64,     0,    44,     0,    63,    39,     0,
      28,     0,    30,    33,    31,    36,    35,     0,    45,     0,
      27,    32,     0,     0,     0,    65,     0,    66,    67,    43,
       0,     0,     0,    68,     0,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -139,   140,  -139,  -139,  -139,  -139,   132,  -139,   200,  -139,
    -139,  -139,   208,  -139,   100,  -139,  -139,   101,  -139,   205,
    -139,  -139,  -139,  -139,   127,  -139,  -139,   103,  -139,  -139,
    -139,    56,   -23,  -139,  -139,    74,   -65,  -139,   -67,  -138,
      -5,   -37
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     3,    11,    12,    13,    74,    75,    51,    14,
      94,    68,    53,    15,   129,   130,   143,   131,   132,    49,
      16,    17,    86,   104,   105,   124,   134,   135,    18,    87,
      38,    79,    39,   136,   156,   157,    40,    41,    66,   126,
      20,    42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      21,    22,    23,    54,    43,    44,    91,    61,    67,     2,
      96,    97,   103,    77,   154,    78,   100,    24,    25,    65,
      24,    25,   115,    69,   164,   108,    24,    25,    65,    19,
      29,    30,    31,    32,    33,    34,    35,   133,   127,   128,
      67,    67,    46,    62,    80,    90,    67,    45,   147,    48,
      95,    77,    24,    25,    50,    67,    52,    27,    28,   137,
     139,    59,    29,    30,    31,    32,    33,    34,    35,   137,
      82,    24,    25,    63,    36,    37,    60,    77,    64,    83,
      89,    29,    30,    31,    32,    33,    34,    35,   158,    70,
      67,    57,    58,   127,   128,    71,   158,    72,   114,    24,
      25,   145,    73,    52,    27,    28,    76,   144,    85,    29,
      30,    31,    32,    33,    34,    35,    81,    84,    88,    92,
      93,    36,    37,    24,    25,    98,    46,    52,    27,    28,
      99,   149,   101,    29,    30,    31,    32,    33,    34,    35,
     141,   102,   103,   127,   128,    36,    37,    24,    25,   107,
     110,    26,    27,    28,   111,   112,   117,    29,    30,    31,
      32,    33,    34,    35,   113,   118,   119,    24,    25,    36,
      37,    52,    27,    28,   125,   120,   122,    29,    30,    31,
      32,    33,    34,    35,    24,    25,   121,   123,   138,    36,
      37,   140,    24,    25,    29,    30,    31,    32,    33,    34,
      35,   133,    29,    30,    31,    32,    33,    34,    35,   155,
       4,     5,     6,     7,     8,     9,    10,   150,   151,   152,
     153,   159,   160,   161,   162,   109,   165,   106,    56,    47,
     142,   116,    55,   146,     0,   163,     0,   148
};

static const yytype_int16 yycheck[] =
{
       5,     6,     7,    26,     9,    10,    71,     3,    45,     0,
      77,    78,    21,    35,   152,    37,    83,     3,     4,     5,
       3,     4,    31,    46,   162,    92,     3,     4,     5,     3,
      13,    14,    15,    16,    17,    18,    19,    20,    23,    24,
      77,    78,    30,    39,    28,    68,    83,    27,    31,    22,
      36,    35,     3,     4,    30,    92,     7,     8,     9,   124,
     127,     3,    13,    14,    15,    16,    17,    18,    19,   134,
      28,     3,     4,    30,    25,    26,     3,    35,    30,    37,
      31,    13,    14,    15,    16,    17,    18,    19,   153,    28,
     127,    15,    16,    23,    24,    33,   161,    28,   103,     3,
       4,    31,     3,     7,     8,     9,    28,   130,     3,    13,
      14,    15,    16,    17,    18,    19,    60,    61,    28,    27,
      29,    25,    26,     3,     4,    28,    30,     7,     8,     9,
      28,   136,    28,    13,    14,    15,    16,    17,    18,    19,
      20,    28,    21,    23,    24,    25,    26,     3,     4,     3,
      31,     7,     8,     9,    36,    38,    31,    13,    14,    15,
      16,    17,    18,    19,    38,    34,    28,     3,     4,    25,
      26,     7,     8,     9,     5,    28,    27,    13,    14,    15,
      16,    17,    18,    19,     3,     4,    30,    30,    28,    25,
      26,    32,     3,     4,    13,    14,    15,    16,    17,    18,
      19,    20,    13,    14,    15,    16,    17,    18,    19,    20,
       6,     7,     8,     9,    10,    11,    12,    32,    28,    27,
      33,    28,    34,    29,    27,    93,    28,    87,    28,    21,
     130,   104,    27,   132,    -1,   161,    -1,   134
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,     0,    42,     6,     7,     8,     9,    10,    11,
      12,    43,    44,    45,    49,    53,    60,    61,    68,     3,
      80,    80,    80,    80,     3,     4,     7,     8,     9,    13,
      14,    15,    16,    17,    18,    19,    25,    26,    70,    72,
      76,    77,    81,    80,    80,    27,    30,    52,    22,    59,
      30,    48,     7,    52,    72,    59,    48,    15,    16,     3,
       3,     3,    39,    30,    30,     5,    78,    81,    51,    72,
      28,    33,    28,     3,    46,    47,    28,    35,    37,    71,
      28,    71,    28,    37,    71,     3,    62,    69,    28,    31,
      72,    76,    27,    29,    50,    36,    78,    78,    28,    28,
      78,    28,    28,    21,    63,    64,    41,     3,    78,    46,
      31,    36,    38,    38,    80,    31,    64,    31,    34,    28,
      28,    30,    27,    30,    65,     5,    79,    23,    24,    54,
      55,    57,    58,    20,    66,    67,    73,    76,    28,    78,
      32,    20,    54,    56,    72,    31,    57,    31,    67,    80,
      32,    28,    27,    33,    79,    20,    74,    75,    76,    28,
      34,    29,    27,    75,    79,    28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    41,    43,    43,    43,    43,    43,
      43,    43,    44,    44,    45,    46,    46,    47,    47,    48,
      49,    50,    50,    51,    51,    52,    53,    54,    54,    55,
      55,    56,    56,    57,    58,    58,    59,    60,    62,    61,
      63,    63,    65,    64,    66,    66,    67,    69,    68,    70,
      70,    70,    70,    71,    71,    72,    72,    72,    72,    72,
      72,    72,    72,    73,    73,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      78,    78,    79,    80,    81,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     5,     3,     1,     1,     3,     4,
       4,     0,     1,     1,     2,     3,     4,     3,     2,     1,
       2,     1,     2,     2,     1,     2,     8,     4,     0,     9,
       1,     2,     0,     9,     1,     2,     8,     0,     6,     1,
       2,     2,     2,     2,     3,     3,     6,     6,     4,     4,
       4,     4,     3,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
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
    default: /* Avoid compiler warnings. */
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
#line 58 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { checkliterals(); }
#line 1359 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 3:
#line 59 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { checkliterals(); }
#line 1365 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 4:
#line 59 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { checkliterals(); }
#line 1371 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 12:
#line 72 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::TYPEDEF);
	  *s->stypedef = (yyvsp[0].decl);
	  s->stypedef->id = getnewid(s->stypedef->id, true);
	}
#line 1382 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 13:
#line 79 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::TYPEDEF);
	  *s->stypedef = (yyvsp[0].decl);
	  s->stypedef->type = string("struct ") + (yyvsp[0].decl).type;
	  s->stypedef->id = getnewid(s->stypedef->id, true);
	}
#line 1394 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 14:
#line 89 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::CONST);
	  s->sconst->id = (yyvsp[-3].str);
	  s->sconst->val = (yyvsp[-1].str);
	}
#line 1405 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 15:
#line 97 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.cnst).id = (yyvsp[-2].str); (yyval.cnst).val = (yyvsp[0].str); }
#line 1411 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 16:
#line 99 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.cnst).id = (yyvsp[0].str);
	  yywarn("RFC4506 requires a value for each enum tag");
	}
#line 1420 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 17:
#line 105 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.const_list).select(); assert((yyval.const_list)->empty()); (yyval.const_list)->push_back(std::move((yyvsp[0].cnst))); }
#line 1426 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 18:
#line 107 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.const_list) = std::move((yyvsp[-2].const_list)); (yyval.const_list)->push_back(std::move((yyvsp[0].cnst))); }
#line 1432 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 19:
#line 109 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.const_list) = std::move((yyvsp[-2].const_list)); }
#line 1438 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 20:
#line 113 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::ENUM);
	  s->senum->id = (yyvsp[-2].str);
	  s->senum->tags = std::move(*(yyvsp[-1].const_list));
	}
#line 1449 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 22:
#line 122 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { yywarn("RFC4506 disallows comma after last enum tag"); }
#line 1455 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 23:
#line 126 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl_list).select();
	  assert((yyval.decl_list)->empty());
	  (yyval.decl_list)->push_back(std::move((yyvsp[0].decl)));
	}
#line 1465 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 24:
#line 132 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl_list) = std::move((yyvsp[-1].decl_list));
	  (yyval.decl_list)->push_back(std::move((yyvsp[0].decl)));
	}
#line 1474 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 25:
#line 139 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl_list) = std::move((yyvsp[-1].decl_list));
	}
#line 1482 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 26:
#line 145 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::STRUCT);
	  s->sstruct->id = (yyvsp[-2].str);
	  s->sstruct->decls = std::move(*(yyvsp[-1].decl_list));
	}
#line 1493 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 27:
#line 153 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = (yyvsp[-1].str); }
#line 1499 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 28:
#line 154 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = ""; }
#line 1505 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 29:
#line 158 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.ufield).select();
	  assert((yyval.ufield)->cases.empty());
	  if ((yyvsp[0].str).empty()) {
	    if ((yyval.ufield)->hasdefault) {
	      yyerror("duplicate default statement");
	      YYERROR;
	    }
	    (yyval.ufield)->hasdefault = true;
	  }
	  (yyval.ufield)->cases.push_back((yyvsp[0].str));
	}
#line 1522 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 30:
#line 171 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.ufield) = std::move((yyvsp[-1].ufield));
	  if ((yyvsp[0].str).empty()) {
	    if ((yyval.ufield)->hasdefault) {
	      yyerror("duplicate default statement");
	      YYERROR;
	    }
	    (yyval.ufield)->hasdefault = true;
	  }
	  (yyval.ufield)->cases.push_back((yyvsp[0].str));
	}
#line 1538 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 31:
#line 184 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl) = std::move((yyvsp[0].decl)); }
#line 1544 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 32:
#line 186 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl).qual = rpc_decl::SCALAR;
	  (yyval.decl).ts_which = rpc_decl::TS_ID;
	  (yyval.decl).type = "void";
	}
#line 1554 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 33:
#line 194 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.ufield) = std::move((yyvsp[-1].ufield));
	  (yyval.ufield)->decl = std::move((yyvsp[0].decl));
	}
#line 1563 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 34:
#line 200 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.ubody).select();
	  assert((yyval.ubody)->fields.empty());
	  if ((yyvsp[0].ufield)->hasdefault)
	    (yyval.ubody)->hasdefault = true;
	  (yyval.ubody)->fields.push_back(std::move(*(yyvsp[0].ufield)));
	}
#line 1575 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 35:
#line 208 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  if ((yyvsp[-1].ubody)->hasdefault && (yyvsp[0].ufield)->hasdefault) {
	    yyerror("duplicate default statement");
	    YYERROR;
	  }
	  (yyval.ubody) = std::move((yyvsp[-1].ubody));
	  if ((yyvsp[0].ufield)->hasdefault)
	    (yyval.ubody)->hasdefault = true;
	  (yyval.ubody)->fields.push_back(std::move(*(yyvsp[0].ufield)));
	}
#line 1590 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 36:
#line 220 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.ubody) = std::move((yyvsp[-1].ubody));
	  (yyval.ubody)->tagtype = (yyvsp[-5].str);
	  (yyval.ubody)->tagid = (yyvsp[-4].str);
	  int next = 0;
	  for (rpc_ufield &uf : (yyval.ubody)->fields) {
	    if (uf.decl.type == "void")
	      uf.fieldno = 0;
	    else
	      uf.fieldno = ++next;
	  }
	}
#line 1607 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 37:
#line 235 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
          symlist.push_back();
	  symlist.back().settype(rpc_sym::UNION);
	  rpc_union &u = *symlist.back().sunion;
	  u = std::move(*(yyvsp[-1].ubody));
	  u.id = (yyvsp[-2].str);
	}
#line 1619 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 38:
#line 245 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back();
	  s->settype(rpc_sym::PROGRAM);
	  s->sprogram->id = (yyvsp[-1].str);
	}
#line 1629 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 39:
#line 251 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.back();
	  s->sprogram->val = (yyvsp[-1].num);
	  qsort(s->sprogram->vers.data(), s->sprogram->vers.size(),
	        sizeof (rpc_vers), vers_compare);
	}
#line 1640 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 42:
#line 263 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.back();
	  rpc_vers *rv = &s->sprogram->vers.push_back();
	  rv->id = (yyvsp[-1].str);
	}
#line 1650 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 43:
#line 269 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.back();
	  rpc_vers *rv = &s->sprogram->vers.back();
	  rv->val = (yyvsp[-1].num);
	  qsort(rv->procs.data(), rv->procs.size(),
		sizeof (rpc_proc), proc_compare);
	}
#line 1662 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 46:
#line 282 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.back();
	  rpc_vers *rv = &s->sprogram->vers.back();
	  rpc_proc *rp = &rv->procs.push_back();
	  rp->id = (yyvsp[-6].str);
	  rp->val = (yyvsp[-1].num);
	  rp->arg = std::move(*(yyvsp[-4].str_list));
	  rp->res = (yyvsp[-7].str);
	}
#line 1676 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 47:
#line 294 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back ();
	  s->settype (rpc_sym::NAMESPACE);
	  s->sliteral = (yyvsp[-1].str);
        }
#line 1686 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 48:
#line 300 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  rpc_sym *s = &symlist.push_back ();
	  s->settype (rpc_sym::CLOSEBRACE);
	}
#line 1695 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 49:
#line 306 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl).type = (yyvsp[0].str); }
#line 1701 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 50:
#line 308 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl).ts_which = rpc_decl::TS_ENUM;
	  (yyval.decl).ts_enum.reset(new rpc_enum {"", std::move(*(yyvsp[0].const_list))});
	}
#line 1710 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 51:
#line 313 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl).ts_which = rpc_decl::TS_STRUCT;
	  (yyval.decl).ts_struct.reset(new rpc_struct {"", std::move(*(yyvsp[0].decl_list))});
	}
#line 1719 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 52:
#line 318 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.decl).ts_which = rpc_decl::TS_UNION;
	  (yyval.decl).ts_union.reset(new rpc_union {std::move(*(yyvsp[0].ubody))});
	}
#line 1728 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 53:
#line 324 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = xdr_unbounded; }
#line 1734 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 54:
#line 325 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = (yyvsp[-1].str); }
#line 1740 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 55:
#line 329 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl) = std::move((yyvsp[-2].decl)); (yyval.decl).set_id((yyvsp[-1].str)); }
#line 1746 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 56:
#line 331 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl) = std::move((yyvsp[-5].decl)); (yyval.decl).set_id((yyvsp[-4].str));
	  (yyval.decl).qual = rpc_decl::ARRAY; (yyval.decl).bound = (yyvsp[-2].str); }
#line 1753 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 57:
#line 334 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl).type = std::move((yyvsp[-5].str)); (yyval.decl).set_id((yyvsp[-4].str));
	  (yyval.decl).qual = rpc_decl::ARRAY; (yyval.decl).bound = (yyvsp[-2].str); }
#line 1760 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 58:
#line 337 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl) = std::move((yyvsp[-3].decl)); (yyval.decl).set_id((yyvsp[-2].str));
	  (yyval.decl).qual = rpc_decl::VEC; (yyval.decl).bound = (yyvsp[-1].str); }
#line 1767 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 59:
#line 340 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl).type = std::move((yyvsp[-3].str)); (yyval.decl).set_id((yyvsp[-2].str));
	  (yyval.decl).qual = rpc_decl::VEC; (yyval.decl).bound = (yyvsp[-1].str); }
#line 1774 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 60:
#line 343 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl).type = std::move((yyvsp[-3].str)); (yyval.decl).set_id((yyvsp[-2].str));
	  (yyval.decl).qual = rpc_decl::VEC; (yyval.decl).bound = (yyvsp[-1].str); }
#line 1781 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 61:
#line 346 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl) = std::move((yyvsp[-3].decl)); (yyval.decl).qual = rpc_decl::PTR; (yyval.decl).set_id((yyvsp[-1].str)); }
#line 1787 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 62:
#line 348 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.decl).set_id((yyvsp[-1].str)); (yyval.decl).type = (yyvsp[-2].str); (yyval.decl).qual = rpc_decl::VEC;
	  (yyval.decl).bound = xdr_unbounded;
	  yywarn ("strings require variable-length array declarations (<>)");
	}
#line 1796 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 64:
#line 354 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "void"; }
#line 1802 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 65:
#line 357 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str_list).select(); }
#line 1808 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 66:
#line 359 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str_list).select() = std::move(*(yyvsp[0].str_list));
	  std::reverse((yyval.str_list)->begin(), (yyval.str_list)->end());
	}
#line 1816 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 67:
#line 364 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str_list).select().push_back((yyvsp[0].str)); }
#line 1822 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 68:
#line 366 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str_list).select() = std::move(*(yyvsp[0].str_list));
	  (yyval.str_list)->push_back((yyvsp[-2].str));
        }
#line 1830 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 71:
#line 374 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "int"; }
#line 1836 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 72:
#line 375 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "unsigned"; }
#line 1842 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 73:
#line 376 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "hyper"; }
#line 1848 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 74:
#line 377 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "unsigned hyper"; }
#line 1854 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 75:
#line 379 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    {
	  (yyval.str) = "unsigned";
	  yywarn("RFC4506 requires \"int\" after \"unsigned\"");
	}
#line 1863 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 76:
#line 383 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "float"; }
#line 1869 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 77:
#line 384 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "double"; }
#line 1875 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 78:
#line 385 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "quadruple"; }
#line 1881 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 79:
#line 386 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = "bool"; }
#line 1887 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 82:
#line 392 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.num) = strtoul ((yyvsp[0].str).c_str(), NULL, 0); }
#line 1893 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;

  case 83:
#line 395 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1663  */
    { (yyval.str) = getnewid ((yyvsp[0].str), true); }
#line 1899 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
    break;


#line 1903 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.cc" /* yacc.c:1663  */
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
#line 401 "/Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/xdrpp/src/xdrc/parse.yy" /* yacc.c:1907  */

symlist_t symlist;

static int
proc_compare(void const *_a, void const *_b)
{
  rpc_proc const *a = (rpc_proc  const*) _a;
  rpc_proc const *b = (rpc_proc  const*) _b;
  return a->val < b->val ? -1 : a->val != b->val;
}

static int
vers_compare(void const *_a, void const *_b)
{
  rpc_vers const *a = (rpc_vers const *) _a;
  rpc_vers const *b = (rpc_vers const *) _b;
  return a->val < b->val ? -1 : a->val != b->val;
}

void
checkliterals()
{
  for (size_t i = 0; i < litq.size (); i++) {
    rpc_sym *s = &symlist.push_back ();
    s->settype (rpc_sym::LITERAL);
    *s->sliteral = litq[i];
  }
  litq.clear ();
}

static string
getnewid(string id, bool repeats_bad)
{
  if (!repeats_bad) { /* noop */ }
  else if (ids.find(id) != ids.end()) {
    yywarn(string("redefinition of symbol ") + id);
  } else {
    ids.insert(id);
  }
  // Possible place to add namespace scope::
  return id;
}
