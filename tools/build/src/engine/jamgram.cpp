/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 99 "src/engine/jamgram.y"

#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "compile.h"
#include "object.h"
#include "rules.h"

# define YYINITDEPTH 5000 /* for C++ parsing */
# define YYMAXDEPTH 10000	/* for OSF and other less endowed yaccs */

# define F0 -1
static PARSE * P0 = nullptr;
# define S0 (OBJECT *)0

# define pappend( l,r )    	parse_make( PARSE_APPEND,l,r,P0,S0,S0,0 )
# define peval( c,l,r )	    parse_make( PARSE_EVAL,l,r,P0,S0,S0,c )
# define pfor( s,l,r,x )    parse_make( PARSE_FOREACH,l,r,P0,s,S0,x )
# define pif( l,r,t )       parse_make( PARSE_IF,l,r,t,S0,S0,0 )
# define pincl( l )         parse_make( PARSE_INCLUDE,l,P0,P0,S0,S0,0 )
# define plist( s )         parse_make( PARSE_LIST,P0,P0,P0,s,S0,0 )
# define plocal( l,r,t )    parse_make( PARSE_LOCAL,l,r,t,S0,S0,0 )
# define pmodule( l,r )     parse_make( PARSE_MODULE,l,r,P0,S0,S0,0 )
# define pclass( l,r )      parse_make( PARSE_CLASS,l,r,P0,S0,S0,0 )
# define pnull()            parse_make( PARSE_NULL,P0,P0,P0,S0,S0,0 )
# define pon( l,r )         parse_make( PARSE_ON,l,r,P0,S0,S0,0 )
# define prule( s,p )       parse_make( PARSE_RULE,p,P0,P0,s,S0,0 )
# define prules( l,r )      parse_make( PARSE_RULES,l,r,P0,S0,S0,0 )
# define pset( l,r,a )      parse_make( PARSE_SET,l,r,P0,S0,S0,a )
# define pset1( l,r,t,a )   parse_make( PARSE_SETTINGS,l,r,t,S0,S0,a )
# define psetc( s,p,a,l )   parse_make( PARSE_SETCOMP,p,a,P0,s,S0,l )
# define psete( s,l,s1,f )  parse_make( PARSE_SETEXEC,l,P0,P0,s,s1,f )
# define pswitch( l,r )     parse_make( PARSE_SWITCH,l,r,P0,S0,S0,0 )
# define pwhile( l,r )      parse_make( PARSE_WHILE,l,r,P0,S0,S0,0 )
# define preturn( l )       parse_make( PARSE_RETURN,l,P0,P0,S0,S0,0 )
# define pbreak()           parse_make( PARSE_BREAK,P0,P0,P0,S0,S0,0 )
# define pcontinue()        parse_make( PARSE_CONTINUE,P0,P0,P0,S0,S0,0 )

# define pnode( l,r )       parse_make( F0,l,r,P0,S0,S0,0 )
# define psnode( s,l )      parse_make( F0,l,P0,P0,s,S0,0 )


