#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/linenoise.h"
#include "../inc/builtin.h"
#include "../inc/config.h"
#include "../inc/utils.h"


void _completion(const char *buf, linenoiseCompletions *lc) {
    char* match = NULL;
    if (find_matching(buf, &match))
    {
        linenoiseAddCompletion(lc, match);
    }
}

char* _hints(const char *buf, int *color, int *bold) {

    if(strlen(buf) == 0)
    {
        return "";
    }

    char* match = NULL;

    if (find_matching(buf, &match))
    {
        int len = strlen(buf);
        match = &match[len];
        *color = 35;
        *bold = 0;
        return match;
    }
    return "";
}

int _builtin(const char *buf) {

    int success = 1;
    builtin_t* builtin;

    if (get_func_by_name(buf, &builtin))
    {
        return success;
    }

    size_t len = strlen(buf);
    if (len > 0)
    {
        if (buf[len-1] == ';') //TODO maybe we need a better handling, but for now that should work.
        {
            return success;
        }
    }

    return -1;
}

void init_history(void)
{
    linenoiseHistoryLoad(hist_path);
    linenoiseHistorySetMaxLen(hist_length);
}

void init_completion(void)
{
    linenoiseSetCompletionCallback(_completion);
    linenoiseSetHintsCallback(_hints);
    linenoiseSetBuiltinCallback(_builtin);
}

#if _DEBUG
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

void segv_handler(int sig) {
    void *array[20];
    size_t frames;

    extern struct termios* p_termios;
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&(*p_termios));

    frames = backtrace(array, 20);
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, frames, STDERR_FILENO);
    exit(1);
}
#endif

int main(int argc, char **argv) {

#if _DEBUG
    signal(SIGSEGV, segv_handler);
#endif

    char *line;
    linenoiseSetMultiLine(1);
    while(argc > 1) {
        argc--;
        argv++;

        if (!strcmp(*argv,"--keys")) {
            linenoisePrintKeyCodes();
        }
    }

    init_history();
    init_completion();

    while((line = linenoise(prompt)) != NULL) {
        builtin_t* builtin;

        if (line[0] != '\0' && line[0] != '/') {

            if (get_func_by_name(line, &builtin))
            {
                linenoiseHistoryAdd(line);
                linenoiseHistorySave(hist_path);
                // now call builtin func
                builtin->func(builtin->args);
            }
            else
            {
                printf("Command not found: %s\n", line);
            }
        }
        //free(builtin->args);
        free(line);
    }
    return 0;
}
