#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "../inc/linenoise.h"
#include "../inc/builtin.h"

builtin_t builtins[] = { 
    {"exit", builtin_exit},
    {"ls", builtin_ls},
    {"clear", builtin_clear},
    {"pwd", builtin_pwd},
    {"edit", builtin_edit},
    {"help", builtin_help},
};

int get_builtin_count(void)
{
    return (int)ARRAYSIZE(builtins);
}

builtin_t* get_builtins(void)
{
    return &builtins[0];
}

func get_func_by_name(const char* name)
{
    for (int i = 0; i < get_builtin_count(); i++)
    {
        //return function pointer if the name is an exact match
        if (strcmp(builtins[i].name, name) == 0)
        {
            return builtins[i].func;
        }
    }

    return NULL;
}

int find_matching(const char* buf, char** match)
{
    // find a match for buf 
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

int builtin_edit(void)
{
    /* 
     * For simplicity we use system() for now, but
     * we should probably fork another process here.
     */
    const char* file = "test.txt";
    const char* editor = getenv("EDITOR");
    char buffer[512];
    snprintf(buffer, 512, "%s %s", "geany", file);
    int status = system(buffer);
    return 0;
}

int builtin_pwd(void)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);

    return 0;
}

int builtin_exit(void)
{
    printf("shutting down now!");
    exit(EXIT_SUCCESS);
}

int builtin_ls(void)
{
    //TODO now it's time to define an argument for builtin methods
    //struct dirent **namelist;
    //int n;

    //if (args.something == NULL)
    //{
    //    n = scandir(".",&namelist,NULL,alphasort);
    //}
    //else
    //{
    //    n = scandir(args[1], &namelist, NULL, alphasort);
    //}
    //if(n < 0)
    //{
    //    perror("scandir");
    //    exit(EXIT_FAILURE);
    //}
    //else
    //{
    //    while (n--)
    //    {
    //        printf("%s\n",namelist[n]->d_name);
    //        free(namelist[n]);
    //    }
    //    free(namelist);
    //}

    return 0;
}

int builtin_clear(void)
{
    linenoiseClearScreen();
    return 0;
}

int builtin_help(void)
{
    return 0;
}