#line 116 "src/engine/jamgram.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "jamgram.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL__BANG_t = 3,                    /* _BANG_t  */
  YYSYMBOL__BANG_EQUALS_t = 4,             /* _BANG_EQUALS_t  */
  YYSYMBOL__AMPER_t = 5,                   /* _AMPER_t  */
  YYSYMBOL__AMPERAMPER_t = 6,              /* _AMPERAMPER_t  */
  YYSYMBOL__LPAREN_t = 7,                  /* _LPAREN_t  */
  YYSYMBOL__RPAREN_t = 8,                  /* _RPAREN_t  */
  YYSYMBOL__PLUS_EQUALS_t = 9,             /* _PLUS_EQUALS_t  */
  YYSYMBOL__COLON_t = 10,                  /* _COLON_t  */
  YYSYMBOL__SEMIC_t = 11,                  /* _SEMIC_t  */
  YYSYMBOL__LANGLE_t = 12,                 /* _LANGLE_t  */
  YYSYMBOL__LANGLE_EQUALS_t = 13,          /* _LANGLE_EQUALS_t  */
  YYSYMBOL__EQUALS_t = 14,                 /* _EQUALS_t  */
  YYSYMBOL__RANGLE_t = 15,                 /* _RANGLE_t  */
  YYSYMBOL__RANGLE_EQUALS_t = 16,          /* _RANGLE_EQUALS_t  */
  YYSYMBOL__QUESTION_EQUALS_t = 17,        /* _QUESTION_EQUALS_t  */
  YYSYMBOL__LBRACKET_t = 18,               /* _LBRACKET_t  */
  YYSYMBOL__RBRACKET_t = 19,               /* _RBRACKET_t  */
  YYSYMBOL_ACTIONS_t = 20,                 /* ACTIONS_t  */
  YYSYMBOL_BIND_t = 21,                    /* BIND_t  */
  YYSYMBOL_BREAK_t = 22,                   /* BREAK_t  */
  YYSYMBOL_CASE_t = 23,                    /* CASE_t  */
  YYSYMBOL_CLASS_t = 24,                   /* CLASS_t  */
  YYSYMBOL_CONTINUE_t = 25,                /* CONTINUE_t  */
  YYSYMBOL_DEFAULT_t = 26,                 /* DEFAULT_t  */
  YYSYMBOL_ELSE_t = 27,                    /* ELSE_t  */
  YYSYMBOL_EXISTING_t = 28,                /* EXISTING_t  */
  YYSYMBOL_FOR_t = 29,                     /* FOR_t  */
  YYSYMBOL_IF_t = 30,                      /* IF_t  */
  YYSYMBOL_IGNORE_t = 31,                  /* IGNORE_t  */
  YYSYMBOL_IN_t = 32,                      /* IN_t  */
  YYSYMBOL_INCLUDE_t = 33,                 /* INCLUDE_t  */
  YYSYMBOL_LOCAL_t = 34,                   /* LOCAL_t  */
  YYSYMBOL_MODULE_t = 35,                  /* MODULE_t  */
  YYSYMBOL_ON_t = 36,                      /* ON_t  */
  YYSYMBOL_PIECEMEAL_t = 37,               /* PIECEMEAL_t  */
  YYSYMBOL_QUIETLY_t = 38,                 /* QUIETLY_t  */
  YYSYMBOL_RETURN_t = 39,                  /* RETURN_t  */
  YYSYMBOL_RULE_t = 40,                    /* RULE_t  */
  YYSYMBOL_SWITCH_t = 41,                  /* SWITCH_t  */
  YYSYMBOL_TOGETHER_t = 42,                /* TOGETHER_t  */
  YYSYMBOL_UPDATED_t = 43,                 /* UPDATED_t  */
  YYSYMBOL_WHILE_t = 44,                   /* WHILE_t  */
  YYSYMBOL__LBRACE_t = 45,                 /* _LBRACE_t  */
  YYSYMBOL__BAR_t = 46,                    /* _BAR_t  */
  YYSYMBOL__BARBAR_t = 47,                 /* _BARBAR_t  */
  YYSYMBOL__RBRACE_t = 48,                 /* _RBRACE_t  */
  YYSYMBOL_ARG = 49,                       /* ARG  */
  YYSYMBOL_STRING = 50,                    /* STRING  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_run = 52,                       /* run  */
  YYSYMBOL_block = 53,                     /* block  */
  YYSYMBOL_rules = 54,                     /* rules  */
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_56_2 = 56,                      /* $@2  */
  YYSYMBOL_null = 57,                      /* null  */
  YYSYMBOL_assign_list_opt = 58,           /* assign_list_opt  */
  YYSYMBOL_59_3 = 59,                      /* $@3  */
  YYSYMBOL_arglist_opt = 60,               /* arglist_opt  */
  YYSYMBOL_local_opt = 61,                 /* local_opt  */
  YYSYMBOL_else_opt = 62,                  /* else_opt  */
  YYSYMBOL_rule = 63,                      /* rule  */
  YYSYMBOL_64_4 = 64,                      /* $@4  */
  YYSYMBOL_65_5 = 65,                      /* $@5  */
  YYSYMBOL_66_6 = 66,                      /* $@6  */
  YYSYMBOL_67_7 = 67,                      /* $@7  */
  YYSYMBOL_68_8 = 68,                      /* $@8  */
  YYSYMBOL_69_9 = 69,                      /* $@9  */
  YYSYMBOL_70_10 = 70,                     /* $@10  */
  YYSYMBOL_71_11 = 71,                     /* $@11  */
  YYSYMBOL_72_12 = 72,                     /* $@12  */
  YYSYMBOL_73_13 = 73,                     /* $@13  */
  YYSYMBOL_74_14 = 74,                     /* $@14  */
  YYSYMBOL_75_15 = 75,                     /* $@15  */
  YYSYMBOL_76_16 = 76,                     /* $@16  */
  YYSYMBOL_77_17 = 77,                     /* $@17  */
  YYSYMBOL_78_18 = 78,                     /* $@18  */
  YYSYMBOL_79_19 = 79,                     /* $@19  */
  YYSYMBOL_80_20 = 80,                     /* $@20  */
  YYSYMBOL_81_21 = 81,                     /* $@21  */
  YYSYMBOL_82_22 = 82,                     /* $@22  */
  YYSYMBOL_83_23 = 83,                     /* $@23  */
  YYSYMBOL_84_24 = 84,                     /* $@24  */
  YYSYMBOL_85_25 = 85,                     /* $@25  */
  YYSYMBOL_86_26 = 86,                     /* $@26  */
  YYSYMBOL_assign = 87,                    /* assign  */
  YYSYMBOL_expr = 88,                      /* expr  */
  YYSYMBOL_89_27 = 89,                     /* $@27  */
  YYSYMBOL_90_28 = 90,                     /* $@28  */
  YYSYMBOL_91_29 = 91,                     /* $@29  */
  YYSYMBOL_92_30 = 92,                     /* $@30  */
  YYSYMBOL_93_31 = 93,                     /* $@31  */
  YYSYMBOL_94_32 = 94,                     /* $@32  */
  YYSYMBOL_95_33 = 95,                     /* $@33  */
  YYSYMBOL_96_34 = 96,                     /* $@34  */
  YYSYMBOL_97_35 = 97,                     /* $@35  */
  YYSYMBOL_98_36 = 98,                     /* $@36  */
  YYSYMBOL_99_37 = 99,                     /* $@37  */
  YYSYMBOL_100_38 = 100,                   /* $@38  */
  YYSYMBOL_101_39 = 101,                   /* $@39  */
  YYSYMBOL_cases = 102,                    /* cases  */
  YYSYMBOL_case = 103,                     /* case  */
  YYSYMBOL_104_40 = 104,                   /* $@40  */
  YYSYMBOL_105_41 = 105,                   /* $@41  */
  YYSYMBOL_lol = 106,                      /* lol  */
  YYSYMBOL_list = 107,                     /* list  */
  YYSYMBOL_listp = 108,                    /* listp  */
  YYSYMBOL_arg = 109,                      /* arg  */
  YYSYMBOL_110_42 = 110,                   /* @42  */
  YYSYMBOL_func = 111,                     /* func  */
  YYSYMBOL_112_43 = 112,                   /* $@43  */
  YYSYMBOL_113_44 = 113,                   /* $@44  */
  YYSYMBOL_114_45 = 114,                   /* $@45  */
  YYSYMBOL_eflags = 115,                   /* eflags  */
  YYSYMBOL_eflag = 116,                    /* eflag  */
  YYSYMBOL_bindlist = 117,                 /* bindlist  */
  YYSYMBOL_118_46 = 118                    /* $@46  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   242

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  207

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   146,   146,   148,   159,   161,   165,   167,   169,   169,
     169,   174,   177,   177,   179,   183,   186,   189,   192,   195,
     198,   200,   202,   202,   204,   204,   206,   206,   208,   208,
     208,   210,   210,   212,   214,   216,   216,   216,   218,   218,
     218,   220,   220,   220,   222,   222,   222,   224,   224,   224,
     226,   226,   226,   228,   228,   228,   228,   230,   233,   235,
     232,   244,   246,   248,   250,   257,   259,   259,   261,   261,
     263,   263,   265,   265,   267,   267,   269,   269,   271,   271,
     273,   273,   275,   275,   277,   277,   279,   279,   281,   281,
     283,   283,   295,   296,   300,   300,   300,   309,   311,   321,
     326,   327,   331,   333,   333,   342,   342,   344,   344,   346,
     346,   357,   358,   362,   364,   366,   368,   370,   372,   382,
     383,   383
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "_BANG_t",
  "_BANG_EQUALS_t", "_AMPER_t", "_AMPERAMPER_t", "_LPAREN_t", "_RPAREN_t",
  "_PLUS_EQUALS_t", "_COLON_t", "_SEMIC_t", "_LANGLE_t",
  "_LANGLE_EQUALS_t", "_EQUALS_t", "_RANGLE_t", "_RANGLE_EQUALS_t",
  "_QUESTION_EQUALS_t", "_LBRACKET_t", "_RBRACKET_t", "ACTIONS_t",
  "BIND_t", "BREAK_t", "CASE_t", "CLASS_t", "CONTINUE_t", "DEFAULT_t",
  "ELSE_t", "EXISTING_t", "FOR_t", "IF_t", "IGNORE_t", "IN_t", "INCLUDE_t",
  "LOCAL_t", "MODULE_t", "ON_t", "PIECEMEAL_t", "QUIETLY_t", "RETURN_t",
  "RULE_t", "SWITCH_t", "TOGETHER_t", "UPDATED_t", "WHILE_t", "_LBRACE_t",
  "_BAR_t", "_BARBAR_t", "_RBRACE_t", "ARG", "STRING", "$accept", "run",
  "block", "rules", "$@1", "$@2", "null", "assign_list_opt", "$@3",
  "arglist_opt", "local_opt", "else_opt", "rule", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "$@16", "$@17", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24",
  "$@25", "$@26", "assign", "expr", "$@27", "$@28", "$@29", "$@30", "$@31",
  "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "cases",
  "case", "$@40", "$@41", "lol", "list", "listp", "arg", "@42", "func",
  "$@43", "$@44", "$@45", "eflags", "eflag", "bindlist", "$@46", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-119)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-25)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     140,  -119,  -119,     1,  -119,     2,   -18,  -119,  -119,   -23,
    -119,    -9,  -119,  -119,  -119,   140,    12,    31,  -119,     4,
     140,    77,   -17,   186,  -119,  -119,  -119,  -119,    -7,     3,
    -119,  -119,  -119,  -119,   177,  -119,  -119,     3,    -5,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,    33,  -119,
    -119,    -9,  -119,    29,  -119,  -119,  -119,  -119,  -119,  -119,
      35,  -119,    14,    50,    -9,    34,  -119,  -119,    23,    39,
      52,    53,    40,  -119,    66,    45,    94,  -119,    67,    30,
    -119,  -119,  -119,    16,  -119,  -119,  -119,    47,  -119,  -119,
    -119,  -119,     3,     3,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,    84,
    -119,  -119,  -119,    51,  -119,  -119,    32,   105,  -119,  -119,
    -119,  -119,  -119,   140,  -119,  -119,  -119,    68,     3,     3,
       3,     3,     3,     3,     3,     3,   140,     3,     3,  -119,
    -119,  -119,   140,    95,   140,   110,  -119,  -119,  -119,  -119,
    -119,    69,    73,    87,  -119,    89,   139,   139,  -119,  -119,
      89,  -119,  -119,    90,   226,   226,  -119,  -119,   140,    91,
    -119,    97,    95,    98,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,   108,  -119,  -119,    88,  -119,  -119,  -119,   141,
     177,   145,   102,   140,   177,  -119,   149,  -119,  -119,  -119,
    -119,   115,  -119,  -119,  -119,   140,  -119
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,   103,   111,     0,    47,     0,    18,    41,    22,     8,
      44,     0,    31,    38,    50,    11,   102,     0,     3,     0,
       6,     0,     0,     0,    33,   100,    34,    17,     0,     0,
     100,   100,   100,   102,    18,   100,   100,     0,     0,     5,
       4,   100,     1,    53,     7,    62,    61,    63,     0,    28,
      26,     0,   105,     0,   118,   115,   117,   116,   114,   113,
     119,   112,     0,    97,    99,     0,    88,    90,     0,    65,
       0,    11,     0,    57,     0,     0,    51,    21,     0,     0,
      64,   100,   100,     0,   100,   104,   120,     0,    48,   100,
     101,    35,     0,     0,    68,    78,    80,    70,    72,    66,
      74,    76,    42,    82,    84,    86,    23,    12,    14,     0,
      45,    32,    39,     0,    25,    54,     0,     0,   109,   107,
     106,   100,    58,    11,    98,   100,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,     0,     0,   100,
     100,     9,    11,    92,    11,    16,    29,    27,   100,   100,
     121,     0,     0,     0,    91,    69,    79,    81,    71,    73,
      67,    75,    77,     0,    83,    85,    87,    13,    11,     0,
      94,     0,    92,     0,   100,    55,   100,   110,   108,    59,
      49,    36,    20,    10,    46,     0,    40,    93,    52,     0,
      18,     0,     0,    11,    18,    43,     0,    15,    56,    30,
      60,     0,    19,    95,    37,    11,    96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -119,  -119,  -118,    25,  -119,  -119,    96,  -119,  -119,  -119,
     160,  -119,   -33,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,    55,    -4,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,     5,  -119,  -119,  -119,   -27,   -28,  -119,     0,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    17,    38,    39,    31,   168,    40,   109,   140,   175,
      19,   195,    20,    30,    41,    82,    81,   176,    35,   125,
     193,    36,   143,    29,   136,    32,   142,    25,   123,    37,
     113,    79,   145,   190,   151,   192,    50,    68,   133,   128,
     131,   132,   134,   135,   129,   130,   137,   138,   139,    92,
      93,   171,   172,   185,   205,    62,    63,    64,    69,    22,
      53,    84,   149,   148,    23,    61,    87,   121
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    73,    70,    71,    72,   152,    66,    74,    75,     1,
      67,    34,    24,    26,    78,    21,    27,   -17,   163,    51,
      21,     1,   -24,   -24,   169,    18,   173,    94,    95,    96,
     -24,    42,    52,    76,    21,    97,    98,    99,   100,   101,
      33,    45,    65,    77,    43,    44,    46,    80,    85,    47,
     183,    83,    33,   116,   117,   118,    86,   120,    48,    88,
      89,   -24,   124,   106,    90,   119,    91,   107,   102,   103,
     104,   105,    94,    95,    96,   201,   154,   111,   114,   115,
      97,    98,    99,   100,   101,   110,    45,   206,   126,   127,
     112,    46,   122,   150,    47,   141,   144,   153,    94,    95,
      96,    97,    98,    48,   100,   101,    97,    98,    99,   100,
     101,   166,   167,    49,   103,   104,   147,   174,   170,   179,
     177,   180,   178,    21,   155,   156,   157,   158,   159,   160,
     161,   162,   181,   164,   165,   194,    21,   196,   182,   184,
     103,   104,    21,    94,    21,   186,   188,   189,   191,   197,
     200,    97,    98,    99,   100,   101,   199,   198,     1,   203,
       2,   202,     3,   204,     4,     5,    28,   108,    21,     6,
       7,   146,     0,     8,     9,    10,    11,   187,     0,    12,
     -18,    13,     0,     0,    14,    15,     0,     0,     0,    16,
      21,     0,     0,    21,    21,     1,     0,     2,     0,     3,
       0,     4,     5,     0,     0,    21,     6,     7,     0,     0,
       8,    27,    10,    11,    54,     0,    12,    55,    13,     0,
       0,    14,    15,    56,    57,     0,    16,     0,    58,    59,
      94,    95,    96,     0,     0,    60,     0,     0,    97,    98,
      99,   100,   101
};

static const yytype_int16 yycheck[] =
{
       0,    34,    30,    31,    32,   123,     3,    35,    36,    18,
       7,    11,    11,    11,    41,    15,    34,    40,   136,    36,
      20,    18,    10,    11,   142,     0,   144,     4,     5,     6,
      18,     0,    49,    37,    34,    12,    13,    14,    15,    16,
      49,     9,    49,    48,    40,    20,    14,    14,    19,    17,
     168,    51,    49,    81,    82,    39,    21,    84,    26,    45,
      10,    49,    89,    11,    64,    49,    32,    14,    45,    46,
      47,    32,     4,     5,     6,   193,     8,    11,    11,    49,
      12,    13,    14,    15,    16,    45,     9,   205,    92,    93,
      45,    14,    45,   121,    17,    11,    45,   125,     4,     5,
       6,    12,    13,    26,    15,    16,    12,    13,    14,    15,
      16,   139,   140,    36,    46,    47,    11,     7,    23,    50,
     148,    48,   149,   123,   128,   129,   130,   131,   132,   133,
     134,   135,    45,   137,   138,    27,   136,    49,    48,    48,
      46,    47,   142,     4,   144,    48,    48,   174,   176,     8,
      48,    12,    13,    14,    15,    16,    11,   190,    18,    10,
      20,   194,    22,    48,    24,    25,     6,    71,   168,    29,
      30,   116,    -1,    33,    34,    35,    36,   172,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
     190,    -1,    -1,   193,   194,    18,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,   205,    29,    30,    -1,    -1,
      33,    34,    35,    36,    28,    -1,    39,    31,    41,    -1,
      -1,    44,    45,    37,    38,    -1,    49,    -1,    42,    43,
       4,     5,     6,    -1,    -1,    49,    -1,    -1,    12,    13,
      14,    15,    16
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    18,    20,    22,    24,    25,    29,    30,    33,    34,
      35,    36,    39,    41,    44,    45,    49,    52,    54,    61,
      63,   109,   110,   115,    11,    78,    11,    34,    61,    74,
      64,    55,    76,    49,   109,    69,    72,    80,    53,    54,
      57,    65,     0,    40,    54,     9,    14,    17,    26,    36,
      87,    36,    49,   111,    28,    31,    37,    38,    42,    43,
      49,   116,   106,   107,   108,    49,     3,     7,    88,   109,
     107,   107,   107,    63,   107,   107,    88,    48,   106,    82,
      14,    67,    66,   109,   112,    19,    21,   117,    45,    10,
     109,    32,   100,   101,     4,     5,     6,    12,    13,    14,
      15,    16,    45,    46,    47,    32,    11,    14,    57,    58,
      45,    11,    45,    81,    11,    49,   107,   107,    39,    49,
     106,   118,    45,    79,   106,    70,    88,    88,    90,    95,
      96,    91,    92,    89,    93,    94,    75,    97,    98,    99,
      59,    11,    77,    73,    45,    83,    87,    11,   114,   113,
     107,    85,    53,   107,     8,    88,    88,    88,    88,    88,
      88,    88,    88,    53,    88,    88,   107,   107,    56,    53,
      23,   102,   103,    53,     7,    60,    68,   107,   106,    50,
      48,    45,    48,    53,    48,   104,    48,   102,    48,   106,
      84,   107,    86,    71,    27,    62,    49,     8,    63,    11,
      48,    53,    63,    10,    48,   105,    53
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    52,    53,    53,    54,    54,    55,    56,
      54,    57,    59,    58,    58,    60,    60,    61,    61,    62,
      62,    63,    64,    63,    65,    63,    66,    63,    67,    68,
      63,    69,    63,    63,    63,    70,    71,    63,    72,    73,
      63,    74,    75,    63,    76,    77,    63,    78,    79,    63,
      80,    81,    63,    82,    83,    84,    63,    63,    85,    86,
      63,    87,    87,    87,    87,    88,    89,    88,    90,    88,
      91,    88,    92,    88,    93,    88,    94,    88,    95,    88,
      96,    88,    97,    88,    98,    88,    99,    88,   100,    88,
     101,    88,   102,   102,   104,   105,   103,   106,   106,   107,
     108,   108,   109,   110,   109,   112,   111,   113,   111,   114,
     111,   115,   115,   116,   116,   116,   116,   116,   116,   117,
     118,   117
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     1,     1,     2,     0,     0,
       7,     0,     0,     3,     1,     3,     0,     1,     0,     2,
       0,     3,     0,     4,     0,     4,     0,     5,     0,     0,
       8,     0,     4,     2,     2,     0,     0,    10,     0,     0,
       7,     0,     0,     8,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     0,     8,     3,     0,     0,
       9,     1,     1,     1,     2,     1,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     3,
       0,     4,     0,     2,     0,     0,     6,     1,     3,     1,
       0,     2,     1,     0,     4,     0,     3,     0,     5,     0,
       5,     0,     2,     1,     1,     1,     1,     1,     1,     0,
       0,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 3: /* run: rules  */
