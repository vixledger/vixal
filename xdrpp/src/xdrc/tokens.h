/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_USERS_LIUWEI_WORKS_CPLUSPLUS_BLOCKCHAIN_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED
# define YY_YY_USERS_LIUWEI_WORKS_CPLUSPLUS_BLOCKCHAIN_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED
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

#endif /* !YY_YY_USERS_LIUWEI_WORKS_CPLUSPLUS_BLOCKCHAIN_VIXAL_XDRPP_SRC_XDRC_TOKENS_H_INCLUDED  */
