struct STSQ; 
struct EXPR;

typedef struct IDEN {
    char* ident;
    int ln;
}IDEN;

typedef struct OP4S {
    char* op4;
    int ln;
}OP4S;
typedef struct OP3S {
    char* op3;
    int ln;
}OP3S;
typedef struct OP2S {
    char* op2;
    int ln;
}OP2S;

typedef struct NUMB {
    char* n;
    int ln;
}NUMB;

typedef struct FCTR {
    union {
        struct IDEN *ident;
        struct NUMB *num;
        char *boollit;
        struct EXPR *exptr;
    };
    int t;
}FCTR;

typedef struct TRM {
    struct FCTR *fcptr1;
    struct OP2S *op2ptr;
    struct FCTR *fcptr2;
    int t;
}TRM;

typedef struct SXPR {
    struct TRM *trptr1;
    struct OP3S*op3ptr;
    struct TRM *trptr2;
    int t;
}SXPR;

typedef struct EXPR {
    struct SXPR *sxptr1;
    struct OP4S*op4ptr;
    struct SXPR *sxptr2;
    int t;
}EXPR;

typedef struct WINT {
    struct EXPR *exptr;
    int ln;
}WINT;

typedef struct WLST {
    struct EXPR *exptr;
    struct STSQ *sqptr;
    int ln;
}WLST;

typedef struct ELCL {
    struct STSQ *stptr;
}ELCL;

typedef struct IFST {
    struct EXPR *exptr;
    struct STSQ *sqptr;
    struct ELCL *elptr;
    int ln;
}IFST;

typedef struct ASN {
    int t;
    struct IDEN *ident;
    struct EXPR *exptr;
}ASN;

typedef struct STMT {
    union {
        struct ASN *asptr;
        struct IFST *ifptr;
        struct WLST *wlptr;
        struct WINT *wiptr;
    };
    int t;
}STMT;

typedef struct TYP {
    char t;
}TYP;

typedef struct STSQ {
    struct STMT * stptr;
    struct STSQ * sqptr;
}STSQ;

typedef struct DECLS {
    struct IDEN *ident;
    struct TYP * tptr;
    struct DECLS *dptr;
}DECLS;

typedef struct PROG {
    struct DECLS *dptr;
    struct STSQ *sptr;
}PROG;




void generateCode(PROG * prog);
char* genDecl(int tabLvl, DECLS * decls);
char* genStsq(int tabLvl, STSQ * stsq);
char* genSt(int tabLvl, STMT * stmt);
char* genAssn(int tabLvl, ASN * asn);
char* genExpr(int tabLvl, EXPR* expr);
char* genIf(int tabLvl, IFST* ifst);
char* genWl(int tabLvl, WLST* wlst);
char* genWi(int tabLvl, WINT* wint);
char* genElse(int tabLvl, ELCL* elcl);
char* genSx(int tabLvl, SXPR* sxpr);
char* genTrm(int tabLvl, TRM* trm);
char* genFctr(int tabLvl, FCTR* fctr);

void throwError(int e, char* ident, char t, int l);


char retrieveEType(EXPR* expr);
char retrieveSType(SXPR* sxpr);
char retrieveTType(TRM* trm);
char retrieveFType(FCTR* fctr);