#line 149 "src/engine/jamgram.y"
                { parse_save( yyvsp[0].parse ); }
#line 1384 "src/engine/jamgram.cpp"
    break;

  case 4: /* block: null  */
#line 160 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 1390 "src/engine/jamgram.cpp"
    break;

  case 5: /* block: rules  */
#line 162 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 1396 "src/engine/jamgram.cpp"
    break;

  case 6: /* rules: rule  */
#line 166 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 1402 "src/engine/jamgram.cpp"
    break;

  case 7: /* rules: rule rules  */
#line 168 "src/engine/jamgram.y"
                { yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); }
#line 1408 "src/engine/jamgram.cpp"
    break;

  case 8: /* $@1: %empty  */
#line 169 "src/engine/jamgram.y"
                  { yymode( SCAN_ASSIGN ); }
#line 1414 "src/engine/jamgram.cpp"
    break;

  case 9: /* $@2: %empty  */
#line 169 "src/engine/jamgram.y"
                                                                           { yymode( SCAN_NORMAL ); }
#line 1420 "src/engine/jamgram.cpp"
    break;

  case 10: /* rules: LOCAL_t $@1 list assign_list_opt _SEMIC_t $@2 block  */
#line 170 "src/engine/jamgram.y"
                { yyval.parse = plocal( yyvsp[-4].parse, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1426 "src/engine/jamgram.cpp"
    break;

  case 11: /* null: %empty  */
#line 174 "src/engine/jamgram.y"
        { yyval.parse = pnull(); }
#line 1432 "src/engine/jamgram.cpp"
    break;

  case 12: /* $@3: %empty  */
#line 177 "src/engine/jamgram.y"
                            { yymode( SCAN_PUNCT ); }
#line 1438 "src/engine/jamgram.cpp"
    break;

  case 13: /* assign_list_opt: _EQUALS_t $@3 list  */
#line 178 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_SET; }
#line 1444 "src/engine/jamgram.cpp"
    break;

  case 14: /* assign_list_opt: null  */
#line 180 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_APPEND; }
#line 1450 "src/engine/jamgram.cpp"
    break;

  case 15: /* arglist_opt: _LPAREN_t lol _RPAREN_t  */
#line 184 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[-1].parse; }
#line 1456 "src/engine/jamgram.cpp"
    break;

  case 16: /* arglist_opt: %empty  */
#line 186 "src/engine/jamgram.y"
                { yyval.parse = P0; }
#line 1462 "src/engine/jamgram.cpp"
    break;

  case 17: /* local_opt: LOCAL_t  */
#line 190 "src/engine/jamgram.y"
                { yyval.number = 1; }
#line 1468 "src/engine/jamgram.cpp"
    break;

  case 18: /* local_opt: %empty  */
#line 192 "src/engine/jamgram.y"
                { yyval.number = 0; }
#line 1474 "src/engine/jamgram.cpp"
    break;

  case 19: /* else_opt: ELSE_t rule  */
#line 196 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 1480 "src/engine/jamgram.cpp"
    break;

  case 20: /* else_opt: %empty  */
#line 198 "src/engine/jamgram.y"
                { yyval.parse = pnull(); }
#line 1486 "src/engine/jamgram.cpp"
    break;

  case 21: /* rule: _LBRACE_t block _RBRACE_t  */
#line 201 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[-1].parse; }
#line 1492 "src/engine/jamgram.cpp"
    break;

  case 22: /* $@4: %empty  */
#line 202 "src/engine/jamgram.y"
                    { yymode( SCAN_PUNCT ); }
#line 1498 "src/engine/jamgram.cpp"
    break;

  case 23: /* rule: INCLUDE_t $@4 list _SEMIC_t  */
#line 203 "src/engine/jamgram.y"
                { yyval.parse = pincl( yyvsp[-1].parse ); yymode( SCAN_NORMAL ); }
#line 1504 "src/engine/jamgram.cpp"
    break;

  case 24: /* $@5: %empty  */
#line 204 "src/engine/jamgram.y"
              { yymode( SCAN_PUNCT ); }
#line 1510 "src/engine/jamgram.cpp"
    break;

  case 25: /* rule: ARG $@5 lol _SEMIC_t  */
#line 205 "src/engine/jamgram.y"
                { yyval.parse = prule( yyvsp[-3].string, yyvsp[-1].parse ); yymode( SCAN_NORMAL ); }
#line 1516 "src/engine/jamgram.cpp"
    break;

  case 26: /* $@6: %empty  */
#line 206 "src/engine/jamgram.y"
                     { yymode( SCAN_PUNCT ); }
#line 1522 "src/engine/jamgram.cpp"
    break;

  case 27: /* rule: arg assign $@6 list _SEMIC_t  */
#line 207 "src/engine/jamgram.y"
                { yyval.parse = pset( yyvsp[-4].parse, yyvsp[-1].parse, yyvsp[-3].number ); yymode( SCAN_NORMAL ); }
#line 1528 "src/engine/jamgram.cpp"
    break;

  case 28: /* $@7: %empty  */
#line 208 "src/engine/jamgram.y"
                   { yymode( SCAN_ASSIGN ); }
#line 1534 "src/engine/jamgram.cpp"
    break;

  case 29: /* $@8: %empty  */
#line 208 "src/engine/jamgram.y"
                                                          { yymode( SCAN_PUNCT ); }
#line 1540 "src/engine/jamgram.cpp"
    break;

  case 30: /* rule: arg ON_t $@7 list assign $@8 list _SEMIC_t  */
#line 209 "src/engine/jamgram.y"
                { yyval.parse = pset1( yyvsp[-7].parse, yyvsp[-4].parse, yyvsp[-1].parse, yyvsp[-3].number ); yymode( SCAN_NORMAL ); }
#line 1546 "src/engine/jamgram.cpp"
    break;

  case 31: /* $@9: %empty  */
#line 210 "src/engine/jamgram.y"
                   { yymode( SCAN_PUNCT ); }
#line 1552 "src/engine/jamgram.cpp"
    break;

  case 32: /* rule: RETURN_t $@9 list _SEMIC_t  */
#line 211 "src/engine/jamgram.y"
                { yyval.parse = preturn( yyvsp[-1].parse ); yymode( SCAN_NORMAL ); }
#line 1558 "src/engine/jamgram.cpp"
    break;

  case 33: /* rule: BREAK_t _SEMIC_t  */
#line 213 "src/engine/jamgram.y"
        { yyval.parse = pbreak(); }
#line 1564 "src/engine/jamgram.cpp"
    break;

  case 34: /* rule: CONTINUE_t _SEMIC_t  */
#line 215 "src/engine/jamgram.y"
        { yyval.parse = pcontinue(); }
#line 1570 "src/engine/jamgram.cpp"
    break;

  case 35: /* $@10: %empty  */
#line 216 "src/engine/jamgram.y"
                                   { yymode( SCAN_PUNCT ); }
#line 1576 "src/engine/jamgram.cpp"
    break;

  case 36: /* $@11: %empty  */
#line 216 "src/engine/jamgram.y"
                                                                            { yymode( SCAN_NORMAL ); }
#line 1582 "src/engine/jamgram.cpp"
    break;

  case 37: /* rule: FOR_t local_opt ARG IN_t $@10 list _LBRACE_t $@11 block _RBRACE_t  */
#line 217 "src/engine/jamgram.y"
                { yyval.parse = pfor( yyvsp[-7].string, yyvsp[-4].parse, yyvsp[-1].parse, yyvsp[-8].number ); }
#line 1588 "src/engine/jamgram.cpp"
    break;

  case 38: /* $@12: %empty  */
#line 218 "src/engine/jamgram.y"
                   { yymode( SCAN_PUNCT ); }
#line 1594 "src/engine/jamgram.cpp"
    break;

  case 39: /* $@13: %empty  */
#line 218 "src/engine/jamgram.y"
                                                            { yymode( SCAN_NORMAL ); }
#line 1600 "src/engine/jamgram.cpp"
    break;

  case 40: /* rule: SWITCH_t $@12 list _LBRACE_t $@13 cases _RBRACE_t  */
#line 219 "src/engine/jamgram.y"
                { yyval.parse = pswitch( yyvsp[-4].parse, yyvsp[-1].parse ); }
#line 1606 "src/engine/jamgram.cpp"
    break;

  case 41: /* $@14: %empty  */
#line 220 "src/engine/jamgram.y"
               { yymode( SCAN_CONDB ); }
#line 1612 "src/engine/jamgram.cpp"
    break;

  case 42: /* $@15: %empty  */
#line 220 "src/engine/jamgram.y"
                                                        { yymode( SCAN_NORMAL ); }
#line 1618 "src/engine/jamgram.cpp"
    break;

  case 43: /* rule: IF_t $@14 expr _LBRACE_t $@15 block _RBRACE_t else_opt  */
#line 221 "src/engine/jamgram.y"
                { yyval.parse = pif( yyvsp[-5].parse, yyvsp[-2].parse, yyvsp[0].parse ); }
#line 1624 "src/engine/jamgram.cpp"
    break;

  case 44: /* $@16: %empty  */
#line 222 "src/engine/jamgram.y"
                   { yymode( SCAN_PUNCT ); }
#line 1630 "src/engine/jamgram.cpp"
    break;

  case 45: /* $@17: %empty  */
#line 222 "src/engine/jamgram.y"
                                                            { yymode( SCAN_NORMAL ); }
#line 1636 "src/engine/jamgram.cpp"
    break;

  case 46: /* rule: MODULE_t $@16 list _LBRACE_t $@17 block _RBRACE_t  */
#line 223 "src/engine/jamgram.y"
                { yyval.parse = pmodule( yyvsp[-4].parse, yyvsp[-1].parse ); }
#line 1642 "src/engine/jamgram.cpp"
    break;

  case 47: /* $@18: %empty  */
#line 224 "src/engine/jamgram.y"
                  { yymode( SCAN_PUNCT ); }
#line 1648 "src/engine/jamgram.cpp"
    break;

  case 48: /* $@19: %empty  */
#line 224 "src/engine/jamgram.y"
                                                          { yymode( SCAN_NORMAL ); }
#line 1654 "src/engine/jamgram.cpp"
    break;

  case 49: /* rule: CLASS_t $@18 lol _LBRACE_t $@19 block _RBRACE_t  */
#line 225 "src/engine/jamgram.y"
                { yyval.parse = pclass( yyvsp[-4].parse, yyvsp[-1].parse ); }
#line 1660 "src/engine/jamgram.cpp"
    break;

  case 50: /* $@20: %empty  */
#line 226 "src/engine/jamgram.y"
                  { yymode( SCAN_CONDB ); }
#line 1666 "src/engine/jamgram.cpp"
    break;

  case 51: /* $@21: %empty  */
#line 226 "src/engine/jamgram.y"
                                                 { yymode( SCAN_NORMAL ); }
#line 1672 "src/engine/jamgram.cpp"
    break;

  case 52: /* rule: WHILE_t $@20 expr $@21 _LBRACE_t block _RBRACE_t  */
#line 227 "src/engine/jamgram.y"
                { yyval.parse = pwhile( yyvsp[-4].parse, yyvsp[-1].parse ); }
#line 1678 "src/engine/jamgram.cpp"
    break;

  case 53: /* $@22: %empty  */
#line 228 "src/engine/jamgram.y"
                        { yymode( SCAN_PUNCT ); }
#line 1684 "src/engine/jamgram.cpp"
    break;

  case 54: /* $@23: %empty  */
#line 228 "src/engine/jamgram.y"
                                                      { yymode( SCAN_PARAMS ); }
#line 1690 "src/engine/jamgram.cpp"
    break;

  case 55: /* $@24: %empty  */
#line 228 "src/engine/jamgram.y"
                                                                                             { yymode( SCAN_NORMAL ); }
#line 1696 "src/engine/jamgram.cpp"
    break;

  case 56: /* rule: local_opt RULE_t $@22 ARG $@23 arglist_opt $@24 rule  */
#line 229 "src/engine/jamgram.y"
                { yyval.parse = psetc( yyvsp[-4].string, yyvsp[0].parse, yyvsp[-2].parse, yyvsp[-7].number ); }
#line 1702 "src/engine/jamgram.cpp"
    break;

  case 57: /* rule: ON_t arg rule  */
#line 231 "src/engine/jamgram.y"
                { yyval.parse = pon( yyvsp[-1].parse, yyvsp[0].parse ); }
#line 1708 "src/engine/jamgram.cpp"
    break;

  case 58: /* $@25: %empty  */
#line 233 "src/engine/jamgram.y"
                { yymode( SCAN_STRING ); }
#line 1714 "src/engine/jamgram.cpp"
    break;

  case 59: /* $@26: %empty  */
#line 235 "src/engine/jamgram.y"
                { yymode( SCAN_NORMAL ); }
#line 1720 "src/engine/jamgram.cpp"
    break;

  case 60: /* rule: ACTIONS_t eflags ARG bindlist _LBRACE_t $@25 STRING $@26 _RBRACE_t  */
#line 237 "src/engine/jamgram.y"
                { yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); }
#line 1726 "src/engine/jamgram.cpp"
    break;

  case 61: /* assign: _EQUALS_t  */
#line 245 "src/engine/jamgram.y"
                { yyval.number = ASSIGN_SET; }
#line 1732 "src/engine/jamgram.cpp"
    break;

  case 62: /* assign: _PLUS_EQUALS_t  */
#line 247 "src/engine/jamgram.y"
                { yyval.number = ASSIGN_APPEND; }
#line 1738 "src/engine/jamgram.cpp"
    break;

  case 63: /* assign: _QUESTION_EQUALS_t  */
#line 249 "src/engine/jamgram.y"
                { yyval.number = ASSIGN_DEFAULT; }
#line 1744 "src/engine/jamgram.cpp"
    break;

  case 64: /* assign: DEFAULT_t _EQUALS_t  */
#line 251 "src/engine/jamgram.y"
                { yyval.number = ASSIGN_DEFAULT; }
#line 1750 "src/engine/jamgram.cpp"
    break;

  case 65: /* expr: arg  */
#line 258 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_EXISTS, yyvsp[0].parse, pnull() ); yymode( SCAN_COND ); }
#line 1756 "src/engine/jamgram.cpp"
    break;

  case 66: /* $@27: %empty  */
#line 259 "src/engine/jamgram.y"
                         { yymode( SCAN_CONDB ); }
#line 1762 "src/engine/jamgram.cpp"
    break;

  case 67: /* expr: expr _EQUALS_t $@27 expr  */
#line 260 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_EQUALS, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1768 "src/engine/jamgram.cpp"
    break;

  case 68: /* $@28: %empty  */
#line 261 "src/engine/jamgram.y"
                              { yymode( SCAN_CONDB ); }
#line 1774 "src/engine/jamgram.cpp"
    break;

  case 69: /* expr: expr _BANG_EQUALS_t $@28 expr  */
#line 262 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_NOTEQ, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1780 "src/engine/jamgram.cpp"
    break;

  case 70: /* $@29: %empty  */
#line 263 "src/engine/jamgram.y"
                         { yymode( SCAN_CONDB ); }
#line 1786 "src/engine/jamgram.cpp"
    break;

  case 71: /* expr: expr _LANGLE_t $@29 expr  */
#line 264 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_LESS, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1792 "src/engine/jamgram.cpp"
    break;

  case 72: /* $@30: %empty  */
#line 265 "src/engine/jamgram.y"
                                { yymode( SCAN_CONDB ); }
#line 1798 "src/engine/jamgram.cpp"
    break;

  case 73: /* expr: expr _LANGLE_EQUALS_t $@30 expr  */
#line 266 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_LESSEQ, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1804 "src/engine/jamgram.cpp"
    break;

  case 74: /* $@31: %empty  */
#line 267 "src/engine/jamgram.y"
                         { yymode( SCAN_CONDB ); }
#line 1810 "src/engine/jamgram.cpp"
    break;

  case 75: /* expr: expr _RANGLE_t $@31 expr  */
#line 268 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_MORE, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1816 "src/engine/jamgram.cpp"
    break;

  case 76: /* $@32: %empty  */
#line 269 "src/engine/jamgram.y"
                                { yymode( SCAN_CONDB ); }
#line 1822 "src/engine/jamgram.cpp"
    break;

  case 77: /* expr: expr _RANGLE_EQUALS_t $@32 expr  */
#line 270 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_MOREEQ, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1828 "src/engine/jamgram.cpp"
    break;

  case 78: /* $@33: %empty  */
#line 271 "src/engine/jamgram.y"
                        { yymode( SCAN_CONDB ); }
#line 1834 "src/engine/jamgram.cpp"
    break;

  case 79: /* expr: expr _AMPER_t $@33 expr  */
#line 272 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_AND, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1840 "src/engine/jamgram.cpp"
    break;

  case 80: /* $@34: %empty  */
#line 273 "src/engine/jamgram.y"
                             { yymode( SCAN_CONDB ); }
#line 1846 "src/engine/jamgram.cpp"
    break;

  case 81: /* expr: expr _AMPERAMPER_t $@34 expr  */
#line 274 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_AND, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1852 "src/engine/jamgram.cpp"
    break;

  case 82: /* $@35: %empty  */
#line 275 "src/engine/jamgram.y"
                      { yymode( SCAN_CONDB ); }
#line 1858 "src/engine/jamgram.cpp"
    break;

  case 83: /* expr: expr _BAR_t $@35 expr  */
#line 276 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_OR, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1864 "src/engine/jamgram.cpp"
    break;

  case 84: /* $@36: %empty  */
#line 277 "src/engine/jamgram.y"
                         { yymode( SCAN_CONDB ); }
#line 1870 "src/engine/jamgram.cpp"
    break;

  case 85: /* expr: expr _BARBAR_t $@36 expr  */
#line 278 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_OR, yyvsp[-3].parse, yyvsp[0].parse ); }
#line 1876 "src/engine/jamgram.cpp"
    break;

  case 86: /* $@37: %empty  */
#line 279 "src/engine/jamgram.y"
                   { yymode( SCAN_PUNCT ); }
#line 1882 "src/engine/jamgram.cpp"
    break;

  case 87: /* expr: arg IN_t $@37 list  */
#line 280 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_IN, yyvsp[-3].parse, yyvsp[0].parse ); yymode( SCAN_COND ); }
#line 1888 "src/engine/jamgram.cpp"
    break;

  case 88: /* $@38: %empty  */
#line 281 "src/engine/jamgram.y"
                  { yymode( SCAN_CONDB ); }
#line 1894 "src/engine/jamgram.cpp"
    break;

  case 89: /* expr: _BANG_t $@38 expr  */
#line 282 "src/engine/jamgram.y"
                { yyval.parse = peval( EXPR_NOT, yyvsp[0].parse, pnull() ); }
#line 1900 "src/engine/jamgram.cpp"
    break;

  case 90: /* $@39: %empty  */
#line 283 "src/engine/jamgram.y"
                    { yymode( SCAN_CONDB ); }
#line 1906 "src/engine/jamgram.cpp"
    break;

  case 91: /* expr: _LPAREN_t $@39 expr _RPAREN_t  */
#line 284 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[-1].parse; }
#line 1912 "src/engine/jamgram.cpp"
    break;

  case 92: /* cases: %empty  */
#line 295 "src/engine/jamgram.y"
                { yyval.parse = P0; }
#line 1918 "src/engine/jamgram.cpp"
    break;

  case 93: /* cases: case cases  */
#line 297 "src/engine/jamgram.y"
                { yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); }
