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
    char* match = NULL;
    if (find_matching(buf, &match))
    {
        *color = 35;
        *bold = 0;
        return match;
    }
    return NULL;
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

int main(int argc, char **argv) {
    char *line;

    while(argc > 1) {
        argc--;
        argv++;
        if (!strcmp(*argv,"--multiline")) {
            linenoiseSetMultiLine(1);
            printf("Multi-line mode enabled.\n");
        }
    }

    init_history();
    init_completion();

    while((line = linenoise(prompt)) != NULL) {

        if (line[0] != '\0' && line[0] != '/') {

            func method = get_func_by_name(line);
            if (method != NULL)
            {
                linenoiseHistoryAdd(line);
                linenoiseHistorySave(hist_path);
                method();
            }
            else
            {
                printf("Command not found: %s\n", line);
            }
        }

        free(line);
    }
    return 0;
}
