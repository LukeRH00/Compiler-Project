%{
#include <stdio.h>

#include "nodeStructs.c"
int yylex(void);
int yyerror(char *);
%}

// Symbols.
%union
{
        char    *sval;
	struct IDEN* ident;
	struct OP4S* op4ptr;
	struct OP3S* op3ptr;
	struct OP2S* op2ptr;
	int ival;
        struct NUMB* num;
};

%union
{
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
};


%token <num> NUM
%token <sval> BOOLLIT
%token <ident> IDENT
%token LP
%token RP
%token ASGN
%token SC
%token <op4ptr> OP4
%token <op3ptr> OP3
%token <op2ptr> OP2
%token <ival> IF
%token THEN
%token ELSE
%token BEGN
%token END
%token <ival> WHILE
%token DO
%token PROGRAM
%token VAR
%token AS
%token INT
%token BOOL
%token <ival> WRITEINT
%token READINT

%type<pptr> Program;
%type<dptr> Declarations;
%type<tptr> Type;
%type<sqptr> StatementSequence;
%type<stptr> Statement;
%type<asptr> Assignment;
%type<ifptr> IfStatement;
%type<elptr> ElseClause;
%type<wlptr> WhileStatement;
%type<wiptr> WriteInt;
%type<xpptr> Expression;
%type<sxptr> SimpleExpression;
%type<tmptr> Term;
%type<fcptr> Factor;


%start Program
%%

Program:
        PROGRAM Declarations BEGN StatementSequence END {PROG *ptr = malloc(sizeof(PROG)); ptr->dptr = $2; ptr->sptr = $4; generateCode(ptr);}
        ;

Declarations:
        /* empty */ {$$ = (DECLS*)NULL;}
        | VAR IDENT AS Type SC Declarations {DECLS *ptr = malloc(sizeof(DECLS)); ptr->ident = $2; ptr->tptr = $4; ptr->dptr = $6; $$ = ptr;}
        ;

Type:
        INT {TYP *ptr = malloc(sizeof(TYP)); ptr->t = 'i'; $$ = ptr;}
        | BOOL {TYP *ptr = malloc(sizeof(TYP)); ptr->t = 'b'; $$ = ptr;}
        ;

StatementSequence:
        /* empty */ {$$ = (STSQ*)NULL;}
        | Statement SC StatementSequence {STSQ *ptr = malloc(sizeof(STSQ)); ptr->stptr = $1; ptr->sqptr = $3; $$ = ptr;}
        ;

Statement:
        Assignment {STMT *ptr = malloc(sizeof(STMT)); ptr->asptr = $1; ptr->t = 1; $$ = ptr;}
        | IfStatement {STMT *ptr = malloc(sizeof(STMT)); ptr->ifptr = $1; ptr->t = 2; $$ = ptr;}
        | WhileStatement {STMT *ptr = malloc(sizeof(STMT)); ptr->wlptr = $1; ptr->t = 3; $$ = ptr;}
        | WriteInt {STMT *ptr = malloc(sizeof(STMT)); ptr->wiptr = $1; ptr->t = 4; $$ = ptr;}
        ;

Assignment:
        IDENT ASGN Expression {ASN *ptr = malloc(sizeof(ASN)); ptr->ident = $1; ptr->exptr = $3; ptr->t = 1; $$ = ptr;}
        | IDENT ASGN READINT {ASN *ptr = malloc(sizeof(ASN)); ptr->ident = $1; ptr->exptr = (EXPR*)NULL; ptr->t = 2; $$ = ptr;}
        ;

IfStatement:
        IF Expression THEN StatementSequence ElseClause END {IFST *ptr = malloc(sizeof(IFST)); ptr->exptr = $2; ptr->sqptr = $4; ptr->elptr = $5; ptr->ln = $1; $$ = ptr;}
        ;

ElseClause:
        /* empty */ {$$ = (ELCL*)NULL;}
        | ELSE StatementSequence {ELCL *ptr = malloc(sizeof(ELCL)); ptr->stptr = $2; $$ = ptr;}
        ;

WhileStatement:
        WHILE Expression DO StatementSequence END {WLST *ptr = malloc(sizeof(WLST)); ptr->exptr = $2; ptr->sqptr = $4; ptr->ln = $1; $$ = ptr;}
        ;

WriteInt:
        WRITEINT Expression {WINT *ptr = malloc(sizeof(WINT)); ptr->exptr = $2; ptr->ln = $1; $$ = ptr;}
        ;

Expression:
        SimpleExpression {EXPR *ptr = malloc(sizeof(EXPR)); ptr->sxptr1 = $1; ptr->op4ptr = (OP4S*)NULL; ptr->sxptr2 = (SXPR*)NULL; ptr->t = 1; $$ = ptr;}
        | SimpleExpression OP4 SimpleExpression {EXPR *ptr = malloc(sizeof(EXPR)); ptr->sxptr1 = $1; ptr->op4ptr = $2; ptr->sxptr2 = $3; ptr->t = 2; $$ = ptr;}
        ;

SimpleExpression:
        Term OP3 Term {SXPR *ptr = malloc(sizeof(SXPR)); ptr->trptr1 = $1; ptr->op3ptr = $2; ptr->trptr2 = $3; ptr->t = 1; $$ = ptr;}
        | Term {SXPR *ptr = malloc(sizeof(SXPR)); ptr->trptr1 = $1; ptr->op3ptr = (OP3S*)NULL; ptr->trptr2 = (TRM*)NULL; ptr->t = 2; $$ = ptr;}
        ;

Term:
        Factor OP2 Factor {TRM *ptr = malloc(sizeof(TRM)); ptr->fcptr1 = $1; ptr->op2ptr = $2; ptr->fcptr2 = $3; ptr->t = 1; $$ = ptr;}
        | Factor {TRM *ptr = malloc(sizeof(TRM)); ptr->fcptr1 = $1; ptr->op2ptr = (OP2S*)NULL; ptr->fcptr2 = (FCTR*)NULL; ptr->t = 2; $$ = ptr;}
        ;

Factor:
        IDENT {FCTR *ptr = malloc(sizeof(FCTR)); ptr->ident = $1; ptr->t = 1; $$ = ptr;}
        | NUM {FCTR *ptr = malloc(sizeof(FCTR)); ptr->num = $1; ptr->t = 2; $$ = ptr;}
        | BOOLLIT {FCTR *ptr = malloc(sizeof(FCTR)); ptr->boollit = $1; ptr->t = 3; $$ = ptr;}
        | LP Expression RP {FCTR *ptr = malloc(sizeof(FCTR)); ptr->exptr = $2; ptr->t = 4; $$ = ptr;}
        ;
%%
int yyerror(char *s) {
  printf("yyerror : %s\n",s);
}
int main(void) {
  yyparse();
}

#include "lex.yy.c"
