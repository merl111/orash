#include <stdio.h>
#include "orash.h"
#include "ocilib/include/ocilib.h"

void err_handler(OCI_Error *err)
{
    printf("%s\n", OCI_ErrorGetString(err));
}

int init_orash(orash_t* sh, char* user, char* pwd, char* db)
{

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
    {
        return EXIT_FAILURE;
    }

    sh->conn = OCI_ConnectionCreate(db, user, pwd, OCI_SESSION_DEFAULT);

    printf("Server major    version : %i\n", OCI_GetServerMajorVersion(sh->conn));
    printf("Server minor    version : %i\n", OCI_GetServerMinorVersion(sh->conn));
    printf("Server revision version : %i\n\n", OCI_GetServerRevisionVersion(sh->conn));
    printf("Connection      version : %i\n\n", OCI_GetVersionConnection(sh->conn));


    return EXIT_SUCCESS;
}

void cleanup_orash(orash_t* sh)
{
    OCI_ConnectionFree(sh->conn);
    OCI_Cleanup();
    //TODO calls the clenaup functions on the struct

}

