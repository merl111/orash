#include <stdio.h>
#include "orash.h"
#include "fort.h"
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

    sh->conn        = OCI_ConnectionCreate(db, user, pwd, OCI_SESSION_DEFAULT);
    sh->create_stmt = OCI_StatementCreate;
    sh->exec_stmt   = OCI_ExecuteStmt;
    sh->free_stmt   = OCI_StatementFree;
    sh->commit      = OCI_Commit;
    sh->rollback    = OCI_Rollback;


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

void print_rs_table(OCI_Resultset* rs)
{
    int n = OCI_GetColumnCount(rs);

    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

    for(int i = 1; i <= n; i++)
    {
        OCI_Column *col = OCI_GetColumn(rs, i);

        char *name = (char*)OCI_ColumnGetName(col);
        if (i < n)
        {
            ft_write(table, name);
        }
        else
        {
            ft_write_ln(table, name);
        }
    }

    otext* value = NULL;
    while (OCI_FetchNext(rs))
    {
        for(int i = 1; i <= n; i++)
        {
            OCI_Column *col = OCI_GetColumn(rs, i);
            unsigned int column_type = OCI_ColumnGetType(col);

            switch (column_type)
            {
                case OCI_CDT_NUMERIC:
                case OCI_CDT_TEXT:
                    value = (otext*)OCI_GetString(rs, i);
                    break;

                case OCI_CDT_DATETIME:
                    value = (otext*)OCI_GetString(rs, i);
                    break;

                default:
                    value = (otext*)OCI_GetString(rs, i);
                    break;
            }

            if (!value)
            {
                value = "<NULL>";
            }

            if (i < n)
            {
                ft_write(table, value);
            }
            else
            {
                ft_write_ln(table, value);
            }
        }
    }

    printf("%s\n", ft_to_string(table));
    printf("%d rows selected\n", OCI_API OCI_GetRowCount(rs));

    /* free willy */
    ft_destroy_table(table);
}

