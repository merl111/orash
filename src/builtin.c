#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../inc/linenoise.h"
#include "../inc/builtin.h"
#include "../inc/utils.h"

builtin_t builtins[] = { 
    { "exit",   NULL, builtin_exit},
    { "ls",     NULL, builtin_ls},
    { "clear",  NULL, builtin_clear},
    { "pwd",    NULL, builtin_pwd},
    { "edit",   NULL, builtin_edit},
    { "help",   NULL, builtin_help},
};

int get_builtin_count(void)
{
    return (int)ARRAYSIZE(builtins);
}

builtin_t* get_builtins(void)
{
    return &builtins[0];
}

int get_func_by_name(const char* line, builtin_t** builtin)
{
    const char* delimiter = " ";
    char* method; 
    char* args;
    int success = 0;

    size_t len = strlen(line);
    char* line_cpy = (char*) calloc(len + 1, sizeof(char));
    strncpy(line_cpy, line, len); //TODO unsecure, fix later
    line_cpy[len] = '\0';

    method = strtok_r(line_cpy, delimiter, &args);

    if (method == NULL)
    {
        return success;
    }

    for (int i = 0; i < get_builtin_count(); i++)
    {
        //return function pointer if the name is an exact match
        if (strcmp(builtins[i].name, method) == 0)
        {
            if (strlen(args) > 0)
            {
                builtins[i].args = malloc(strlen(args) * sizeof(char));
                strncpy(builtins[i].args, args, strlen(args));
                builtins[i].args[strlen(args)] = '\0';
            }
            else
            {
                builtins[i].args = "";
            }

            *builtin = &builtins[i];
            success = 1;
        }
    }

    free(line_cpy);
    return success;
}

int find_matching(const char* buf, char** match)
{
    for (int i = 0; i < get_builtin_count(); i++)
    {
        if (strstr(builtins[i].name, buf) == builtins[i].name)
        {
            *match = builtins[i].name;
            return 1;
        }
    }

    return 0;
}

int builtin_edit(char* args)
{
    /* 
     * For simplicity we use system() for now, but
     * we should probably fork another process here.
     */
    const char* editor = getenv("EDITOR");
    char buffer[512];
    snprintf(buffer, 512, "%s %s", editor, args); //TODO clang warning, unsecure snprintf, I know I know
    system(buffer);
    return 0;
}

int builtin_pwd(char* args)
{
    UNUSED(args);

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);

    return 0;
}

int builtin_exit(char* args)
{
    UNUSED(args);
    printf("shutting down now!");
    exit(EXIT_SUCCESS);
}

int builtin_ls(char* args)
{
    if (args != NULL)
    {
        char cmd[strlen(args) + 3];
        snprintf(cmd, strlen(args)+4, "ls %s", args);
        system(cmd);
    }

    return 0;
}

int builtin_clear(char* args)
{
    UNUSED(args);
    linenoiseClearScreen();
    return 0;
}

int builtin_help(char* args)
{
    UNUSED(args);
    return 0;
}
