#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>
#include "../inc/builtin.h"
#include "../inc/utils.h"


void print_err(void *handle, ub4 htype)
{
    text errbuf[4096]; // lets hope the buffer is big enough
    sb4 errcode = 0;

    (void) OCIErrorGet(handle, 1, (text *) NULL, &errcode,
            errbuf, (ub4) sizeof(errbuf), htype);
    (void) printf("Error - %.*s\n", (int)sizeof(errbuf), errbuf);
}

void eval_return(void *handle, ub4 htype, sword status)
{
    switch (status)
    {
        case OCI_SUCCESS:
            break;
        case OCI_SUCCESS_WITH_INFO:
            (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
            break;
        case OCI_NEED_DATA:
            (void) printf("Error - OCI_NEED_DATA\n");
            break;
        case OCI_NO_DATA:
            (void) printf("Error - OCI_NODATA\n");
            break;
        case OCI_ERROR:
            (void) printf("Error - OCI_ERROR\n");
            if (handle)
            {
                print_err(handle,htype);
            }
            else
            {
                (void) printf("NULL handle\n");
                (void) printf("Unable to extract detailed diagnostic information\n");
            }
            break;
        case OCI_INVALID_HANDLE:
            (void) printf("Error - OCI_INVALID_HANDLE\n");
            break;
        case OCI_STILL_EXECUTING:
            (void) printf("Error - OCI_STILL_EXECUTE\n");
            break;
        case OCI_CONTINUE:
            (void) printf("Error - OCI_CONTINUE\n");
            break;
        default:
            break;
    }
}
