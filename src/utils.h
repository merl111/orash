#include <ctype.h>
#ifndef __UTILS_H
#define __UTILS_H
#define UNUSED(x) (void)(x)

/* ctype macros that work with signed characters */
#define IsSpace(X)  isspace((unsigned char)X)
#define IsDigit(X)  isdigit((unsigned char)X)
#define ToLower(X)  (char)tolower((unsigned char)X)

int contains_semicolon(const char *z, int N);
int only_whitespace(const char *z);
int is_terminator(const char *line);
int strlen30(const char *z);
int is_complete_sql(char *sql, int n);

#endif /* __UTILS_H */
