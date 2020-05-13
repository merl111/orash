#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <oci.h>
#include "utils.h"

/*
 ** Return TRUE if a semicolon occurs anywhere in the first N characters
 ** of string z[].
 */
int contains_semicolon(const char *z, int N)
{
    int i;
    for(i=0; i<N; i++) {  if( z[i]==';' ) return 1; }
    return 0;
}

/*
 ** Test to see if a line consists entirely of whitespaces.
 */
int only_whitespace(const char *z)
{
    for(; *z; z++)
    {
        if(IsSpace(z[0])) continue;

        if(*z=='/' && z[1]=='*')
        {
            z += 2;
            while(*z && (*z!='*' || z[1]!='/')) { z++; }

            if(*z==0) return 0;
            z++;

            continue;
        }

        if (*z=='-' && z[1]=='-')
        {
            z += 2;
            while (*z && *z!='\n') { z++; }
            if (*z==0) return 1;

            continue;
        }

        return 0;
    }

    return 1;
}

int is_complete_sql(char *sql, int n)
{
    //TODO either needs sql parser, or step is disrcarded and command is directly sent to the db, no se yet...
    int rc = 1;
    //if (sql==0) return 1;
    //sql[n] = ';';
    //sql[n+1] = 0;
    //rc = complete(sql);
    //sql[n] = 0;
    return rc;
}

int is_terminator(const char *line)
{

    while( IsSpace(line[0]) ) { line++; };

    if (line[0]=='/' && only_whitespace(&line[1]))
    {
        return 1;  /* Oracle */
    }

    return 0;
}

/*
 ** Compute a string length that is limited to what can be stored in
 ** lower 30 bits of a 32-bit signed integer.
 */
int strlen30(const char *z)
{
    const char *z2 = z;
    while (*z2) { z2++; }
    return 0x3fffffff & (int)(z2 - z);
}

//void print_err(void *handle, ub4 htype)
//{
//    text errbuf[4096]; // lets hope the buffer is big enough
//    sb4 errcode = 0;
//
//    (void) OCIErrorGet(handle, 1, (text *) NULL, &errcode,
//            errbuf, (ub4) sizeof(errbuf), htype);
//    (void) printf("Error - %.*s\n", (int)sizeof(errbuf), errbuf);
//}
//
//void eval_return(void *handle, ub4 htype, sword status)
//{
//    switch (status)
//    {
//        case OCI_SUCCESS:
//            break;
//        case OCI_SUCCESS_WITH_INFO:
//            (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
//            break;
//        case OCI_NEED_DATA:
//            (void) printf("Error - OCI_NEED_DATA\n");
//            break;
//        case OCI_NO_DATA:
//            (void) printf("Error - OCI_NODATA\n");
//            break;
//        case OCI_ERROR:
//            (void) printf("Error - OCI_ERROR\n");
//            if (handle)
//            {
//                print_err(handle,htype);
//            }
//            else
//            {
//                (void) printf("NULL handle\n");
//                (void) printf("Unable to extract detailed diagnostic information\n");
//            }
//            break;
//        case OCI_INVALID_HANDLE:
//            (void) printf("Error - OCI_INVALID_HANDLE\n");
//            break;
//        case OCI_STILL_EXECUTING:
//            (void) printf("Error - OCI_STILL_EXECUTE\n");
//            break;
//        case OCI_CONTINUE:
//            (void) printf("Error - OCI_CONTINUE\n");
//            break;
//        default:
//            break;
//    }
//}
