#ifndef __BUILTIN_H
#define __BUILTIN_H

// should do for now, just remember to not pass a pointer!
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int builtin_exit(char* args);
int builtin_ls(char* args);
int builtin_clear(char* args);
int builtin_pwd(char* args);
int builtin_help(char* args);
int builtin_edit(char* args);

typedef int (*func)(char* args[]);

typedef struct builtin {
    char* name;
    char* args;
    int (*func)();
} builtin_t;

int get_builtin_count(void);
builtin_t* get_builtins(void);
int get_func_by_name(const char* name, builtin_t** builtin);
int find_matching(const char* buf, char** match);

#endif /* __BUILTIN_H */