#line 1924 "src/engine/jamgram.cpp"
    break;

  case 94: /* $@40: %empty  */
#line 300 "src/engine/jamgram.y"
                 { yymode( SCAN_CASE ); }
#line 1930 "src/engine/jamgram.cpp"
    break;

  case 95: /* $@41: %empty  */
#line 300 "src/engine/jamgram.y"
                                                       { yymode( SCAN_NORMAL ); }
#line 1936 "src/engine/jamgram.cpp"
    break;

  case 96: /* case: CASE_t $@40 ARG _COLON_t $@41 block  */
#line 301 "src/engine/jamgram.y"
                { yyval.parse = psnode( yyvsp[-3].string, yyvsp[0].parse ); }
#line 1942 "src/engine/jamgram.cpp"
    break;

  case 97: /* lol: list  */
#line 310 "src/engine/jamgram.y"
                { yyval.parse = pnode( P0, yyvsp[0].parse ); }
#line 1948 "src/engine/jamgram.cpp"
    break;

  case 98: /* lol: list _COLON_t lol  */
#line 312 "src/engine/jamgram.y"
                { yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); }
#line 1954 "src/engine/jamgram.cpp"
    break;

  case 99: /* list: listp  */
#line 322 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 1960 "src/engine/jamgram.cpp"
    break;

  case 100: /* listp: %empty  */
