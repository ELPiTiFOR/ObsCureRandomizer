#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

void mkdir_p(char *path);
void copy_file(char *src, char *dst);
void free_argv(char **argv);
char *str_from_file(char *src, size_t *total_written);
char **str_to_argv(char *str, int *argc);
int get_buf_from_file(FILE *file, size_t from, size_t to, unsigned char *buf);
int file_from_string(char *dst, char *content, size_t len);

#endif // FILE_IO_H
