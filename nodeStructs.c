#include "nodeStructs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASHSIZE 50

//try passing line number with assignment operators

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Code for hash table ////////////////

//This is only a partial implementation of a hash table; I did not add in currently unused functionality

//individual entry struct
struct HashEntry {
	char type;
	char* name;
	int k;
};

//the symbol table that will be used for tracking variables
struct HashEntry* symbolTable[HASHSIZE];
int marker = 0;
int error = 0;

//Stops compilation if there will be overflow in the symbol table
void checkIndex(int k) {
	if (k >= HASHSIZE) {
		printf("ERROR - unexpected amount of variables used. Edit HASHSIZE in nodeStructs.c\n");
		exit(1);
	}
}

//Adds entry to symbol table
void addEntry(char* nm, char t) {
	struct HashEntry *entry = (struct HashEntry*) malloc(sizeof(struct HashEntry));
	entry->name = nm; 
	entry->type = t;
	entry->k = marker;

	marker++;
	checkIndex(entry->k);
	symbolTable[entry->k] = entry;
}
//There was currently no need to remove entries from the table, so such a function was not included. Would ned to add if there were different scopes.


//check if entry has correct type (also checks if properly declared)
int checkEntryType(char* nm, char t, int ln) {
	int id = 0;
	while(symbolTable[id] != NULL && id < HASHSIZE) {
		if(strcmp(symbolTable[id]->name, nm) == 0) {
			char check = symbolTable[id]->type;
			if(check == t) {
				return 0;
			}
			else {
				//throw error type mismatch
				throwError(2, nm, t, ln);
				return 2;
			}
		}
		id++;
	}
	//throw error not declared
	throwError(1, nm, t, ln);
	return 1;
}

