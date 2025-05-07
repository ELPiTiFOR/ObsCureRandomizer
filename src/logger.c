#include "logger.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "time.h"
#include "utils.h"

//char *log_type_str[] = { "LOG_APP_CMD", "LOG_MINOR", "WARN_LOW", "WARN_HIGH", "ERROR", "INFO" };
char *log_type_str[] = { "LOG", "LOG", "LOG", "WARN", "WARN", "ERROR", "INFO" };
enum log_type log_level = LOG_APP_CMD;
FILE *log_file = NULL;
char log_buf[LOG_BUF_SIZE];

void set_default_log_file()
{
    arfillzeros(path_logs, 512);
    strcpy(path_logs, "OCR.log");
    log_file = fopen(path_logs, "a+");
    if (!log_file)
    {
        fprintf(stderr, "ERROR: Couldn't open log_file %s", path_logs);
        return;
    }

    log_start_log();
}

void log_to(FILE *stream, enum log_type type, char *msg)
{
    fseek(stream, 0, SEEK_END);

    time_t now_t = time(NULL);
    struct tm tm = *localtime(&now_t);
    fprintf(stream, "[%02d/%02d/%d %02d:%02d:%02d] ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char *str = log_type_str[type];
    fwrite(str, 1, strlen(str), stream);
    fwrite(": ", 1, 2, stream);
    fwrite(msg, 1, strlen(msg), stream);
}

void log_to_file_only(enum log_type type, char *msg)
{
    if (!log_file)
    {
        log(WARN_LOW, "Couldn't log a log_file-only message\n");
        return;
    }

    log_to(log_file, type, msg);
}

void log(enum log_type type, char *msg)
{
    if (log_file)
    {
        //printf("I'm logging to log_file\n");
        log_to(log_file, type, msg);
        //printf("I logged to log_file\n");
        if (type == WARN_HIGH || type == WARN_LOW)
        {
            //printf("I logged to log_file a WARNING\n");
        }
    }
    else
    {
        // if there is no log_file, we log everything in stdout
        // no matter the log_level
        //printf("I'm logging to stdout\n");
        log_to(stdout, type, msg);
        //printf("I logged to stdout\n");
        return;
    }

    if (type >= log_level)
    {
        // TODO: if the type is of type ERROR or WARN, stderr should be used
        //printf("I'm logging to log_file because of the log_level\n");
        log_to(stdout, type, msg);
        //printf("I logged to log_file because of the log_level\n");
    }
}

/* TODO
void logf(enum log_type type, char *msg, va_list ap);
*/

void log_start_log()
{
    log_to_file_only(INFO, "Logging started.\n");
}

void log_end_log()
{
    log_to_file_only(INFO, "Logging ended.\n");
}