#line 326 "src/engine/jamgram.y"
                { yyval.parse = pnull(); }
#line 1966 "src/engine/jamgram.cpp"
    break;

  case 101: /* listp: listp arg  */
#line 328 "src/engine/jamgram.y"
                { yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); }
#line 1972 "src/engine/jamgram.cpp"
    break;

  case 102: /* arg: ARG  */
#line 332 "src/engine/jamgram.y"
                { yyval.parse = plist( yyvsp[0].string ); }
#line 1978 "src/engine/jamgram.cpp"
    break;

  case 103: /* @42: %empty  */
#line 333 "src/engine/jamgram.y"
                      { yyval.number = yymode( SCAN_CALL ); }
#line 1984 "src/engine/jamgram.cpp"
    break;

  case 104: /* arg: _LBRACKET_t @42 func _RBRACKET_t  */
#line 334 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[-1].parse; yymode( yyvsp[-2].number ); }
#line 1990 "src/engine/jamgram.cpp"
    break;

  case 105: /* $@43: %empty  */
#line 342 "src/engine/jamgram.y"
              { yymode( SCAN_PUNCT ); }
#line 1996 "src/engine/jamgram.cpp"
    break;

  case 106: /* func: ARG $@43 lol  */
#line 343 "src/engine/jamgram.y"
                { yyval.parse = prule( yyvsp[-2].string, yyvsp[0].parse ); }
