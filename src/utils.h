#include <ctype.h>
#ifndef __UTILS_H
#define __UTILS_H
#define UNUSED(x) (void)(x)

/* ctype macros that work with signed characters */
#define IsSpace(X)  isspace((unsigned char)X)
#define IsDigit(X)  isdigit((unsigned char)X)
#define ToLower(X)  (char)tolower((unsigned char)X)

#define contains_semicolon_nt(x, y) contains_semicolon(x,y,0)

int contains_semicolon(char *z, int N, int trim);
int only_whitespace(const char *z);
int is_terminator(const char *line);
int strlen30(const char *z);
int is_complete_sql(char *sql, int n);

void parse_login(const char* input, char** user, char** pwd, char** db);
void identify_input(const char* input, int* mode);

#endif /* __UTILS_H */
