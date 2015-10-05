
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTITY = 258,
     DEFAULT = 259,
     HOSTNAME = 260,
     USERNAME = 261,
     PASSWORD = 262,
     STARTTLS = 263,
     CERTIFICATE_PASSPHRASE = 264,
     PRECONNECT = 265,
     POSTCONNECT = 266,
     MDA = 267,
     QUALIFYDOMAIN = 268,
     HELO = 269,
     FORCE = 270,
     SENDER = 271,
     MSGID = 272,
     REVERSE_PATH = 273,
     FORCE_MDA = 274,
     MAP = 275,
     DISABLED = 276,
     ENABLED = 277,
     REQUIRED = 278,
     STRING = 279,
     NUMBER = 280
   };
#endif
/* Tokens.  */
#define IDENTITY 258
#define DEFAULT 259
#define HOSTNAME 260
#define USERNAME 261
#define PASSWORD 262
#define STARTTLS 263
#define CERTIFICATE_PASSPHRASE 264
#define PRECONNECT 265
#define POSTCONNECT 266
#define MDA 267
#define QUALIFYDOMAIN 268
#define HELO 269
#define FORCE 270
#define SENDER 271
#define MSGID 272
#define REVERSE_PATH 273
#define FORCE_MDA 274
#define MAP 275
#define DISABLED 276
#define ENABLED 277
#define REQUIRED 278
#define STRING 279
#define NUMBER 280




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 62 "parser.y"

    int number;
    char *sval;



/* Line 1676 of yacc.c  */
#line 109 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


