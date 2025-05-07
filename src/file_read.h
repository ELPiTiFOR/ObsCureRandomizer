#ifndef FILE_READ_H
#define FILE_READ_H

#include <stddef.h>
#include <stdio.h>

int check_pattern(FILE *file, unsigned char *pat, size_t pat_len);
ssize_t search_pattern(FILE* file, unsigned char *pat, size_t pat_len);

#endif // FILE_READ_H
