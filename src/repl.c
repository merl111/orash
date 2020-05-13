#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "orash.h"
#include "utils.h"
#include "linenoise.h"


void _completion(const char *buf, linenoiseCompletions *lc) 
{
    char* match = NULL;
    if (find_matching(buf, &match))
    {
        linenoiseAddCompletion(lc, match);
    }
}

char* _hints(const char *buf, int *color, int *bold) 
{

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

int _builtin(const char *buf)
{
    builtin_t* builtin;

    if (get_func_by_name(buf, &builtin))
    {
        return 1;
    }

    size_t len = strlen(buf);
    if (len > 0)
    {
        if (buf[len-1] == ';') //TODO maybe we need a better handling, but for now that should work.
        {
            return 1;
        }
    }

    return 0;
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
}

#if _DEBUG
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

/* only used for debugging, don't like destroyed shell after fail! */
void segv_handler(int sig)
{
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

static void out_of_memory(void)
{
    fprintf(stderr,"Error: ORASH ran out of memory! \
            Sir, can I have some more please...\n");
    exit(-1);
}

static void get_line(char** result, int continous)
{
    char *ora_prompt;

    ora_prompt = continous ? ">> " : "orash> ";
    *result = linenoise(ora_prompt);
    //if( ora_result && *ora_result ) shell_add_history(ora_result);
}

static int get_input(orash_t *p)
{
    char *line = 0;          /* A single input line */
    char *sql = 0;           /* Accumulated SQL text */
    int len_line = 0;                /* Length of current line */
    int nAlloc = 0;           /* Allocated sql[] space */
    int len_sql = 0;             /* Bytes of sql[] used */
    int prev_len_sql = 0;        /* Bytes of sql[] used by prior line */
    int errCnt = 0;           /* Number of errors seen */
    int startline = 0;        /* Line number for start of current input */

    p->lineno = 0;
    while(1)
    {
        fflush(p->ofd);
        get_line(&line, len_sql > 0);
        if( line==0 )
        {
            /* End of input */
            if(p->ifd == 0) printf("\n");
            break;
        }

        p->lineno++;
        if( len_sql == 0 && only_whitespace(line) )
        {
            continue;
        }

        if( line && len_sql == 0 )
        {
            builtin_t* builtin;
            if (get_func_by_name(line, &builtin))
            {
                linenoiseHistoryAdd(line);
                linenoiseHistorySave(hist_path);

                // now call builtin func
                builtin->func(builtin->args);
                continue;
            }
        }

        if (is_terminator(line) && is_complete_sql(sql, len_sql))
        {
            memcpy(line,";",2);
        }

        len_line = strlen30(line);
        if (len_sql + len_line + 2 >= nAlloc)
        {

            nAlloc = len_sql + len_line+100;
            sql = realloc(sql, nAlloc);

            if( sql==0 ) out_of_memory();
        }

        prev_len_sql = len_sql;
        if (len_sql==0)
        {
            int i;
            for(i=0; line[i] && IsSpace(line[i]); i++){}
            assert( nAlloc>0 && sql!=0 );
            memcpy(sql, line+i, len_line+1-i);
            startline = p->lineno;
            len_sql = len_line-i;
        }
        else
        {
            sql[len_sql++] = '\n';
            memcpy(sql+len_sql, line, len_line+1);
            len_sql += len_line;
        }

    }

    if (len_sql && !only_whitespace(sql))
    {
        //TODO
        //errCnt += runStatement(p, sql, p->ifd, startline);
    }

    free(sql);
    free(line);
    return errCnt>0;
}

int main(int argc, char **argv) {

    int ret;
#if _DEBUG
    signal(SIGSEGV, segv_handler);
#endif

    /*
     * mlmode 1 = line wrapping
     * mlmode 2 = multi line editing
     */

    linenoiseSetMultiLine(2);
    while(argc > 1) {
        argc--;
        argv++;

        if (!strcmp(*argv,"--keys")) {
            linenoisePrintKeyCodes();
        }
    }

    init_history();
    init_completion();


    orash_t sh_state;

    //TODO init sh_state

    ret = get_input(&sh_state);
    return 0;
}
