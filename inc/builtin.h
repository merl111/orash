#ifndef __BUILTIN_H
#define __BUILTIN_H

// should do for now, just remember to not pass a pointer!
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int builtin_exit(void);
int builtin_ls(void);
int builtin_clear(void);
int builtin_pwd(void);
int builtin_help(void);

typedef int (*func)();

typedef struct builtin {
    char *name;
    int (*func)();
} builtin_t;

int get_builtin_count(void);
builtin_t* get_builtins(void);
func get_func_by_name(const char* name);
int find_matching(const char* buf, char** match);


#endif /* __BUILTIN_H */