#line 2002 "src/engine/jamgram.cpp"
    break;

  case 107: /* $@44: %empty  */
#line 344 "src/engine/jamgram.y"
                       { yymode( SCAN_PUNCT ); }
#line 2008 "src/engine/jamgram.cpp"
    break;

  case 108: /* func: ON_t arg ARG $@44 lol  */
#line 345 "src/engine/jamgram.y"
                { yyval.parse = pon( yyvsp[-3].parse, prule( yyvsp[-2].string, yyvsp[0].parse ) ); }
#line 2014 "src/engine/jamgram.cpp"
    break;

  case 109: /* $@45: %empty  */
#line 346 "src/engine/jamgram.y"
                            { yymode( SCAN_PUNCT ); }
#line 2020 "src/engine/jamgram.cpp"
    break;

  case 110: /* func: ON_t arg RETURN_t $@45 list  */
#line 347 "src/engine/jamgram.y"
                { yyval.parse = pon( yyvsp[-3].parse, yyvsp[0].parse ); }
#line 2026 "src/engine/jamgram.cpp"
    break;

  case 111: /* eflags: %empty  */
#line 357 "src/engine/jamgram.y"
                { yyval.number = 0; }
#line 2032 "src/engine/jamgram.cpp"
    break;

  case 112: /* eflags: eflags eflag  */
