#include <stdio.h>
#include "linenoise.h"
#include "ocilib/include/ocilib.h"

#ifndef __ORASH_H
#define __ORASH_H

/*
** State information about the database connection is contained in an
** instance of the following structure.
*/
// TODO remove unnecessary stuff, and add what we need.
typedef struct {
    OCI_Connection* conn;
    unsigned mEqpLines;    
    int outCount;          
    int cnt;               
    int lineno;            
    int openFlags;         
    FILE *ifd;             
    FILE *ofd;             
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
