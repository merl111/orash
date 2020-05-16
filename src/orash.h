#include <stdio.h>
#include "linenoise.h"
#include "ocilib/include/ocilib.h"

#ifndef __ORASH_H
#define __ORASH_H
/* --------------------------------------------------------------------------------------------- *
 * STRING PRINTING
 * --------------------------------------------------------------------------------------------- */

#define print_frmt(f, x)    printf(f, x)
#define print_text(x)       printf(x)

#if defined(OCI_CHARSET_WIDE)
    #ifdef _WINDOWS
        #define print_ostr(x)   wprintf(OTEXT("%s"), x)
#else
        #define print_ostr(x)   printf("%ls", x)
#endif
#else
  #define print_ostr(x)   printf(OTEXT("%s"), x)
#endif

typedef OCI_Statement* (*stmt_c_func_p)(OCI_Connection*);

typedef int (*stmt_e_func_p)(OCI_Statement*, const otext* sql);
typedef int (*stmt_f_func_p)(OCI_Statement*);

typedef int (*commit_func_p)(OCI_Connection*);
typedef int (*rollback_func_p)(OCI_Connection*);

/*
** State information about the database connection is contained in an
** instance of the following structure.
*/
// TODO remove unnecessary stuff, and add what we need.
typedef struct {
    OCI_Connection* conn;
    OCI_Statement* current_stmt;
    stmt_c_func_p create_stmt;
    stmt_e_func_p exec_stmt;
    stmt_f_func_p free_stmt;
    commit_func_p commit;
    rollback_func_p rollback;

    unsigned mEqpLines;    
    int outCount;          
    int cnt;               
    int lineno;            
    int openFlags;         
    FILE *ifd;             
    FILE *tfd;             
    int nErr;              
    int mode;              
    int modePrior;         
    int cMode;             
    int normalMode;        
    int showHeader;        
    int nCheck;            
    unsigned nProgress;    
    unsigned mxProgress;   
    unsigned flgProgress;  
    unsigned shellFlgs;    
    unsigned priorShFlgs;  
    char zTestcase[30];    
    char colSeparator[20]; 
    char rowSeparator[20]; 
    char colSepPrior[20];  
    char rowSepPrior[20];  
    int colWidth[100];     
    int actualWidth[100];  
    char nullValue[20];    
    FILE *pLog;            
    int *aiIndent;         
    int nIndent;           
    int iIndent;           
} orash_t;

int init_orash(orash_t* sh, char* user, char* pwd, char* db);
void cleanup_orash(orash_t* sh);

#endif /* __ORASH_H */
