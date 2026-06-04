#include "logger.h"

#include <stdio.h>
#include <string.h>

int is_file = 0;
FILE *logs_stream = NULL;
FILE *errors_stream = NULL;
LOG_LEVEL log_level = LOGA;
char *log_prefixes[] = { "NONE", "ERRO", "WARN", "LOGA", "LOGM" };

static void setup_default_streams(void)
{
    logs_stream = stdout;
    errors_stream = stderr;
    is_file = 0;
}

void log_msg(LOG_LEVEL level, char *msg)
{
    if (!logs_stream)
    {
        setup_default_streams();
    }

    if (level > log_level)
    {
        return;
    }

    FILE *final_stream = logs_stream;
    if (level == ERRO || level == WARN)
    {
        final_stream = errors_stream;
    }

    // TODO: add time at the start of the message
    char tag[512] = {0};
    sprintf(tag, "[ %s ] ", log_prefixes[level]);

    fprintf(final_stream, "%s%s%s", tag, msg, NEWLINE);
}

void set_log_file(char *path, char *mode)
{
    FILE *file = fopen(path, mode);
    if (!file)
    {
        fprintf(stderr, "ERROR: Couldn't open file %s\n", path);
        return;
    }

    is_file = 1;
    logs_stream = file;
    errors_stream = file;
}

void set_log_level(LOG_LEVEL level)
{
    log_level = level;
}

void close_log_file(void)
{
    if (!is_file)
    {
        fprintf(stderr, "WARN: There are no log files. Returning.");
        return;
    }

    fclose(logs_stream);
}