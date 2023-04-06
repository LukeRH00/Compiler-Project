/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_PROJECT1_TAB_H_INCLUDED
# define YY_YY_PROJECT1_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     BOOLLIT = 259,
     IDENT = 260,
     LP = 261,
     RP = 262,
     ASGN = 263,
     SC = 264,
     OP4 = 265,
     OP3 = 266,
     OP2 = 267,
     IF = 268,
     THEN = 269,
     ELSE = 270,
     BEGN = 271,
     END = 272,
     WHILE = 273,
     DO = 274,
     PROGRAM = 275,
     VAR = 276,
     AS = 277,
     INT = 278,
     BOOL = 279,
     WRITEINT = 280,
     READINT = 281
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 11 "project1.y"

        char    *sval;
	struct IDEN* ident;
	struct OP4S* op4ptr;
	struct OP3S* op3ptr;
	struct OP2S* op2ptr;
	int ival;
        struct NUMB* num;

/* Line 2058 of yacc.c  */
#line 22 "project1.y"

	struct PROG *pptr;
	struct DECLS *dptr;
	struct TYP *tptr;
	struct STSQ *sqptr;
	struct STMT *stptr;
	struct ASN *asptr;
	struct IFST *ifptr;
	struct ELCL *elptr;
	struct WLST *wlptr;
	struct WINT *wiptr;
	struct EXPR *xpptr;
	struct SXPR *sxptr;
	struct TRM *tmptr;
	struct FCTR *fcptr;


/* Line 2058 of yacc.c  */
#line 112 "project1.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PROJECT1_TAB_H_INCLUDED  */
