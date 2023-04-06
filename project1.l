%{
#include "project1.tab.h"
int lnNum = 1;
%}

white	[ \t]+
newln	[\n]
num	[1-9][0-9]*|0
boollit	"true"|"false"
ident	[A-Z][A-Z0-9]*

%%
{white}	{/*ignore*/}
{newln}	{lnNum++;}
{num}	{
		yylval.num = malloc(sizeof(NUMB));
		yylval.num->n = strdup(yytext);
		yylval.num->ln = lnNum;
		return(NUM);
}

{boollit}	{
		yylval.sval = strdup(yytext);
		return(BOOLLIT);
}

{ident}	{
		yylval.ident = malloc(sizeof(IDEN));
		yylval.ident->ident = strdup(yytext);
		yylval.ident->ln = lnNum;
		return(IDENT);
}

"("	return(LP);
")"	return(RP);
":="	return(ASGN);
";"	return(SC);
"*"|"div"|"mod"	{
		yylval.op2ptr = malloc(sizeof(OP2S));
		yylval.op2ptr->op2 = strdup(yytext);
		yylval.op2ptr->ln = lnNum;
		return(OP2);
}

"+"|"-"	{
		yylval.op3ptr = malloc(sizeof(OP3S));
		yylval.op3ptr->op3 = strdup(yytext);
		yylval.op3ptr->ln = lnNum;
		return(OP3);
}

"="|"!="|"<"|">"|"<="|">="	{
		yylval.op4ptr = malloc(sizeof(OP4S));
		yylval.op4ptr->op4 = strdup(yytext);
		yylval.op4ptr->ln = lnNum;
		return(OP4);
}

"if"	{yylval.ival = lnNum; return(IF);}
"then"	return(THEN);
"else"	return(ELSE);
"begin"	return(BEGN);
"end"	return(END);
"while"	{yylval.ival = lnNum; return(WHILE);}
"do"	return(DO);
"program"	return(PROGRAM);
"var"	return(VAR);
"as"	return(AS);
"int"	return(INT);
"bool"	return(BOOL);

"writeInt"	{yylval.ival = lnNum; return(WRITEINT);}
"readInt"	return(READINT);
