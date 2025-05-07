#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define LOG_BUF_SIZE 1024

enum log_type
{
    LOG_VERY_MINOR = 0,
    LOG_MINOR,
    LOG_APP_CMD,
    WARN_LOW,
    WARN_HIGH,
    ERROR,
    INFO
};

extern char *log_type_str[];
extern enum log_type log_level;
extern FILE *log_file;
extern char log_buf[LOG_BUF_SIZE];

// /!\ DON'T USE MATH.H, CAUSE IT HAS ITS OWN DEFINITION FOR log()!
// or rename this log()
void set_default_log_file();
void log_to(FILE *stream, enum log_type type, char *msg);
void log_to_file_only(enum log_type type, char *msg);
void log(enum log_type type, char *msg);
void logf(enum log_type type, char *msg, va_list ap);
void log_start_log();
void log_end_log();

#endif // LOGGER_H