#line 359 "src/engine/jamgram.y"
                { yyval.number = yyvsp[-1].number | yyvsp[0].number; }
#line 2038 "src/engine/jamgram.cpp"
    break;

  case 113: /* eflag: UPDATED_t  */
#line 363 "src/engine/jamgram.y"
                { yyval.number = EXEC_UPDATED; }
#line 2044 "src/engine/jamgram.cpp"
    break;

  case 114: /* eflag: TOGETHER_t  */
#line 365 "src/engine/jamgram.y"
                { yyval.number = EXEC_TOGETHER; }
#line 2050 "src/engine/jamgram.cpp"
    break;

  case 115: /* eflag: IGNORE_t  */
#line 367 "src/engine/jamgram.y"
                { yyval.number = EXEC_IGNORE; }
#line 2056 "src/engine/jamgram.cpp"
    break;

  case 116: /* eflag: QUIETLY_t  */
#line 369 "src/engine/jamgram.y"
                { yyval.number = EXEC_QUIETLY; }
#line 2062 "src/engine/jamgram.cpp"
    break;

  case 117: /* eflag: PIECEMEAL_t  */
#line 371 "src/engine/jamgram.y"
                { yyval.number = EXEC_PIECEMEAL; }
#line 2068 "src/engine/jamgram.cpp"
    break;

  case 118: /* eflag: EXISTING_t  */
#line 373 "src/engine/jamgram.y"
                { yyval.number = EXEC_EXISTING; }
#line 2074 "src/engine/jamgram.cpp"
    break;

  case 119: /* bindlist: %empty  */
#line 382 "src/engine/jamgram.y"
                { yyval.parse = pnull(); }
#line 2080 "src/engine/jamgram.cpp"
    break;

  case 120: /* $@46: %empty  */
#line 383 "src/engine/jamgram.y"
                 { yymode( SCAN_PUNCT ); }
#line 2086 "src/engine/jamgram.cpp"
    break;

  case 121: /* bindlist: BIND_t $@46 list  */
#line 384 "src/engine/jamgram.y"
                { yyval.parse = yyvsp[0].parse; }
#line 2092 "src/engine/jamgram.cpp"
    break;


#line 2096 "src/engine/jamgram.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

