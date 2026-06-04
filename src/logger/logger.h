#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifdef _WIN32
    #define NEWLINE "\r\n"
    #define LEN_NEWLINE 2
#else
    #define NEWLINE "\n"
    #define LEN_NEWLINE 1
#endif

typedef enum
{
    NONE = 0,
    ERRO,
    WARN,
    LOGA,
    LOGM
} LOG_LEVEL;


extern FILE *logs_stream;
extern LOG_LEVEL log_level;

void log_msg(LOG_LEVEL level, char *msg);
void set_log_file(char *path, char *mode);
void set_log_level(LOG_LEVEL level);
void close_log_file(void);

#endif /* LOGGER_H */