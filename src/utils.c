#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <oci.h>
#include "utils.h"

/* Debugging macro. */
#if 1
FILE *shdbg_fp = NULL;
#define shdbg(...) \
    do { \
        if (shdbg_fp == NULL) { \
            shdbg_fp = fopen("/tmp/shdbg.txt","a"); \
        } \
        fprintf(shdbg_fp, ", " __VA_ARGS__); \
        fflush(shdbg_fp); \
    } while (0)
#else
#define lndebug(fmt, ...)
#endif
/*
 ** Return TRUE if a semicolon occurs anywhere in the first N characters
 ** of string z[].
 */
int contains_semicolon(char *z, int N, int trim)
{
    int i;
    for(i=0; i<N; i++)
    {
        if( z[i]==';' )
        {
            if (trim)
            {
                z[i] = 0;
            }
            return 1; 
        }
    }
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

int is_sql(const char* str)
{
    if (strcasecmp(str, "select") == 0 || strcasecmp(str, "insert") == 0 || strcasecmp(str, "update") == 0
            || strcasecmp(str, "delete") == 0 || strcasecmp(str, "alter") == 0 || strcasecmp(str, "commit") == 0)
    {
        return 1;
    }

    return 0;
}

int is_plsql(const char* str)
{
    shdbg("plsql check: %s \n", str);
    if (strcasecmp(str, "begin") == 0 || strcasecmp(str, "declare") == 0) 
    {
        return 1;
    }

    return 0;
}

void identify_input(const char* input, int* mode)
{
    /*
     * mode = 1 sql/other statement
     * mode = 2 plsql
     *
     */

    const char* del = " ";
    int len = strlen(input);
    char* input_copy = malloc(len * sizeof(char));
    char* head = NULL;
    char* saveptr= NULL;

    shdbg("input: %s \n", input);

    if (input_copy == NULL)
    {
        *mode = -1;
    }

    strncpy(input_copy, input, len);

    head = strtok_r(input_copy, del, &saveptr);
    /* add NULL terminator */
    head[len-strlen(saveptr)] = 0;

    shdbg("head: %s \n", head);

    if (is_sql(head))
    {
        shdbg("is sql %s \n", head);
        *mode = 1;
    }
    else if (is_plsql(head))
    {
        shdbg("is plsql %s \n", head);
        *mode = 2;
    }
    else
    {
        shdbg("fail %s \n", head);
        *mode = -2;
    }

    free(input_copy);
}

void parse_login(const char* input, char** user, char** pwd, char** db)
{
    const char* del1 = "/";
    const char* del2 = "@";
    int len = strlen(input);
    char* input_copy = malloc(len);
    char* head = NULL;
    char* saveptr = NULL;

    strncpy(input_copy, input, len);

    head = strtok_r(input_copy, del1, &saveptr);
    *user = calloc(strlen(head), sizeof(char));
    strncpy(*user, head, strlen(head));

    head = strtok_r(NULL, del2, &saveptr);
    *pwd = calloc(strlen(head), sizeof(char));
    strncpy(*pwd, head, strlen(head));

    head = strtok_r(NULL, "\0", &saveptr);
    *db = calloc(strlen(head), sizeof(char));
    strncpy(*db, head, strlen(head));

    free(input_copy);
}