//throws error if variable already exists
void checkNew(char* nm, int ln) {
	int id = 0;
	while(symbolTable[id] != NULL && id < HASHSIZE) {
		if(strcmp(symbolTable[id]->name, nm) == 0) {
			//throw error already declared
			throwError(3, nm, 'n', ln);
		}
		id++;
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////Code for checking types////////////////////////

//gets type of identifier
char getIDType(char* nm, int ln) {
	int id = 0;
	while(symbolTable[id] != NULL && id < HASHSIZE) {
		if(strcmp(symbolTable[id]->name, nm) == 0) {
			return symbolTable[id]->type;
		}
		id++;
	}
	//variable not in symbol table
	throwError(1, nm, 'n', ln);
	return 'n';
}

//gets type of expression
char retrieveEType(EXPR* expr) {
	if(expr->t == 2) {
		return 'b';
	}
	else {
		return retrieveSType(expr->sxptr1);
	}
}

//gets type of simple expression
char retrieveSType(SXPR* sxpr) {
	if(sxpr->t == 1) {
		return 'i';
	}
	else {
		return retrieveTType(sxpr->trptr1);
	}
}

//gets type of term
char retrieveTType(TRM* trm) {
	if(trm->t == 1) {
		return 'i';
	}
	else {
		return retrieveFType(trm->fcptr1);
	}
}

//gets type of factor
char retrieveFType(FCTR* fctr) {
	if(fctr->t == 1) {
		return getIDType(fctr->ident->ident, fctr->ident->ln);
	}
	else if(fctr->t == 2) {
		return 'i';
	}
	else if(fctr->t == 3) {
		return 'b';
	}
	else {
		//need to recursively find expression's type
		return retrieveEType(fctr->exptr);
	}
}

//check if number accepted range
//returns -1 for below, 0 for good, 1 for exceeds
int checkNum(char* n) {
	char* nullp;
	long long int num = strtoll(n, &nullp, 10);
	if(num < 0) {
		return -1;
	}
	else if(num > 2147483647) {
		return 1;
	}
	else {
		return 0;
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Code for generating code ///////////

//start of generation called with PROG
void generateCode(PROG* prog) {

	//generate code for declarations and statement sequence
	char* declOut = genDecl(0, prog->dptr);
	char* stsqOut = genStsq(1, prog->sptr);

	//format output to print if no errors
	char* output = malloc(strlen("#include <stdio.h>\n#include <stdbool.h>\n\nint main() {\n\n\treturn 0;\n}\n") + strlen(stsqOut) + strlen(declOut) + 1);
	strcpy(output, "#include <stdio.h>\n#include <stdbool.h>\n\n");
    strcat(output, declOut);
	free(declOut);
	strcat(output, "int main() {\n");
	strcat(output, stsqOut);
	free(stsqOut);
	strcat(output, "\n\treturn 0;\n}\n");
	if(error == 0) {
		printf("%s", output);
	}
	else {
		exit(1);
	}
}

//runs for every declaration rule
char* genDecl(int tabLvl, DECLS * decls){
	if(decls != (DECLS*)NULL) {
		//make sure not previously declared
		checkNew(decls->ident->ident, decls->ident->ln);
		//add to symbol table
		addEntry(decls->ident->ident, decls->tptr->t);

		//generate code for other declarations
		char* out2 = genDecl(0, decls->dptr);

		//create output to return since no errors
		char* out = malloc(strlen("bool  = false;\n") + strlen(decls->ident->ident) + strlen("\t")*tabLvl + strlen(out2) + 1);
		if(tabLvl > 0) {
			strcpy(out, "\t");
			int i;
			for(i = 1; i < tabLvl; i++) {
				strcat(out, "\t");
			}
		}
		else {
			strcpy(out, "");
		}
		if(decls->tptr->t == 'i') {
			strcat(out, "int ");
			strcat(out, decls->ident->ident);
			strcat(out, " = 0;\n");
		}
		else if(decls->tptr->t == 'b') {
			strcat(out, "bool ");
			strcat(out, decls->ident->ident);
			strcat(out, " = false;\n");
		}
		strcat(out, out2);
		free(out2);
		return out;
	}
	else { //return empty if NULL
		char* out = malloc(strlen("\n") + 1);
		strcpy(out, "\n");
		return out;
	}
}

//runs for every statement sequence rule
char* genStsq(int tabLvl, STSQ * stsq) {
	if(stsq != (STSQ*)NULL) {
		//generate code for statements
		char* out1 = genSt(tabLvl, stsq->stptr);
		char* out2 = genStsq(tabLvl, stsq->sqptr);

		//create output to return since no errors
		char* out = malloc(strlen(out1) + strlen(out2) + tabLvl + 1);
		if(tabLvl > 0) {
			strcpy(out, "\t");
			int i;
			for(i = 1; i < tabLvl; i++) {
				strcat(out, "\t");
			}
		}
		else {
			strcpy(out, "");
		}

		strcat(out, out1);
		strcat(out, out2);
		free(out1);
		free(out2);
		return out;
	}
	else { //return empty if NULL
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every statement rule
char* genSt(int tabLvl, STMT * stmt) {
	//determine which type of statement code to generate
	if(stmt->t == 1) {
		return genAssn(tabLvl, stmt->asptr);
	}
	else if(stmt->t == 2) {
		return genIf(tabLvl, stmt->ifptr);
	}
	else if(stmt->t == 3) {
		return genWl(tabLvl, stmt->wlptr);
	}
	else if(stmt->t == 4) { 
		return genWi(tabLvl, stmt->wiptr);
	}
	else { //SHOULD NEVER BE CALLED, BUT HERE JUST IN CASE
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every assignment statement rule
char* genAssn(int tabLvl, ASN * asn) {
	char* out;

	//create output to return depending on type
	if(asn->t == 2){
		//check for type mismatch with readInt
		checkEntryType(asn->ident->ident, 'i', asn->ident->ln);

		out = malloc(strlen("scanf(\"%d\", &);\n") + strlen(asn->ident->ident) + 1);
		strcpy(out, "scanf(\"%d\", &");
		strcat(out, asn->ident->ident);
		strcat(out, ");\n");
	}
	else if(asn->t == 1){
		//check for type mismatch with expression
		char c = retrieveEType(asn->exptr);
		checkEntryType(asn->ident->ident, c, asn->ident->ln);
		
		char* out1 = genExpr(tabLvl, asn->exptr);
		out = malloc(strlen(" = ;\n") + strlen(asn->ident->ident) + strlen(out1) + 1);
		strcpy(out, asn->ident->ident);
		strcat(out, " = ");
		strcat(out, out1);
		free(out1);
		strcat(out, ";\n");
	}
	return out;
}

//runs for every expression rule
char* genExpr(int tabLvl, EXPR* expr) {
	if(expr->t == 1) { //just simple expression
		return genSx(tabLvl, expr->sxptr1);
	}
	else if(expr->t == 2) { 
		//check for wrong operand types
		char c = retrieveSType(expr->sxptr1);
		if(c != 'i') {
			throwError(4, expr->op4ptr->op4, 'i', expr->op4ptr->ln);
		}
		c = retrieveSType(expr->sxptr2);
		if(c != 'i') {
			throwError(5, expr->op4ptr->op4, 'i', expr->op4ptr->ln);
		}

		char* out1 = genSx(tabLvl, expr->sxptr1);
		char* out2 = genSx(tabLvl, expr->sxptr2);

		//create output to return since no errors
		char* out = malloc(strlen(out1) + strlen(out2) + strlen(expr->op4ptr->op4) + 3);
		strcpy(out, out1);
		strcat(out, " ");
		strcat(out, expr->op4ptr->op4);
		strcat(out, " ");
		strcat(out, out2);
		free(out1);
		free(out2);

		return out;
	}
	else { //SHOULD NEVER RUN
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every simple expression rule
char* genSx(int tabLvl, SXPR* sxpr) {
	if(sxpr->t == 1) {
		//check for wrong operand types
		char c = retrieveTType(sxpr->trptr1);
		if(c != 'i') {
			throwError(4, sxpr->op3ptr->op3, 'i', sxpr->op3ptr->ln);
		}
		c = retrieveTType(sxpr->trptr2);
		if(c != 'i') {
			throwError(5, sxpr->op3ptr->op3, 'i', sxpr->op3ptr->ln);
		}

		char* out1 = genTrm(tabLvl, sxpr->trptr1);
		char* out2 = genTrm(tabLvl, sxpr->trptr2);

		//create output to return since no errors
		char* out = malloc(strlen(out1) + strlen(out2) + strlen(sxpr->op3ptr->op3) + 5);
		strcpy(out, "(");
		strcat(out, out1);
		strcat(out, " ");
		strcat(out, sxpr->op3ptr->op3);
		strcat(out, " ");
		strcat(out, out2);
		strcat(out, ")");
		free(out1);
		free(out2);

		return out;
	}
	else if(sxpr->t == 2) { //just term
		return genTrm(tabLvl, sxpr->trptr1);
	}
	else { //NEVER RUNS
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every term rule
char* genTrm(int tabLvl, TRM* trm) {
	if(trm->t == 1) {
		//cehck for wrong operands
		char c = retrieveFType(trm->fcptr1);
		if(c != 'i') {
			throwError(4, trm->op2ptr->op2, 'i', trm->op2ptr->ln);
		}
		c = retrieveFType(trm->fcptr2);
		if(c != 'i') {
			throwError(5, trm->op2ptr->op2, 'i', trm->op2ptr->ln);
		}

		char* out1 = genFctr(tabLvl, trm->fcptr1);
		char* out2 = genFctr(tabLvl, trm->fcptr2);

		//create output to return since no errors
		char* out = malloc(strlen(out1) + strlen(out2) + 6);
		strcpy(out, "(");
		strcat(out, out1);
		strcat(out, " ");

		if(strcmp(trm->op2ptr->op2, "div")) {
			strcat(out, "/");
		}
		else if(strcmp(trm->op2ptr->op2, "mod")) {
			strcat(out, "%");
		}
		else {
			strcat(out, trm->op2ptr->op2);
		}
		strcat(out, " ");
		strcat(out, out2);
		strcat(out, ")");
		free(out1);
		free(out2);

		return out;
	}
	else if(trm->t == 2) { //just factor
		return genFctr(tabLvl, trm->fcptr1);
	}
	else { //NEVER RUNS
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every factor rule
char* genFctr(int tabLvl, FCTR* fctr) {
	if(fctr->t == 1) { //just variable
		char* out = fctr->ident->ident;
		return out;
	}
	if(fctr->t == 2) { //just a number
		//check if number is in acceptable range
		int c = checkNum(fctr->num->n);
		if(c == -1) { //Actually can't be called since the regex for num doesn't include '-' for negatives
			throwError(10, fctr->num->n, 'i', fctr->num->ln);
		}
		else if(c == 1) { //over 2147483647
			throwError(9, fctr->num->n, 'i', fctr->num->ln);
		}

		//no errors, return output
		char* out = fctr->num->n;
		return out;
	}
	if(fctr->t == 3) { //just a boolean
		char* out = fctr->boollit;
		return out;
	}
	if(fctr->t == 4) { //just an expression
		char*out1 = genExpr(tabLvl, fctr->exptr);
		
		//create output to return since no errors
		char* out = malloc(strlen(out1) + 3);
		strcpy(out, "(");
		strcat(out, out1);
		strcat(out, ")");
		free(out1);
		return out;
	}
	else { //NEVER RUNS
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every if statement rule
char* genIf(int tabLvl, IFST* ifst) {
	//check for boolean
	char c = retrieveEType(ifst->exptr);
	if(c == 'i'){
		throwError(8, "", 'i', ifst->ln);
	}

	char* out1 = genExpr(tabLvl, ifst->exptr);
	char* out2 = genStsq(tabLvl + 1, ifst->sqptr);
	char* out3 = genElse(tabLvl, ifst->elptr);

	//create output to return since no errors
	char* out = malloc(strlen(out1) + strlen(out2) + strlen(out3) + strlen("if() {\n}\n\n") + 1 + tabLvl);

	strcpy(out, "if(");
	strcat(out, out1);
	strcat(out, ") {\n");
	strcat(out, out2);
	int i;
	for(i = 0; i < tabLvl; i++) {
		strcat(out, "\t");
	}
	strcat(out, "}\n");
	strcat(out, out3);
	strcat(out, "\n");
	free(out1);
	free(out2);
	free(out3);
	return out;
}

//runs for every else clause
char* genElse(int tabLvl, ELCL* elcl) {
	if(elcl != (ELCL*)NULL) {
		char* out1 = genStsq(tabLvl + 1, elcl->stptr);

		//create output to return since no errors
		char* out = malloc(strlen(out1) + strlen("else {\n}\n") + tabLvl*2 + 1);

		if(tabLvl > 0) {
			strcpy(out, "\t");
			int i;
			for(i = 1; i < tabLvl; i++) {
				strcat(out, "\t");
			}
		}
		else {
			strcpy(out, "");
		}
		strcat(out, "else {\n");
		strcat(out, out1);
		free(out1);

		int i;
		for(i = 0; i < tabLvl; i++) {
			strcat(out, "\t");
		}
		strcat(out, "}\n");

		return out;
	}
	else { //return empty if no else clause
		char* out = malloc(strlen("") + 1);
		strcpy(out, "");
		return out;
	}
}

//runs for every while loop rule
char* genWl(int tabLvl, WLST* wlst) {
	//check for boolean
	char c = retrieveEType(wlst->exptr);
	if(c == 'i'){
		throwError(7, "", 'i', wlst->ln);
	}

	char* out1 = genExpr(tabLvl, wlst->exptr);
	char* out2 = genStsq(tabLvl + 1, wlst->sqptr);

	//create output to return since no errors
	char* out = malloc(strlen(out1) + strlen(out2) + strlen("while () {\n}\n\n") + tabLvl + 1);

	strcpy(out, "while (");
	strcat(out, out1);
	free(out1);
	strcat(out, ") {\n");
	strcat(out, out2);
	free(out2);

	int i;
	for(i = 0; i < tabLvl; i++) {
		strcat(out, "\t");
	}
	strcat(out, "}\n\n");

	return out;
}

//runs for every writeInt rule
char* genWi(int tabLvl, WINT* wint) {
	//check the value to write is an integer
	char c = retrieveEType(wint->exptr);
	if(c != 'i') {
		throwError(6, "", 'i', wint->ln);
	}

	char* out1 = genExpr(tabLvl, wint->exptr);

	//create output to return since no errors
	char* out = malloc(strlen("printf(\"%d\", );\n") + strlen(out1) + 1);
	strcpy(out, "printf(\"%d\", ");
	strcat(out, out1);
	strcat(out, ");\n");
	return out;
}



///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////error handler ////////////////////////

void throwError(int e, char* ident, char t, int l) {
	//not declared
	if(e == 1) {
		printf("ERROR - Variable %s was not declared. Attempted use on line %d.\n", ident, l);
	}
	//type mismatch
	if(e == 2) {
		printf("ERROR - Type mismatch on line %d. Expected ", l);
		if(t == 'i') {
			printf("INTEGER variable.\n");
		}
		else if(t == 'b') {
			printf("BOOLEAN variable.\n");
		}
	}
	//already declared
	if(e == 3) {
		printf("ERROR - %s has already been declared. 2nd declaration on line %d.\n", ident, l);
	}
	//Left operand mismatch
	if(e == 4) {
		printf("ERROR - Expected left operand of %s on line %d to be ", ident, l);
		if(t == 'i'){
			printf("INTEGER, evaluates to BOOLEAN.\n");
		}
		else {
			printf("BOOLEAN, evaluates to INTEGER.\n");
		}
	}
	//Right operand mismatch
	if(e == 5) {
		printf("ERROR - Expected right operand of %s on line %d to be ", ident, l);
		if(t == 'i'){
			printf("INTEGER, evaluates to BOOLEAN.\n");
		}
		else {
			printf("BOOLEAN, evaluates to INTEGER.\n");
		}
	}
	if(e == 6) {
		printf("ERROR - Attempted to write non INTEGER value with WRITEINT on line %d.\n", l);
	}
	if(e == 7) {
		printf("ERROR - WHILE loop expression evaluates to non-BOOLEAN; line %d.\n", l);
	}
	if(e == 8) {
		printf("ERROR - IF statement expression evaluates to non-BOOLEAN; line %d.\n", l);
	}
	if(e == 9) {
		printf("ERROR - INTEGER exceeds accepted value; line %d.\n", l);
	}
	if(e == 10) {
		printf("ERROR - INTEGER assigned below accepted range; line %d.\n", l);
	}
	error = 1;
}